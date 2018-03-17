#define _XOPEN_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <shadow.h>
#include "util.h"

int main(int argc, char* argv[])
{
    struct passwd *pwd;
    struct spwd *spwd;
    char *passwd;
    char username[0x100];
    int username_len;
    char *password;
    char *encrypted;
    int auth_flag;

    while(1)
    {
        printf("user name:");
        if(fgets(username, sizeof(username), stdin) == NULL)
        {
            exit(1);
        }
        username_len = strlen(username);
        if(username[username_len - 1] == '\n')
        {
            username[username_len - 1] = 0;
        }
        pwd = getpwnam(username);
        if(pwd == NULL)
        {
            printf("get user information error\n");
            continue;
        }
        spwd = getspnam(username);
        if(spwd == NULL && errno == EACCES)
        {
            //printf("no permission to read shadow password file\n");
            perror("read shadow file error");
            continue;
        }
        if(spwd != NULL)
        {
            pwd->pw_passwd = spwd->sp_pwdp;
        }
        //read password
        password = getpass("user password:");
        printf("%s\n", pwd->pw_passwd);
        encrypted = crypt(password, pwd->pw_passwd);
        if(encrypted == NULL)
        {
            printf("encrypted error\n");
            continue;
        }

        auth_flag = strcmp(encrypted, pwd->pw_passwd) == 0;
        if(!auth_flag)
        {
            printf("Incorrect password\n");
        }
        else
        {
            printf("%s\n", encrypted);
            printf("Successfully authecticated. UID = %d\n", pwd->pw_uid);
        }
    }
    return 0;
}
