#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define USR_KEY_LEN 64

void usage_exit(char* progname) {
    fprintf(stderr, "Usage: %s [mode] [key] [filename]\n", progname);
    exit (EXIT_FAILURE);
}

int main(int argc, char** argv) {
    int error; /* Used to check the return value of function calls */

    if (argc != 4) {
        usage_exit(argv[0]);
    }
    
    /* The following if statement is to prevent unexpected behavior if
     * the first argument is not a mode character.
     */
    if (argv[1][1] != '\0') {
        usage_exit(argv[0]);
    }
    /* Get the mode */
    char mode = argv[1][0];
    if (mode != 'e' && mode != 'd') {
        usage_exit(argv[0]);
    }
    
    /* Get the bottom half of the key. */
    if (strlen(argv[2]) > USR_KEY_LEN) {
        usage_exit(argv[0]);
    }
    char key_bottom[USR_KEY_LEN + 1];
    strncpy(key_bottom, argv[2], USR_KEY_LEN);
    key_bottom[64] = '/0';
    
    /* Get the filename */
    char filename[strlen(argv[3]) + 1];
    strncpy(filename, argv[3], strlen(argv[3]) + 1);
    
    /* Get file info, used for the inode number and the sticky bit
     * status.
     */
    struct stat file_info;
    error = stat(filename, &file_info);
    if (error != 0) {
        usage_exit(argv[0]);
    }
    int file_nr = file_info.st_ino;
    int mode = file_info.st_mode;
    int is_sticky = mode & S_ISVTX;
    
    /*** CODE UNFINISHED ***/
