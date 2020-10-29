
#include "uidmap.h"
#include "userinfo.h"
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

/* UID MAPPING
   Find the username with the corresponding uid in order to match the 
   Fuse context uid to the correct /Users/$USER/local folder */

/* Look-up the mapping of uid to username -- I think this is a pretty quick call
   so we don't need to cache <uid, username> mappings, instead just call this everytime */
char *get_user(uid_t uid)
{
    struct passwd *p = getpwuid(uid);

    // Null means no user
    if (p == NULL)
    {
        printf("ERROR\n");
        return NULL;
    }

    // Copy the username to the res...
    return p->pw_name;
}