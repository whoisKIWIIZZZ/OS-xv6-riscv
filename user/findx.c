#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char* kmp(const char *s, const char *t)
{
    int n = strlen(s);
    int m = strlen(t);
    if (m == 0) return (char *)s;
    for (int i = 0; i <= n-m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (s[i + j] != t[j])
                break;
        }
        if (j == m) return (char*)(s + i);
    }
    return 0;
}

int match(char *name, char *target) {
    int n = strlen(name);
    int t = strlen(target);

    //*example* 
    if (target[0] == '*' && target[t-1] == '*') {
        char pattern[64];
        if (t-2>= 64) return 0;
        for (int i = 0; i < t - 2; i++) {
            pattern[i] = target[i + 1];
        }
        pattern[t-2] = '\0';
        return kmp(name, pattern) != 0;
    }
    //*example 
    if (target[0] == '*') {
        char *pattern = target + 1;
        int p = strlen(pattern);
        if (n < p) return 0;
        return strcmp(name + n-p, pattern) == 0;
    }
    //example*
    if (target[t-1] == '*') {    
      int i;
      for (i = 0; i < t - 1; i++) {
          if (name[i] != target[i])
              break;
      }
      if (i == t - 1) {
          return 1;
      }
    }
    return strcmp(name, target) == 0;
}

void findx(char *path, char *target) {
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

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if (stat(buf, &st) < 0) {
            continue;
        }

        if (st.type == T_DIR) {
            findx(buf, target);
        } else if (st.type == T_FILE) {
            if (match(de.name, target)) {
                printf("%s\n", buf);
            }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: findx <dir> <pattern>\n");
        exit(1);
    }
    findx(argv[1], argv[2]);
    exit(0);
}