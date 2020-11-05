
#include "uidmap.h"
#include "userinfo.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <libproc.h>
#include <pwd.h>

/* UID MAPPING
   Find the username with the corresponding uid in order to match the 
   Fuse context uid to the correct /Users/$USER/local folder */

/* Look-up the mapping of uid to username -- I think this is a pretty quick call
   so we don't need to cache <uid, username> mappings, instead just call this everytime */
// pthread_mutex_t passwdstruct_mutex = PTHREAD_MUTEX_INITIALIZER;

int get_user(uid_t uid, char *result)
{

    if (uid == 700)
    {
        // pthread_mutex_lock(&passwdstruct_mutex);
        // struct passwd pass;
        // struct passwd *passp;
        // char buf[1024];
        // printf("GETTING USER ID %i\n", uid);
        // Re-entrant version of getpwuid -- CALLING THIS LOCKS EVERYTHING UP :(
        // int status = getpwuid_r(uid, &pass, buf, sizeof buf, &passp);
        strcpy(result, "test");
        // pthread_mutex_unlock(&passwdstruct_mutex);
        return uid_ok;
    }
    else if (uid == 701)
    {
        strcpy(result, "bob");
        // pthread_mutex_unlock(&passwdstruct_mutex);
        return uid_ok;
    }
    else if (uid == 0)
    {
        // For default user and root
        return uid_root;
    }
    else
    {
        return uid_other;
    }

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

// PID Mapping ... maybe
int get_pid_cwd(pid_t pid)
{
    int ret;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    struct proc_vnodepathinfo vpi;

    ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if (ret <= 0)
    {
        fprintf(stderr, "PID %d: proc_pidpath ();\n", pid);
        fprintf(stderr, "    %s\n", strerror(errno));
        return 1;
    }
    printf("proc %d executable: %s\n", pid, pathbuf);
    ret = proc_pidinfo(pid, PROC_PIDVNODEPATHINFO, 0, &vpi,
                       sizeof(vpi));
    if (ret <= 0)
    {
        fprintf(stderr, "PID %d: proc_pidinfo ();\n", pid);
        fprintf(stderr, "    %s\n", strerror(errno));
        return 1;
    }
    printf("proc %d cwd: %s\n", pid, vpi.pvi_cdir.vip_path);
    // printf("proc %d root: %s\n", pid, vpi.pvi_rdir.vip_path);

    return 0;
}

int get_pid_info(pid_t pid) {
    struct proc_bsdinfo proc_inf;

    int status = proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc_inf, PROC_PIDTBSDINFO_SIZE);

    if (status != PROC_PIDTBSDINFO_SIZE)
    {
        fprintf(stderr, "Cannot get process info");
        return 1;
    }

    printf("PID: %i\n,PPID: %i\nCalling for cwd of pid\n", pid, proc_inf.pbi_ppid);
    get_pid_cwd(pid);
    printf("Calling for PPID\n");
    return get_pid_cwd(proc_inf.pbi_ppid);
}