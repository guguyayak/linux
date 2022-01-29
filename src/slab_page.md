# define PAGE_MASK      (~(PAGE_SIZE-1))
# 内核slab使用
```c
static struct kmem_cache *file_slab;

void
nfsd4_free_slabs(void)
{
	...
	kmem_cache_destroy(file_slab);
	...
}

int
nfsd4_init_slabs(void)
{
	...
	file_slab = kmem_cache_create("nfsd4_files",
			sizeof(struct nfs4_file), 0, 0, NULL);
	if (file_slab == NULL)
		goto out_free_lockowner_slab;
	...
}

static struct nfs4_file *nfsd4_alloc_file(void)
{
	return kmem_cache_alloc(file_slab, GFP_KERNEL);
}

static void nfsd4_free_file_rcu(struct rcu_head *rcu)
{
	struct nfs4_file *fp = container_of(rcu, struct nfs4_file, fi_rcu);

	kmem_cache_free(file_slab, fp);
}
```

# [unable to find new object create with kmem_cache_create() in /proc/slabinfo](https://stackoverflow.com/questions/24858424/unable-to-find-new-object-create-with-kmem-cache-create-in-proc-slabinfo/24872230)
