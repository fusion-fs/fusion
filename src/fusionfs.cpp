#include "fusionfs.hpp"

FusionFS* FusionFS::_instance = NULL;

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

// utility functions
string FusionFS::GetParent(const string& path)
{
    unsigned found = path.find_last_of("/\\");
    return (path.substr(0,found));
}

string FusionFS::GetFile(const string& path)
{
    unsigned found = path.find_last_of("/\\");
    return (path.substr(found + 1));
}

void FusionFS::GetNewInode(int& inode)
{
	string cmd = _root;
	cmd += ":INODE";
	reply r = _conn->run(command("INCR") << cmd);
	inode = (int)r.integer();
}

void FusionFS::Path2Inode(const char* path, int& inode)
{
	string p = _root;
	p += ":INODE:";
	p += path;
    reply r = _conn->run(command("GET") << p);
    inode = (int)r.integer();
}

void FusionFS::SetInode(const char* path, int inode)
{
	string p = _root;
	p += ":INODE:";
	p += path;
    _conn->run(command("SET") << p << inode);
}

void FusionFS::AddEntry(const string& parent, const string& file)
{
    _conn->run(command("SADD") << parent << file);
}

void FusionFS::setRootDir(const char *path) {
    fprintf(stderr,"setting FS root to: %s\n", path);
    _root = path;
}

void FusionFS::SetAttr(const struct stat& statbuf)
{
	_conn->run(command("HMSET") << _root << " ATTR_N:INODE:" + statbuf.st_ino << 
               " GID " << statbuf.st_gid << 
               " UID " << statbuf.st_uid << 
               " LINK " << statbuf.st_nlink << 
               " MODE " << statbuf.st_mode);

    if (statbuf.st_atime && statbuf.st_mtime && statbuf.st_ctime) {
        _conn->run(command("HMSET") << _root << " ATTR_V:INODE:" + statbuf.st_ino <<     
                   " ATIME " << statbuf.st_atime << 
                   " MTIME " << statbuf.st_mtime << 
                   " CTIME " << statbuf.st_ctime);
    }
    if (statbuf.st_mode & S_IFDIR) {
        _conn->run(command("HMSET") << _root << " ATTR_V:INODE:" + statbuf.st_ino <<     
                   " SIZE " << statbuf.st_size);
    }
}
int FusionFS::Getattr(const char *path, struct stat *statbuf) {
    fprintf(stderr,"getattr(%s)\n", path);
    memset(statbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0)
    {
        statbuf->st_mtime = statbuf->st_atime = statbuf->st_ctime = time(NULL);
        statbuf->st_uid = getuid();
        statbuf->st_gid = getgid();

        statbuf->st_mode = S_IFDIR | 0755;
        statbuf->st_nlink = 1;

        return 0;
    }


	int inode;
	Path2Inode(path, inode);
    if (!inode) {
        return -ENOENT;
    }

    statbuf->st_ino = inode;

	// first non-volatile parts
	reply r = _conn->run(command("HMGET") << _root << " ATTR_N:INODE:" + inode << 
                         " GID UID LINK MODE");
	vector<reply> re = r.elements();

	statbuf->st_gid   = re[0].integer();
    statbuf->st_uid   = re[1].integer();
    statbuf->st_nlink = re[2].integer();
    statbuf->st_mode  = re[3].integer();

    // then volatile parts    
    r = _conn->run(command("HMGET") << _root << " ATTR_V:INODE:" + inode << 
               "ATIME CTIME MTIME SIZE");
    statbuf->st_atime  = re[0].integer();
    statbuf->st_ctime  = re[1].integer();
    statbuf->st_mtime  = re[2].integer();
    if (statbuf->st_mode & S_IFDIR) {
        statbuf->st_size  = re[3].integer();
    }
    //FIXME: blocksize, blocks, rdev, dev
    return 0;
}

int FusionFS::Readlink(const char *path, char *link, size_t size) {
    fprintf(stderr,"readlink(path=%s, link=%s, size=%d)\n", path, link, (int)size);
    return 0;
}

int FusionFS::Mknod(const char *path, mode_t mode, dev_t dev) {
    fprintf(stderr,"mknod(path=%s, mode=%d)\n", path, mode);
    //handles creating FIFOs, regular files, etc...
    return 0;
}

int FusionFS::Mkdir(const char *path, mode_t mode) {
    fprintf(stderr,"**mkdir(path=%s, mode=%d)\n", path, (int)mode);
    return 0;
}

int FusionFS::Unlink(const char *path) {
    fprintf(stderr,"unlink(path=%s\n)", path);
    return 0;
}

int FusionFS::Rmdir(const char *path) {
    fprintf(stderr,"rmkdir(path=%s\n)", path);
    return 0;
}

int FusionFS::Symlink(const char *path, const char *link) {
    fprintf(stderr,"symlink(path=%s, link=%s)\n", path, link);
    return 0;
}

int FusionFS::Rename(const char *path, const char *newpath) {
    fprintf(stderr,"rename(path=%s, newPath=%s)\n", path, newpath);
    return 0;
}

int FusionFS::Link(const char *path, const char *newpath) {
    fprintf(stderr,"link(path=%s, newPath=%s)\n", path, newpath);
    return 0;
}

int FusionFS::Chmod(const char *path, mode_t mode) {
    fprintf(stderr,"chmod(path=%s, mode=%d)\n", path, mode);
    return 0;
}

int FusionFS::Chown(const char *path, uid_t uid, gid_t gid) {
    fprintf(stderr,"chown(path=%s, uid=%d, gid=%d)\n", path, (int)uid, (int)gid);
    return 0;
}

int FusionFS::Truncate(const char *path, off_t newSize) {
    fprintf(stderr,"truncate(path=%s, newSize=%d\n", path, (int)newSize);
    return 0;
}

int FusionFS::Truncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"truncate(path=%s, offset=%d)\n", path, (int)offset);
    return 0;
}


int FusionFS::Utime(const char *path, struct utimbuf *ubuf) {
    fprintf(stderr,"utime(path=%s)\n", path);
    return 0;
}

int FusionFS::Open(const char *path, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"open(path=%s)\n", path);
    return 0;
}

int FusionFS::Read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"read(path=%s, size=%d, offset=%d)\n", path, (int)size, (int)offset);
    return 0;

}

int FusionFS::Write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"write(path=%s, size=%d, offset=%d)\n", path, (int)size, (int)offset);
    return 0;
}

int FusionFS::Statfs(const char *path, struct statvfs *statInfo) {
    fprintf(stderr,"statfs(path=%s)\n", path);
    return 0;
}

int FusionFS::Flush(const char *path, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"flush(path=%s)\n", path);
    return 0;
}

int FusionFS::Release(const char *path, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"release(path=%s)\n", path);
    return 0;
}

int FusionFS::Fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    fprintf(stderr,"fsync(path=%s, datasync=%d\n", path, datasync);
    return 0;
}

int FusionFS::Setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    fprintf(stderr,"setxattr(path=%s, name=%s, value=%s, size=%d, flags=%d\n",
           path, name, value, (int)size, flags);
    return 0;
}

int FusionFS::Getxattr(const char *path, const char *name, char *value, size_t size) {
    fprintf(stderr,"getxattr(path=%s, name=%s, size=%d\n", path, name, (int)size);
    return 0;
}

int FusionFS::Listxattr(const char *path, char *list, size_t size) {
    fprintf(stderr,"listxattr(path=%s, size=%d)\n", path, (int)size);
    return 0;
}

int FusionFS::Removexattr(const char *path, const char *name) {
    fprintf(stderr,"removexattry(path=%s, name=%s)\n", path, name);
    return 0;
}

int FusionFS::Opendir(const char *path, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"opendir(path=%s)\n", path);
    return 0;
}

int FusionFS::Readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"readdir(path=%s, offset=%d)\n", path, (int)offset);
    return 0;
}

int FusionFS::Releasedir(const char *path, struct fuse_file_info *fileInfo) {
    fprintf(stderr,"releasedir(path=%s)\n", path);
    return 0;
}

int FusionFS::Fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}

int FusionFS::Create(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    if (!path) {
        return EINVAL;
    }

    int inode;
    GetNewInode(inode);
    SetInode(path, inode);

    // create dentry
    struct stat statbuf;
    memset(&statbuf, 0, sizeof(struct stat));
    statbuf.st_ino = inode;
    statbuf.st_mtime = statbuf.st_ctime = statbuf.st_atime = time(NULL);
    statbuf.st_mode = mode;
    statbuf.st_uid = fuse_get_context()->uid;
    statbuf.st_gid = fuse_get_context()->gid;
    statbuf.st_nlink = 1;
    SetAttr(statbuf);

    // add to parent
    string parent = GetParent(path);
    string file = GetFile(path);
    AddEntry(parent, file);
}

int FusionFS::Access(const char *path, int mode)
{
    return 0;
}

void* FusionFS::Init(struct fuse_conn_info *conn) {
    _conn = connection::create();
    return 0;
}

