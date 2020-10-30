#ifndef INC_BINDFS_UIDMAP_H
#define INC_BINDFS_UIDMAP_H

#include <config.h>

#include <stdio.h>
#include <unistd.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

// Based on return values outlined here: https://linux.die.net/man/3/getpwuid_r
typedef enum UidStatus
{
  uid_ok = 0,
  uid_not_found = 1,
  uid_error = 2
} UidStatus;

int get_user(uid_t uid, char *result);

#endif