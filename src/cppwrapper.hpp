#ifndef cppwrap_hh
#define cppwrap_hh

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/xattr.h>

#ifdef __cplusplus
extern "C" {
#endif

    void set_Namespace(const char *path);

    int cppwrap_create(const char *path, mode_t mode, struct fuse_file_info *fileInfo);
    int cppwrap_getattr(const char *path, struct stat *statbuf);
    int cppwrap_readlink(const char *path, char *link, size_t size);
    int cppwrap_mknod(const char *path, mode_t mode, dev_t dev);
    int cppwrap_mkdir(const char *path, mode_t mode);
    int cppwrap_unlink(const char *path);
    int cppwrap_rmdir(const char *path);
    int cppwrap_symlink(const char *path, const char *link);
    int cppwrap_rename(const char *path, const char *newpath);
    int cppwrap_link(const char *path, const char *newpath);
    int cppwrap_chmod(const char *path, mode_t mode);
    int cppwrap_chown(const char *path, uid_t uid, gid_t gid);
    int cppwrap_truncate(const char *path, off_t newSize);
    int cppwrap_utime(const char *path, struct utimbuf *ubuf);
    int cppwrap_open(const char *path, struct fuse_file_info *fileInfo);
    int cppwrap_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    int cppwrap_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo);
    int cppwrap_statfs(const char *path, struct statvfs *statInfo);
    int cppwrap_flush(const char *path, struct fuse_file_info *fileInfo);
    int cppwrap_release(const char *path, struct fuse_file_info *fileInfo);
    int cppwrap_fsync(const char *path, int datasync, struct fuse_file_info *fi);
    int cppwrap_setxattr(const char *path, const char *name, const char *value, size_t size, int flags);
    int cppwrap_getxattr(const char *path, const char *name, char *value, size_t size);
    int cppwrap_listxattr(const char *path, char *list, size_t size);
    int cppwrap_removexattr(const char *path, const char *name);
    int cppwrap_opendir(const char *path, struct fuse_file_info *fileInfo);
    int cppwrap_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo);
    int cppwrap_releasedir(const char *path, struct fuse_file_info *fileInfo);
    int cppwrap_fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo);
    void* cppwrap_init(struct fuse_conn_info *conn);
    int cppwrap_access(const char *path, int mode);
#ifdef __cplusplus
}
#endif

#endif //cppwrap_hh
