#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static void *fusionfs_init()
{
}
static void fusionfs_destroy(void *unused __attribute__ ((unused)))
{
}
static int fusionfs_fsync(const char *path, int isdatasync,
                        struct fuse_file_info *fi)
{
    return 0;
}
static int fusionfs_release(const char *path, struct fuse_file_info *fi)
{
    return 0;
}
static int fusionfs_getattr(const char *path, struct stat *stbuf)
{
    return 0;
}

static int fusionfs_access(const char *path, int mask)
{
    return 0;
}

static int fusionfs_readlink(const char *path, char *buf, size_t size)
{
    return 0;
}
static int fusionfs_readdir(const char *path, void *buf,
                            fuse_fill_dir_t filler, off_t offset,
                            struct fuse_file_info *fi)
{
    return 0;
}
static int fusionfs_mknod(const char *path, mode_t mode, dev_t rdev)
{
    return 0;
}
static int fusionfs_mkdir(const char *path, mode_t mode)
{
    return 0;
}
static int fusionfs_unlink(const char *path)
{
    return 0;
}
static int fusionfs_rmdir(const char *path)
{
    return 0;
}
static int fusionfs_symlink(const char *from, const char *to)
{
    return 0;
}
static int fusionfs_rename(const char *from, const char *to)
{
    return 0;
}
static int fusionfs_link(const char *from, const char *to)
{
    return 0;
}
static int fusionfs_chmod(const char *path, mode_t mode)
{
    return 0;
}
static int fusionfs_chown(const char *path, uid_t uid, gid_t gid)
{
    return 0;
}
static int fusionfs_truncate(const char *path, off_t size)
{
    return 0;
}
static int fusionfs_utimens(const char *path, const struct timespec ts[2])
{
    return 0;
}
static int fusionfs_open(const char *path, struct fuse_file_info *fi)
{
    return 0;
}
static int fusionfs_read(const char *path, char *buf, size_t size,
                         off_t offset, struct fuse_file_info *fi)
{
    return 0;
}
static int fusionfs_write(const char *path, const char *buf, size_t size,
                          off_t offset, struct fuse_file_info *fi)
{
    return 0;
}
static int fusionfs_statfs(const char *path, struct statvfs *stbuf)
{
    return 0;
}

static struct fuse_operations fusionfs_op = {
    .getattr = fusionfs_getattr,
    .access = fusionfs_access,
    .readlink = fusionfs_readlink,
    .readdir = fusionfs_readdir,
    .mknod = fusionfs_mknod,
    .mkdir = fusionfs_mkdir,
    .symlink = fusionfs_symlink,
    .unlink = fusionfs_unlink,
    .rmdir = fusionfs_rmdir,
    .rename = fusionfs_rename,
    .link = fusionfs_link,
    .chmod = fusionfs_chmod,
    .chown = fusionfs_chown,
    .truncate = fusionfs_truncate,
    .utimens = fusionfs_utimens,
    .open = fusionfs_open,
    .read = fusionfs_read,
    .write = fusionfs_write,
    .statfs = fusionfs_statfs,
    .release = fusionfs_release,
    .fsync = fusionfs_fsync,
    .destroy = fusionfs_destroy,
    .init = fusionfs_init,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &fusionfs_op, NULL);
}
