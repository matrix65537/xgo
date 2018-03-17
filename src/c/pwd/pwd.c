#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
#include "util.h"

int main(int argc, char* argv[])
{
    struct passwd *pwd;
    struct spwd *spw;
    if(argc < 2)
    {
        printf("Usage: %s username\n", argv[0]);
        exit(1);
    }

    pwd = getpwnam(argv[1]);
    if(pwd == NULL)
    {
        perror("get user information error");
        exit(1);
    }
    printf("pw_name: %s\n", pwd->pw_name);
    printf("pw_passwd: %s\n", pwd->pw_passwd);
    printf("pw_gecos: %s\n", pwd->pw_gecos);
    printf("pw_dir: %s\n", pwd->pw_dir);
    printf("pw_shell: %s\n", pwd->pw_shell);

    printf("pw_uid: %d\n", pwd->pw_uid);
    printf("pw_gid: %d\n", pwd->pw_gid);

    printf("===================================\n");
    while((pwd = getpwent()) != NULL)
    {
        printf("%20s : %32s : %32s : %6d : %6d\n", pwd->pw_name, pwd->pw_dir, pwd->pw_shell, pwd->pw_uid, pwd->pw_gid);
    }

    printf("shadow===================================\n");
    while((spw = getspent()) != NULL)
    {
        printf("[%s] : [%s]\n", spw->sp_namp, spw->sp_pwdp);
    }
    return 0;
}
