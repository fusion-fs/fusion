#include "cppwrapper.hpp"
#include "fusionfs.hpp"

void set_Namespace(const char *path) {
    FusionFS::Instance()->setNamespace(path);
}

int cppwrap_create(const char *path, mode_t mode, struct fuse_file_info *fileInfo){
    return FusionFS::Instance()->Create(path, mode, fileInfo);
}
int cppwrap_getattr(const char *path, struct stat *statbuf) {
    return FusionFS::Instance()->Getattr(path, statbuf);
}

int cppwrap_readlink(const char *path, char *link, size_t size) {
    return FusionFS::Instance()->Readlink(path, link, size);
}

int cppwrap_mknod(const char *path, mode_t mode, dev_t dev) {
    return FusionFS::Instance()->Mknod(path, mode, dev);
}
int cppwrap_mkdir(const char *path, mode_t mode) {
    return FusionFS::Instance()->Mkdir(path, mode);
}
int cppwrap_unlink(const char *path) {
    return FusionFS::Instance()->Unlink(path);
}
int cppwrap_rmdir(const char *path) {
    return FusionFS::Instance()->Rmdir(path);
}
int cppwrap_symlink(const char *path, const char *link) {
    return FusionFS::Instance()->Symlink(path, link);
}
int cppwrap_rename(const char *path, const char *newpath) {
    return FusionFS::Instance()->Rename(path, newpath);
}
int cppwrap_link(const char *path, const char *newpath) {
    return FusionFS::Instance()->Link(path, newpath);
}
int cppwrap_chmod(const char *path, mode_t mode) {
    return FusionFS::Instance()->Chmod(path, mode);
}
int cppwrap_chown(const char *path, uid_t uid, gid_t gid) {
    return FusionFS::Instance()->Chown(path, uid, gid);
}
int cppwrap_truncate(const char *path, off_t newSize) {
    return FusionFS::Instance()->Truncate(path, newSize);
}
int cppwrap_utime(const char *path, struct utimbuf *ubuf) {
    return FusionFS::Instance()->Utime(path, ubuf);
}
int cppwrap_open(const char *path, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Open(path, fileInfo);
}
int cppwrap_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Read(path, buf, size, offset, fileInfo);
}
int cppwrap_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Write(path, buf, size, offset, fileInfo);
}
int cppwrap_statfs(const char *path, struct statvfs *statInfo) {
    return FusionFS::Instance()->Statfs(path, statInfo);
}
int cppwrap_flush(const char *path, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Flush(path, fileInfo);
}
int cppwrap_release(const char *path, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Release(path, fileInfo);
}
int cppwrap_fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    return FusionFS::Instance()->Fsync(path, datasync, fi);
}
int cppwrap_setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    return FusionFS::Instance()->Setxattr(path, name, value, size, flags);
}
int cppwrap_getxattr(const char *path, const char *name, char *value, size_t size) {
    return FusionFS::Instance()->Getxattr(path, name, value, size);
}
int cppwrap_listxattr(const char *path, char *list, size_t size) {
    return FusionFS::Instance()->Listxattr(path, list, size);
}
int cppwrap_removexattr(const char *path, const char *name) {
    return FusionFS::Instance()->Removexattr(path, name);
}
int cppwrap_opendir(const char *path, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Opendir(path, fileInfo);
}
int cppwrap_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Readdir(path, buf, filler, offset, fileInfo);
}
int cppwrap_releasedir(const char *path, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Releasedir(path, fileInfo);
}
int cppwrap_fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    return FusionFS::Instance()->Fsyncdir(path, datasync, fileInfo);
}
int cppwrap_access(const char *path, int mode)
{
    return FusionFS::Instance()->Access(path, mode);
}
void* cppwrap_init(struct fuse_conn_info *conn) {
    return FusionFS::Instance()->Init(conn);
}
