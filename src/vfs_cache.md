# dentry
## d_alloc会把新创建的dentry加到父目录的d_subdirs链表中。
```c
struct dentry *d_alloc(struct dentry * parent, const struct qstr *name)
if (parent)
	list_add(&dentry->d_u.d_child, &parent->d_subdirs);
```
## Vfs dentry项删除会iput关联的inode
```c
static struct dentry *d_kill(struct dentry *dentry)
	__releases(dentry->d_lock)
	__releases(dcache_lock)
{
	struct dentry *parent;

	list_del(&dentry->d_u.d_child);
	dentry_stat.nr_dentry--;	/* For d_free, below */
	/*drops the locks, at that point nobody can reach this dentry */
	dentry_iput(dentry);
	if (IS_ROOT(dentry))
		parent = NULL;
	else
		parent = dentry->d_parent;
	d_free(dentry);
	return parent;
}
static void dentry_iput(struct dentry * dentry)
	__releases(dentry->d_lock)
	__releases(dcache_lock)
{
	struct inode *inode = dentry->d_inode;
	if (inode) {
		dentry->d_inode = NULL;
		list_del_init(&dentry->d_alias);
		spin_unlock(&dentry->d_lock);
		spin_unlock(&dcache_lock);
		if (!inode->i_nlink)
			fsnotify_inoderemove(inode);
		if (dentry->d_op && dentry->d_op->d_iput)
			dentry->d_op->d_iput(dentry, inode);
		else
			iput(inode);
	} else {
		spin_unlock(&dentry->d_lock);
		spin_unlock(&dcache_lock);
	}
}
```
## DCACHE_REFERENCED有这个标志位说明dentry在LRU链表里了
