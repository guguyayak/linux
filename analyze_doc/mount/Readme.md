# 内核vfs 层 mount流程分析
> 网上分析文档：https://www.bilibili.com/read/cv17366256/

```c
struct path {
	struct vfsmount *mnt;
	struct dentry *dentry;
} __randomize_layout;



struct mountpoint *mp, *new = NULL;
hlist_add_head(&new->m_hash, mp_hash(dentry));
static inline struct hlist_head *mp_hash(struct dentry *dentry)
{
	unsigned long tmp = ((unsigned long)dentry / L1_CACHE_BYTES);
	tmp = tmp + (tmp >> mp_hash_shift);
	return &mountpoint_hashtable[tmp & mp_hash_mask];
}


static inline struct hlist_head *m_hash(struct vfsmount *mnt, struct dentry *dentry)
{
	unsigned long tmp = ((unsigned long)mnt / L1_CACHE_BYTES);
	tmp += ((unsigned long)dentry / L1_CACHE_BYTES);
	tmp = tmp + (tmp >> m_hash_shift);
	return &mount_hashtable[tmp & m_hash_mask];
}
```
