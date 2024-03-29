/*
 * This file was created for CMPS111 Sprint 2012 at UCSC
 * Authors: Benjamin Ross, Quentin Rivers, Matthew Musselman
 */
/* This file contains the wrapper functions for issuing a request
 * and receiving response from FS processes.
 * Each function builds a request message according to the request
 * parameter, calls the most low-level fs_sendrec, and copies
 * back the response.
 */

#include "fs.h"
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/statvfs.h>
#include <minix/vfsif.h>
#include <minix/com.h>
#include <minix/const.h>
#include <minix/endpoint.h>
#include <minix/u64.h>
#include <unistd.h>
#include <minix/vfsif.h>
#include "fproc.h"
#include "vmnt.h"
#include "vnode.h"
#include "path.h"
#include "param.h"




/*===========================================================================*
 *			req_breadwrite					     *
 *===========================================================================*/
PUBLIC int req_breadwrite(
  endpoint_t fs_e,
  endpoint_t user_e,
  dev_t dev,
  u64_t pos,
  unsigned int num_of_bytes,
  char *user_addr,
  int rw_flag,
  u64_t *new_posp,
  unsigned int *cum_iop
)
{
  int r;
  cp_grant_id_t grant_id;
  message m;
  grant_id = cpf_grant_magic(fs_e, user_e, (vir_bytes) user_addr, num_of_bytes,
			(rw_flag == READING ? CPF_WRITE : CPF_READ));
  if(grant_id == -1)
	  panic("req_breadwrite: cpf_grant_magic failed");

  /* Fill in request message */
  m.m_type = rw_flag == READING ? REQ_BREAD : REQ_BWRITE;
  m.REQ_DEV2 = dev;
  m.REQ_GRANT = grant_id;
  m.REQ_SEEK_POS_LO = ex64lo(pos);
  m.REQ_SEEK_POS_HI = ex64hi(pos);
  m.REQ_NBYTES = num_of_bytes;
  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);
  if (r != OK) return(r);

  /* Fill in response structure */
  *new_posp = make64(m.RES_SEEK_POS_LO, m.RES_SEEK_POS_HI);
  *cum_iop = m.RES_NBYTES;

  return(OK);
}


/*===========================================================================*
 *				req_chmod	      			     *
 *===========================================================================*/
PUBLIC int req_chmod(
  int fs_e,
  ino_t inode_nr,
  mode_t rmode,
  mode_t *new_modep
)
{
  message m;
  int r;

  /* Fill in request message */
  m.m_type = REQ_CHMOD;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_MODE = rmode;
  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);

  /* Copy back actual mode. */
  *new_modep = m.RES_MODE;

  return(r);
}

/*===========================================================================*
 *				req_setkey	      			     *
 *===========================================================================*/

PUBLIC int req_setkey( int fs_e, int k0, int k1)
{
    message m;
    m.m_type = REQ_SETKEY;
    int r;
    m.m1_i1 = k0;
    m.m1_i2 = k1;
    r = fs_sendrec(fs_e, &m);
    return(r);
}

/*===========================================================================*
 *				req_chown          			     *
 *===========================================================================*/
PUBLIC int req_chown(
  endpoint_t fs_e,
  ino_t inode_nr,
  uid_t newuid,
  gid_t newgid,
  mode_t *new_modep
)
{
  message m;
  int r;

  /* Fill in request message */
  m.m_type = REQ_CHOWN;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_UID = newuid;
  m.REQ_GID = newgid;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);

  /* Return new mode to caller. */
  *new_modep = m.RES_MODE;

  return(r);
}


/*===========================================================================*
 *				req_create				     *
 *===========================================================================*/
PUBLIC int req_create(
  int fs_e,
  ino_t inode_nr,
  int omode,
  uid_t uid,
  gid_t gid,
  char *path,
  node_details_t *res
)
{
  int r;
  cp_grant_id_t grant_id;
  size_t len;
  message m;

  if (path[0] == '/')
	panic("req_create: filename starts with '/'");

  len = strlen(path) + 1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes) path, len, CPF_READ);
  if (grant_id == -1)
	panic("req_create: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type	= REQ_CREATE;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_MODE	= omode;
  m.REQ_UID	= uid;
  m.REQ_GID	= gid;
  m.REQ_GRANT	= grant_id;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);
  if (r != OK) return(r);

  /* Fill in response structure */
  res->fs_e	= m.m_source;
  res->inode_nr	= m.RES_INODE_NR;
  res->fmode	= m.RES_MODE;
  res->fsize	= m.RES_FILE_SIZE_LO;
  res->uid	= m.RES_UID;
  res->gid	= m.RES_GID;
  res->dev	= m.RES_DEV;

  return(OK);
}


/*===========================================================================*
 *				req_flush	      			     *
 *===========================================================================*/
PUBLIC int req_flush(endpoint_t fs_e, dev_t dev)
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_FLUSH;
  m.REQ_DEV = dev;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_fstatfs	    			     *
 *===========================================================================*/
PUBLIC int req_fstatfs(int fs_e, int proc_e, char *buf)
{
  int r;
  cp_grant_id_t grant_id;
  message m;

  grant_id = cpf_grant_magic(fs_e, proc_e, (vir_bytes) buf, sizeof(struct statfs),
			CPF_WRITE);
  if(grant_id == -1)
	  panic("req_fstatfs: cpf_grant_magic failed");

  /* Fill in request message */
  m.m_type = REQ_FSTATFS;
  m.REQ_GRANT = grant_id;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_statvfs	    			     *
 *===========================================================================*/
PUBLIC int req_statvfs(int fs_e, int proc_e, char *buf)
{
  int r;
  cp_grant_id_t grant_id;
  message m;

  grant_id = cpf_grant_magic(fs_e, proc_e, (vir_bytes) buf, sizeof(struct statvfs),
			CPF_WRITE);
  if(grant_id == -1)
	  panic("req_statvfs: cpf_grant_magic failed");

  /* Fill in request message */
  m.m_type = REQ_STATVFS;
  m.REQ_GRANT = grant_id;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_ftrunc	     			     *
 *===========================================================================*/
PUBLIC int req_ftrunc(endpoint_t fs_e, ino_t inode_nr, off_t start, off_t end)
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_FTRUNC;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_TRC_START_LO = start;
  m.REQ_TRC_START_HI = 0;	/* Not used for now, so clear it. */
  m.REQ_TRC_END_LO = end;
  m.REQ_TRC_END_HI = 0;		/* Not used for now, so clear it. */

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_getdents	     			     *
 *===========================================================================*/
PUBLIC int req_getdents(
  endpoint_t fs_e,
  ino_t inode_nr,
  u64_t pos,
  char *buf,
  size_t size,
  u64_t *new_pos,
  int direct
)
{
  int r;
  message m;
  cp_grant_id_t grant_id;

  if (direct) {
	grant_id = cpf_grant_direct(fs_e, (vir_bytes) buf, size,
								CPF_WRITE);
  } else {
	grant_id = cpf_grant_magic(fs_e, who_e, (vir_bytes) buf, size,
								CPF_WRITE);
  }

  if (grant_id < 0)
	panic("req_getdents: cpf_grant_direct/cpf_grant_magic failed: %d",
								grant_id);

  m.m_type = REQ_GETDENTS;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_GRANT = grant_id;
  m.REQ_MEM_SIZE = size;
  m.REQ_SEEK_POS_LO = ex64lo(pos);
  m.REQ_SEEK_POS_HI = 0;	/* Not used for now, so clear it. */

  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  if (r == OK) {
	  *new_pos = cvul64(m.RES_SEEK_POS_LO);
	  r = m.RES_NBYTES;
  }

  return(r);
}

/*===========================================================================*
 *				req_inhibread	  			     *
 *===========================================================================*/
PUBLIC int req_inhibread(endpoint_t fs_e, ino_t inode_nr)
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_INHIBREAD;
  m.REQ_INODE_NR = inode_nr;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_link	       			     *
 *===========================================================================*/
PUBLIC int req_link(
  endpoint_t fs_e,
  ino_t link_parent,
  char *lastc,
  ino_t linked_file
)
{
  int r;
  cp_grant_id_t grant_id;
  const size_t len = strlen(lastc) + 1;
  message m;

  grant_id = cpf_grant_direct(fs_e, (vir_bytes)lastc, len, CPF_READ);
  if(grant_id == -1)
	  panic("req_link: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_LINK;
  m.REQ_INODE_NR = linked_file;
  m.REQ_DIR_INO = link_parent;
  m.REQ_GRANT = grant_id;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_lookup	                   	     *
 *===========================================================================*/
PUBLIC int req_lookup(
  endpoint_t fs_e,
  ino_t dir_ino,
  ino_t root_ino,
  uid_t uid,
  gid_t gid,
  struct lookup *resolve,
  lookup_res_t *res,
  struct fproc *rfp
)
{
  int r;
  size_t len;
  cp_grant_id_t grant_id=0, grant_id2=0;
  message m;
  vfs_ucred_t credentials;
  int flags;

  grant_id = cpf_grant_direct(fs_e, (vir_bytes) resolve->l_path, PATH_MAX,
			      CPF_READ | CPF_WRITE);
  if(grant_id == -1)
	  panic("req_lookup: cpf_grant_direct failed");

  flags = resolve->l_flags;
  len = strlen(resolve->l_path) + 1;

  m.m_type		= REQ_LOOKUP;
  m.REQ_GRANT		= grant_id;
  m.REQ_PATH_LEN 	= len;
  m.REQ_PATH_SIZE 	= PATH_MAX + 1;
  m.REQ_DIR_INO 	= dir_ino;
  m.REQ_ROOT_INO 	= root_ino;

  if(rfp->fp_ngroups > 0) { /* Is the process member of multiple groups? */
	/* In that case the FS has to copy the uid/gid credentials */
	int i;

	/* Set credentials */
	credentials.vu_uid = rfp->fp_effuid;
	credentials.vu_gid = rfp->fp_effgid;
	credentials.vu_ngroups = rfp->fp_ngroups;
	for (i = 0; i < rfp->fp_ngroups; i++)
		credentials.vu_sgroups[i] = rfp->fp_sgroups[i];

	grant_id2 = cpf_grant_direct(fs_e, (vir_bytes) &credentials,
				     sizeof(credentials), CPF_READ);
	if(grant_id2 == -1)
		panic("req_lookup: cpf_grant_direct failed");

	m.REQ_GRANT2	= grant_id2;
	m.REQ_UCRED_SIZE= sizeof(credentials);
	flags		|= PATH_GET_UCRED;
  } else {
	/* When there's only one gid, we can send it directly */
	m.REQ_UID	= uid;
	m.REQ_GID	= gid;
	flags		&= ~PATH_GET_UCRED;
  }

  m.REQ_FLAGS		= flags;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);
  if(rfp->fp_ngroups > 0) cpf_revoke(grant_id2);

  /* Fill in response according to the return value */
  res->fs_e = m.m_source;

  switch (r) {
  case OK:
	  res->inode_nr = m.RES_INODE_NR;
	  res->fmode = m.RES_MODE;
	  res->fsize = m.RES_FILE_SIZE_LO;
	  res->dev = m.RES_DEV;
	  res->uid= m.RES_UID;
	  res->gid= m.RES_GID;
	  break;
  case EENTERMOUNT:
	  res->inode_nr = m.RES_INODE_NR;
	  res->char_processed = m.RES_OFFSET;
	  res->symloop = m.RES_SYMLOOP;
	  break;
  case ELEAVEMOUNT:
	  res->char_processed = m.RES_OFFSET;
	  res->symloop = m.RES_SYMLOOP;
	  break;
  case ESYMLINK:
	  res->char_processed = m.RES_OFFSET;
	  res->symloop = m.RES_SYMLOOP;
	  break;
  default:
	  break;
  }

  return(r);
}


/*===========================================================================*
 *				req_mkdir	      			     *
 *===========================================================================*/
PUBLIC int req_mkdir(
  endpoint_t fs_e,
  ino_t inode_nr,
  char *lastc,
  uid_t uid,
  gid_t gid,
  mode_t dmode
)
{
  int r;
  cp_grant_id_t grant_id;
  size_t len;
  message m;

  len = strlen(lastc) + 1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes)lastc, len, CPF_READ);
  if(grant_id == -1)
	  panic("req_mkdir: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_MKDIR;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_MODE = dmode;
  m.REQ_UID = uid;
  m.REQ_GID = gid;
  m.REQ_GRANT = grant_id;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_mknod	      			     *
 *===========================================================================*/
PUBLIC int req_mknod(
  endpoint_t fs_e,
  ino_t inode_nr,
  char *lastc,
  uid_t uid,
  gid_t gid,
  mode_t dmode,
  dev_t dev
)
{
  int r;
  size_t len;
  cp_grant_id_t grant_id;
  message m;

  len = strlen(lastc) + 1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes)lastc, len, CPF_READ);
  if(grant_id == -1)
	  panic("req_mknod: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_MKNOD;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_MODE = dmode;
  m.REQ_DEV = dev;
  m.REQ_UID = uid;
  m.REQ_GID = gid;
  m.REQ_GRANT = grant_id;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_mountpoint	                 	     *
 *===========================================================================*/
PUBLIC int req_mountpoint(endpoint_t fs_e, ino_t inode_nr)
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_MOUNTPOINT;
  m.REQ_INODE_NR = inode_nr;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_newnode	      			     *
 *===========================================================================*/
PUBLIC int req_newnode(
  endpoint_t fs_e,
  uid_t uid,
  gid_t gid,
  mode_t dmode,
  dev_t dev,
  struct node_details *res
)
{
  int r;
  message m;

  /* Fill in request message */
  m.m_type = REQ_NEWNODE;
  m.REQ_MODE = dmode;
  m.REQ_DEV = dev;
  m.REQ_UID = uid;
  m.REQ_GID = gid;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);

  res->fs_e	= m.m_source;
  res->inode_nr = m.RES_INODE_NR;
  res->fmode	= m.RES_MODE;
  res->fsize	= m.RES_FILE_SIZE_LO;
  res->dev	= m.RES_DEV;
  res->uid	= m.RES_UID;
  res->gid	= m.RES_GID;

  return(r);
}


/*===========================================================================*
 *				req_newdriver          			     *
 *===========================================================================*/
PUBLIC int req_newdriver(
  endpoint_t fs_e,
  dev_t dev,
  char *label
)
{
  cp_grant_id_t grant_id;
  size_t len;
  message m;
  int r;

  /* Grant access to label */
  len = strlen(label) + 1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes) label, len, CPF_READ);
  if (grant_id == -1)
	panic("req_newdriver: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_NEW_DRIVER;
  m.REQ_DEV = dev;
  m.REQ_GRANT = grant_id;
  m.REQ_PATH_LEN = len;

  /* Issue request */
  r = fs_sendrec(fs_e, &m);

  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_putnode				     *
 *===========================================================================*/
PUBLIC int req_putnode(fs_e, inode_nr, count)
int fs_e;
ino_t inode_nr;
int count;
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_PUTNODE;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_COUNT = count;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_rdlink	     			     *
 *===========================================================================*/
PUBLIC int req_rdlink(fs_e, inode_nr, proc_e, buf, len, direct)
endpoint_t fs_e;
ino_t inode_nr;
endpoint_t proc_e;
char *buf;
size_t len;
int direct; /* set to 1 to use direct grants instead of magic grants */
{
  message m;
  int r;
  cp_grant_id_t grant_id;

  if (direct) {
	grant_id = cpf_grant_direct(fs_e, (vir_bytes) buf, len, CPF_WRITE);
  } else {
	grant_id = cpf_grant_magic(fs_e, proc_e, (vir_bytes) buf, len,
								CPF_WRITE);
  }
  if(grant_id == -1)
	  panic("req_rdlink: cpf_grant_magic failed");

  /* Fill in request message */
  m.m_type = REQ_RDLINK;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_GRANT = grant_id;
  m.REQ_MEM_SIZE = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  if(r == OK) r = m.RES_NBYTES;

  return(r);
}


/*===========================================================================*
 *				req_readsuper	                  	     *
 *===========================================================================*/
PUBLIC int req_readsuper(
  endpoint_t fs_e,
  char *label,
  dev_t dev,
  int readonly,
  int isroot,
  struct node_details *res_nodep,
  int *con_reqs
)
{
  int r;
  cp_grant_id_t grant_id;
  size_t len;
  message m;

  len = strlen(label)+1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes) label, len, CPF_READ);
  if (grant_id == -1)
	  panic("req_readsuper: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_READSUPER;
  m.REQ_FLAGS = 0;
  if(readonly) m.REQ_FLAGS |= REQ_RDONLY;
  if(isroot)   m.REQ_FLAGS |= REQ_ISROOT;
  m.REQ_GRANT = grant_id;
  m.REQ_DEV = dev;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  if(r == OK) {
	/* Fill in response structure */
	res_nodep->fs_e = m.m_source;
	res_nodep->inode_nr = m.RES_INODE_NR;
	res_nodep->fmode = m.RES_MODE;
	res_nodep->fsize = m.RES_FILE_SIZE_LO;
	res_nodep->uid = m.RES_UID;
	res_nodep->gid = m.RES_GID;
	*con_reqs = m.RES_CONREQS;
  }

  return(r);
}


/*===========================================================================*
 *				req_readwrite				     *
 *===========================================================================*/
PUBLIC int req_readwrite(fs_e, inode_nr, pos, rw_flag, user_e,
	user_addr, num_of_bytes, new_posp, cum_iop)
endpoint_t fs_e;
ino_t inode_nr;
u64_t pos;
int rw_flag;
endpoint_t user_e;
char *user_addr;
unsigned int num_of_bytes;
u64_t *new_posp;
unsigned int *cum_iop;
{
  int r;
  cp_grant_id_t grant_id;
  message m;

  if (ex64hi(pos) != 0)
	  panic("req_readwrite: pos too large");

  grant_id = cpf_grant_magic(fs_e, user_e, (vir_bytes) user_addr, num_of_bytes,
			     (rw_flag==READING ? CPF_WRITE:CPF_READ));
  if (grant_id == -1)
	  panic("req_readwrite: cpf_grant_magic failed");

  /* Fill in request message */
  m.m_type = rw_flag == READING ? REQ_READ : REQ_WRITE;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_GRANT = grant_id;
  m.REQ_SEEK_POS_LO = ex64lo(pos);
  m.REQ_SEEK_POS_HI = 0;	/* Not used for now, so clear it. */
  m.REQ_NBYTES = num_of_bytes;
  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  if (r == OK) {
	/* Fill in response structure */
	*new_posp = cvul64(m.RES_SEEK_POS_LO);
	*cum_iop = m.RES_NBYTES;
  }

  return(r);
}


/*===========================================================================*
 *				req_rename	     			     *
 *===========================================================================*/
PUBLIC int req_rename(fs_e, old_dir, old_name, new_dir, new_name)
endpoint_t fs_e;
ino_t old_dir;
char *old_name;
ino_t new_dir;
char *new_name;
{
  int r;
  cp_grant_id_t gid_old, gid_new;
  size_t len_old, len_new;
  message m;

  len_old = strlen(old_name) + 1;
  gid_old = cpf_grant_direct(fs_e, (vir_bytes) old_name, len_old, CPF_READ);
  if(gid_old == -1)
	  panic("req_rename: cpf_grant_direct failed");

  len_new = strlen(new_name) + 1;
  gid_new = cpf_grant_direct(fs_e, (vir_bytes) new_name, len_new, CPF_READ);
  if(gid_new == -1)
	  panic("req_rename: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_RENAME;
  m.REQ_REN_OLD_DIR = old_dir;
  m.REQ_REN_NEW_DIR = new_dir;
  m.REQ_REN_GRANT_OLD = gid_old;
  m.REQ_REN_LEN_OLD = len_old;
  m.REQ_REN_GRANT_NEW = gid_new;
  m.REQ_REN_LEN_NEW = len_new;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(gid_old);
  cpf_revoke(gid_new);

  return(r);
}


/*===========================================================================*
 *				req_rmdir	      			     *
 *===========================================================================*/
PUBLIC int req_rmdir(fs_e, inode_nr, lastc)
endpoint_t fs_e;
ino_t inode_nr;
char *lastc;
{
  int r;
  cp_grant_id_t grant_id;
  size_t len;
  message m;

  len = strlen(lastc) + 1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes) lastc, len, CPF_READ);
  if(grant_id == -1)
	  panic("req_rmdir: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_RMDIR;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_GRANT = grant_id;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_slink	      			     *
 *===========================================================================*/
PUBLIC int req_slink(
  endpoint_t fs_e,
  ino_t inode_nr,
  char *lastc,
  endpoint_t proc_e,
  char *path_addr,
  unsigned short path_length,
  uid_t uid,
  gid_t gid
)
{
  int r;
  size_t len;
  cp_grant_id_t gid_name, gid_buf;
  message m;

  len = strlen(lastc) + 1;
  gid_name = cpf_grant_direct(fs_e, (vir_bytes) lastc, len, CPF_READ);
  if(gid_name == -1)
	  panic("req_slink: cpf_grant_direct failed");

  gid_buf = cpf_grant_magic(fs_e, proc_e, (vir_bytes) path_addr, path_length,
			    CPF_READ);
  if(gid_buf == -1) {
	  cpf_revoke(gid_name);
	  panic("req_slink: cpf_grant_magic failed");
  }

  /* Fill in request message */
  m.m_type = REQ_SLINK;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_UID = uid;
  m.REQ_GID = gid;
  m.REQ_GRANT = gid_name;
  m.REQ_PATH_LEN = len;
  m.REQ_GRANT3 = gid_buf;
  m.REQ_MEM_SIZE = path_length;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(gid_name);
  cpf_revoke(gid_buf);

  return(r);
}


/*===========================================================================*
 *				req_stat	       			     *
 *===========================================================================*/
PUBLIC int req_stat(fs_e, inode_nr, proc_e, buf, pos, stat_version)
int fs_e;
ino_t inode_nr;
int proc_e;
char *buf;
int pos;
int stat_version;
{
  cp_grant_id_t grant_id;
  int r;
  message m;
  struct stat sb;
  struct minix_prev_stat old_sb; /* for backward compatibility */

  if (pos != 0 || stat_version != 0)
	  grant_id = cpf_grant_direct(fs_e, (vir_bytes) &sb,
				      sizeof(struct stat), CPF_WRITE);
  else
	  grant_id = cpf_grant_magic(fs_e, proc_e, (vir_bytes) buf,
				sizeof(struct stat), CPF_WRITE);

  if (grant_id < 0)
	panic("req_stat: cpf_grant_* failed");

  /* Fill in request message */
  m.m_type = REQ_STAT;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_GRANT = grant_id;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  if (r != OK || (pos == 0 && stat_version == 0))
	return(r);

  if (pos != 0)
	sb.st_size -= pos;
  if (stat_version == 0) {
	r = sys_vircopy(SELF, D, (vir_bytes) &sb, proc_e, D, (vir_bytes) buf,
			sizeof(struct stat));
	return(r);
  }

  /* User needs old struct stat.
   * Just 1 prev version at this moment */
  assert(stat_version == 1);

/* XXX until that st_Xtime macroses used, we have to undefine them,
 * because of minix_prev_stat
 */
#undef st_atime
#undef st_ctime
#undef st_mtime

/* Copy field by field because of st_gid type mismath and
 * difference in order after atime.
 */
  old_sb.st_dev = sb.st_dev;
  old_sb.st_ino = sb.st_ino;
  old_sb.st_mode = sb.st_mode;
  old_sb.st_nlink = sb.st_nlink;
  old_sb.st_uid = sb.st_uid;
  old_sb.st_gid = sb.st_gid;
  old_sb.st_rdev = sb.st_rdev;
  old_sb.st_size = sb.st_size;
#if defined(_NETBSD_SOURCE)
  old_sb.st_atime = sb.st_atimespec.tv_sec;
  old_sb.st_mtime = sb.st_mtimespec.tv_sec;
  old_sb.st_ctime = sb.st_ctimespec.tv_sec;
#else
  old_sb.st_atime = sb.st_atime;
  old_sb.st_mtime = sb.st_mtime;
  old_sb.st_ctime = sb.st_ctime;
#endif

  r = sys_vircopy(SELF, D, (vir_bytes) &old_sb, proc_e, D, (vir_bytes) buf,
		  sizeof(struct minix_prev_stat));

  return(r);
}


/*===========================================================================*
 *				req_sync	       			     *
 *===========================================================================*/
PUBLIC int req_sync(fs_e)
endpoint_t fs_e;
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_SYNC;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_unlink	     			     *
 *===========================================================================*/
PUBLIC int req_unlink(fs_e, inode_nr, lastc)
endpoint_t fs_e;
ino_t inode_nr;
char *lastc;
{
  cp_grant_id_t grant_id;
  size_t len;
  int r;
  message m;

  len = strlen(lastc) + 1;
  grant_id = cpf_grant_direct(fs_e, (vir_bytes) lastc, len, CPF_READ);
  if(grant_id == -1)
	  panic("req_unlink: cpf_grant_direct failed");

  /* Fill in request message */
  m.m_type = REQ_UNLINK;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_GRANT = grant_id;
  m.REQ_PATH_LEN = len;

  /* Send/rec request */
  r = fs_sendrec(fs_e, &m);
  cpf_revoke(grant_id);

  return(r);
}


/*===========================================================================*
 *				req_unmount	    			     *
 *===========================================================================*/
PUBLIC int req_unmount(fs_e)
endpoint_t fs_e;
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_UNMOUNT;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}


/*===========================================================================*
 *				req_utime	      			     *
 *===========================================================================*/
PUBLIC int req_utime(fs_e, inode_nr, actime, modtime)
endpoint_t fs_e;
ino_t inode_nr;
time_t actime;
time_t modtime;
{
  message m;

  /* Fill in request message */
  m.m_type = REQ_UTIME;
  m.REQ_INODE_NR = inode_nr;
  m.REQ_ACTIME = actime;
  m.REQ_MODTIME = modtime;

  /* Send/rec request */
  return fs_sendrec(fs_e, &m);
}
