#define FUSE_USE_VERSION 26


#include "cppwrapper.hpp"
#include <fuse.h>
#include <stdio.h>

struct fuse_operations fusionfs_op;

int main(int argc, char *argv[]) {
    int i, fuse_stat;
    fusionfs_op.create = cppwrap_create;
    fusionfs_op.getattr = cppwrap_getattr;
    fusionfs_op.readlink = cppwrap_readlink;
    fusionfs_op.getdir = NULL;
    fusionfs_op.mknod = cppwrap_mknod;
    fusionfs_op.mkdir = cppwrap_mkdir;
    fusionfs_op.unlink = cppwrap_unlink;
    fusionfs_op.rmdir = cppwrap_rmdir;
    fusionfs_op.symlink = cppwrap_symlink;
    fusionfs_op.rename = cppwrap_rename;
    fusionfs_op.link = cppwrap_link;
    fusionfs_op.chmod = cppwrap_chmod;
    fusionfs_op.chown = cppwrap_chown;
    fusionfs_op.truncate = cppwrap_truncate;
    fusionfs_op.utime = cppwrap_utime;
    fusionfs_op.open = cppwrap_open;
    fusionfs_op.read = cppwrap_read;
    fusionfs_op.write = cppwrap_write;
    fusionfs_op.statfs = cppwrap_statfs;
    fusionfs_op.flush = cppwrap_flush;
    fusionfs_op.release = cppwrap_release;
    fusionfs_op.fsync = cppwrap_fsync;
    fusionfs_op.setxattr = cppwrap_setxattr;
    fusionfs_op.getxattr = cppwrap_getxattr;
    fusionfs_op.listxattr = cppwrap_listxattr;
    fusionfs_op.removexattr = cppwrap_removexattr;
    fusionfs_op.opendir = cppwrap_opendir;
    fusionfs_op.readdir = cppwrap_readdir;
    fusionfs_op.releasedir = cppwrap_releasedir;
    fusionfs_op.fsyncdir = cppwrap_fsyncdir;
    fusionfs_op.init = cppwrap_init;

    printf("mounting file system...\n");
        
    for(i = 1; i < argc && (argv[i][0] == '-'); i++) {
        if(i == argc) {
            return (-1);
        }
    }

    //realpath(...) returns the canonicalized absolute pathname
    set_rootdir(realpath(argv[i], NULL));

    for(; i < argc; i++) {
        argv[i] = argv[i+1];
    }
    argc--;

    fuse_stat = fuse_main(argc, argv, &fusionfs_op, NULL);

    printf("fuse_main returned %d\n", fuse_stat);

    return fuse_stat;
}
