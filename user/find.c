#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    fd = open(path, 0);
    fstat(fd, &st);	
    if(st.type != T_DIR) {
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0) continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        stat(buf, &st);

        if(st.type == T_DIR) {
            find(buf, target);      
        } else if(st.type == T_FILE) {
            if(strcmp(de.name, target) == 0)
                printf("%s\n", buf); 
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(2, "Usage: find <dir> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
