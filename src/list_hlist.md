# struct
```c
struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};
```
# hash list
```c
#define FILE_HASH_BITS		7
#define FILE_NRHASH		(1<<FILE_HASH_BITS)
static struct hlist_head	nlm_files[FILE_NRHASH];

struct nlm_file {
	struct hlist_node	f_list;		/* linked list */
	...
	struct list_head	f_blocks;	/* blocked locks */
	...
};
struct nlm_file	*file;
```
- 初始化
```c
INIT_HLIST_NODE(&file->f_list);
```
- 链表遍历举例
```c
hlist_for_each_entry(file, &nlm_files[hash], f_list)
```
# list
- 初始化
```c
INIT_LIST_HEAD(&file->f_blocks);

// 宏LIST_HEAD
// LIST_HEAD_INIT 宏用于给一个 struct list_head 表头初始化，将 struct list_head 的 prev 和 next 成员都指向它自己。

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \

    struct list_head name = LIST_HEAD_INIT(name)
```
# 链表详细操作见内核源码[include/linux/list.h](https://github.com/torvalds/linux/blob/master/include/linux/list.h)文件
