
#include "uidmap.h"
#include "userinfo.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <pwd.h>

/* UID MAPPING
   Find the username with the corresponding uid in order to match the 
   Fuse context uid to the correct /Users/$USER/local folder */

/* Look-up the mapping of uid to username -- I think this is a pretty quick call
   so we don't need to cache <uid, username> mappings, instead just call this everytime */
pthread_mutex_t passwdstruct_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_user(uid_t uid, char *result)
{

    // if (uid == 700)
    // {
    //     pthread_mutex_lock(&passwdstruct_mutex);
    //     struct passwd pass;
    //     struct passwd *passp;
    //     char buf[1024];
    //     printf("GETTING USER ID %i\n", uid);
    //     // Re-entrant version of getpwuid -- CALLING THIS LOCKS EVERYTHING UP :(
    //     int status = getpwuid_r(uid, &pass, buf, sizeof buf, &passp);
    //     strcpy(result, "test");
    //     pthread_mutex_unlock(&passwdstruct_mutex);
    //     return uid_ok;
    // }
    // else
    // {
    //     strcpy(result, "patrickferris");
    //     return uid_ok;
    // }

    struct passwd pass;
    struct passwd *passp;
    char buf[1024];
    printf("GETTING USER ID %i\n", uid);
    // Re-entrant version of getpwuid  -- CALLING THIS LOCKS EVERYTHING UP :(
    int status = getpwuid_r(uid, &pass, buf, sizeof buf, &passp);
    printf("SUCCESSFULLY GOT PASSED TRYING TO GET USER ID\n");

    if (status != 0)
    {
        fprintf(stderr, "getpwuid_r failed with: %i\n", status);
        return uid_error;
    }

    if (passp == NULL)
    {
        printf("NULL PASSP FOR: %i\n", uid);
        return uid_not_found;
    }

    strcpy(result, pass.pw_name);
    return uid_ok;
}