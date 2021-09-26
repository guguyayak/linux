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
## hash_list 使用示例
```c
#define NLM_HOST_NRHASH		32
static struct hlist_head	nlm_server_hosts[NLM_HOST_NRHASH];

struct hlist_head *chain;
chain = &nlm_server_hosts[nlm_hash_address(ni.sap)];
hlist_add_head(&host->h_hash, chain);
struct nlm_host {
	struct hlist_node	h_hash;		/* doubly linked list */
	...
}

unsigned int hash;
return hash & (NLM_HOST_NRHASH - 1);

hlist_del_init(&host->h_hash);

/**
 * hlist_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another &struct hlist_node to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry_safe(pos, n, head, member) 		\
	for (pos = hlist_entry_safe((head)->first, typeof(*pos), member);\
	     pos && ({ n = pos->member.next; 1; });			\
	     pos = hlist_entry_safe(n, typeof(*pos), member))

	struct hlist_node *next;
	struct nlm_host	*host;
	
hlist_for_each_entry_safe(host, next, chain, h_hash)
	//host是要操作的成员，chain是hash链表头；
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
# container_of 及示例
```c
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct delayed_work {
	struct work_struct work;
	struct timer_list timer;

	/* target workqueue and CPU ->timer uses to queue ->work */
	struct workqueue_struct *wq;
	int cpu;
};

static void grace_ender(struct work_struct *grace)
{
	struct delayed_work *dwork = container_of(grace, struct delayed_work,
						  work);
	struct lockd_net *ln = container_of(dwork, struct lockd_net,
					    grace_period_end);

	locks_end_grace(&ln->lockd_manager);
}
```
# 链表详细操作见内核源码[include/linux/list.h](https://github.com/torvalds/linux/blob/master/include/linux/list.h)文件
