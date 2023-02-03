# 读写锁
## 实例1 —— sunrpc cache_detail
```c
struct cache_detail {
	...
	rwlock_t		hash_lock;
	...
}

	//初始化
	rwlock_init(&cd->hash_lock);

	read_lock(&detail->hash_lock);
	...
	read_unlock(&detail->hash_lock);
  
	write_lock(&detail->hash_lock);
	...
	write_unlock(&detail->hash_lock);
  ```
# sleep interruptible and clearsig
```c
static inline int msleep_interruptible_and_clearsig(unsigned long long wait_ms)
{
	int rc = 0;
	
}
```
