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
# [在结构体最后定义一个成员数为0的数组的意义](https://blog.csdn.net/weixin_42031299/article/details/115842672)
1. 如下示例代码，data[0]成员是不占用内存空间的，即struct resultInfo a; sizeof(a) == sizeof(a.type) + sizeof(a.num);
2. 申请内存时，可以灵活申请内存大小（申请的内存是连续的），如：
```
struct resultInfo *p = (struct resultInfo *)malloc(sizeof(struct resultInfo) + sizeof(int) * 2);
p --->	|++++++++++++++++++++++++++++++++
	| type  |  num  | (int) | (int) |
	|++++++++++++++++++++++++++++++++
```
3. 释放内存时也可以一次释放：free(p);
4. 假如data[0]换成指针 *data。则struct resultInfo a; sizeof(a) == sizeof(a.type) + sizeof(a.num) + sizeof(a.data); sizeof(a.data)是指针大小。并且若想额外申请空间，只能：struct resultInfo *p = (struct resultInfo *)malloc(sizeof(struct resultInfo)); p->data = (int *)malloc(sizeof(int) * 2); 同样，释放空间也需要逆序两次调用free: free(p->data); free(p);
5. 二级指针同理，一般声明 TYPE **p 为 TYPE *p[0]; **p 要多占一个指针的内存，需要单独赋值和释放； 用 *p[0]可以分配连续的内存 方便访问和释放
```c
struct resultInfo
{
	int type;
	int num;
	int data[0];
};
```
