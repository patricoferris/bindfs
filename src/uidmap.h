#ifndef INC_BINDFS_UIDMAP_H
#define INC_BINDFS_UIDMAP_H

#include <config.h>

#include <stdio.h>
#include <unistd.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

char *get_user(uid_t uid);

#endif