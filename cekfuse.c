#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>

static const char *dirpath = "/home/administrator/Documents";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if(res == -1)
	{
		return -errno;
	}

	return 0;
}

static int xmp_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
	DIR *dp;
	struct dirent *de;
	dp = opendir(fpath);
	if(dp==NULL)
	{
		return -errno;
	}
	while((de = readdir(dp))!=NULL)
	{
		res = filler(h, de->d_name, de->d_type);
		if(res!=0) break;
	}
	closedir(dp);
	return res;
}

static struct fuse_operations xmp_oper =
{
	.getattr = xmp_getattr,
	//.readlink = xmp_readlink,
	.getdir = xmp_getdir,
	//.mknod = xmp_mknod,
	//.mkdir = xmp_mkdir,
	//.symlink = xmp_symlink,
	//.unlink = xmp_unlink,
	//.rmdir = xmp_rmdir,
	//.rename = xmp_rename,
	//.link = xmp_link,
	//.chmod = xmp_chmod,
	//.chown = xmp_chown,
	//.truncate = xmp_truncate,
	//.utime = xmp_utime,
	//.open = xmp_open,
	//.read = xmp_read,
	//.write = xmp_write,
	//.release = xmp_release,
	//.fsync = xmp_fsync,
	//.readdir = hello_readdir
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &xmp_oper);
}
