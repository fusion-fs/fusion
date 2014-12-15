#include "fusionfs.hpp"

FusionFS* FusionFS::_instance = NULL;

#define RETURN_ERRNO(x) (x) == 0 ? 0 : -errno

FusionFS* FusionFS::Instance() {
    if(_instance == NULL) {
	_instance = new FusionFS();
    }
    return _instance;
}

FusionFS::FusionFS() {

}

FusionFS::~FusionFS() {

}

void FusionFS::GetNewInode(int& inode)
{
	std::string cmd = "INCR ";
	cmd += _root;
	cmd += ":INODE";
	reply r = _conn->run(command(cmd));
	inode = (int)r.integer();
}

void FusionFS::Path2Inode(const char path[PATH_MAX], int& inode)
{
	std::string p = _root;
	p += ":INODE:";
	p += path;
    reply r = _conn->run(command("GET") << p);
    inode = (int)r.integer();
}

void FusionFS::SetInode(const char path[PATH_MAX], int inode)
{
	std::string p = _root;
	p += ":INODE:";
	p += path;
    _conn->run(command("SET") << p << inode);
}

void FusionFS::setRootDir(const char *path) {
    printf("setting FS root to: %s\n", path);
    _root = path;
}

int FusionFS::Getattr(const char *path, struct stat *statbuf) {
    printf("getattr(%s)\n", path);
    return 0;
}

int FusionFS::Readlink(const char *path, char *link, size_t size) {
    printf("readlink(path=%s, link=%s, size=%d)\n", path, link, (int)size);
    return 0;
}

int FusionFS::Mknod(const char *path, mode_t mode, dev_t dev) {
    printf("mknod(path=%s, mode=%d)\n", path, mode);
    //handles creating FIFOs, regular files, etc...
    return 0;
}

int FusionFS::Mkdir(const char *path, mode_t mode) {
    printf("**mkdir(path=%s, mode=%d)\n", path, (int)mode);
    return 0;
}

int FusionFS::Unlink(const char *path) {
    printf("unlink(path=%s\n)", path);
    return 0;
}

int FusionFS::Rmdir(const char *path) {
    printf("rmkdir(path=%s\n)", path);
    return 0;
}

int FusionFS::Symlink(const char *path, const char *link) {
    printf("symlink(path=%s, link=%s)\n", path, link);
    return 0;
}

int FusionFS::Rename(const char *path, const char *newpath) {
    printf("rename(path=%s, newPath=%s)\n", path, newpath);
    return 0;
}

int FusionFS::Link(const char *path, const char *newpath) {
    printf("link(path=%s, newPath=%s)\n", path, newpath);
    return 0;
}

int FusionFS::Chmod(const char *path, mode_t mode) {
    printf("chmod(path=%s, mode=%d)\n", path, mode);
    return 0;
}

int FusionFS::Chown(const char *path, uid_t uid, gid_t gid) {
    printf("chown(path=%s, uid=%d, gid=%d)\n", path, (int)uid, (int)gid);
    return 0;
}

int FusionFS::Truncate(const char *path, off_t newSize) {
    printf("truncate(path=%s, newSize=%d\n", path, (int)newSize);
    return 0;
}

int FusionFS::Utime(const char *path, struct utimbuf *ubuf) {
    printf("utime(path=%s)\n", path);
    return 0;
}

int FusionFS::Open(const char *path, struct fuse_file_info *fileInfo) {
    printf("open(path=%s)\n", path);
    return 0;
}

int FusionFS::Read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    printf("read(path=%s, size=%d, offset=%d)\n", path, (int)size, (int)offset);
    return 0;

}

int FusionFS::Write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    printf("write(path=%s, size=%d, offset=%d)\n", path, (int)size, (int)offset);
    return 0;
}

int FusionFS::Statfs(const char *path, struct statvfs *statInfo) {
    printf("statfs(path=%s)\n", path);
    return 0;
}

int FusionFS::Flush(const char *path, struct fuse_file_info *fileInfo) {
    printf("flush(path=%s)\n", path);
    return 0;
}

int FusionFS::Release(const char *path, struct fuse_file_info *fileInfo) {
    printf("release(path=%s)\n", path);
    return 0;
}

int FusionFS::Fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    printf("fsync(path=%s, datasync=%d\n", path, datasync);
    return 0;
}

int FusionFS::Setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    printf("setxattr(path=%s, name=%s, value=%s, size=%d, flags=%d\n",
	   path, name, value, (int)size, flags);
    return 0;
}

int FusionFS::Getxattr(const char *path, const char *name, char *value, size_t size) {
    printf("getxattr(path=%s, name=%s, size=%d\n", path, name, (int)size);
    return 0;
}

int FusionFS::Listxattr(const char *path, char *list, size_t size) {
    printf("listxattr(path=%s, size=%d)\n", path, (int)size);
    return 0;
}

int FusionFS::Removexattr(const char *path, const char *name) {
    printf("removexattry(path=%s, name=%s)\n", path, name);
    return 0;
}

int FusionFS::Opendir(const char *path, struct fuse_file_info *fileInfo) {
    printf("opendir(path=%s)\n", path);
    return 0;
}

int FusionFS::Readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    printf("readdir(path=%s, offset=%d)\n", path, (int)offset);
    return 0;
}

int FusionFS::Releasedir(const char *path, struct fuse_file_info *fileInfo) {
    printf("releasedir(path=%s)\n", path);
    return 0;
}

int FusionFS::Fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}

int FusionFS::Init(struct fuse_conn_info *conn) {
    _conn = connection::create();
}

int FusionFS::Truncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    printf("truncate(path=%s, offset=%d)\n", path, (int)offset);
    return 0;
}

