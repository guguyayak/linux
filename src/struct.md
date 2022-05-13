# 结构体联合体嵌套使用
```c
struct nfs_fhbase_new {
	__u8		fb_version;	/* == 1, even => nfs_fhbase_old */
	__u8		fb_auth_type;
	__u8		fb_fsid_type;
	__u8		fb_fileid_type;
	__u32		fb_auth[1];
/*	__u32		fb_fsid[0]; floating */
/*	__u32		fb_fileid[0]; floating */
};

struct knfsd_fh {
	unsigned int	fh_size;	/* significant for NFSv3.
					 * Points to the current size while building
					 * a new file handle
					 */
	union {
		struct nfs_fhbase_old	fh_old;
		__u32			fh_pad[NFS4_FHSIZE/4];
		struct nfs_fhbase_new	fh_new;
	} fh_base;
};

#define	fh_version		fh_base.fh_new.fb_version
#define	fh_fsid_type		fh_base.fh_new.fb_fsid_type
#define	fh_auth_type		fh_base.fh_new.fb_auth_type
#define	fh_fileid_type		fh_base.fh_new.fb_fileid_type
#define	fh_fsid			fh_base.fh_new.fb_auth
```
