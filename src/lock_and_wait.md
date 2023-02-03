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
typedef struct sigset
{
	int sigset;
} sigset_t;

#define SIGNAL_MASK_LOCK(task, flags)		\
	spin_lock_irqsave(&task->sighand->siglock, flags)

#define SIGNAL_MASK_UNLOCK(task, flags)		\
	spin_unlock_irqrestore(&task->sighand->siglock, flags)

static inline sigset_t sigs_save()
{
	sigset_t oldsigs;
	unsigned long flags = 0;
	SIGNAL_MASK_LOCK(current, flags);
	oldsigs.sigset = current->blocked;
	recalc_sigpending();
	SIGNAL_MASK_UNLOCK(current, flags);
	
	return oldsigs;
}

static inline void sigs_restor(sigset_t sigs)
{
	unsigned long flags = 0;
	
	SIGNAL_MASK_LOCK(current, flags);
	current->blocked = sigs.sigset;
	recalc_sigpending();
	SIGNAL_MASK_UNLOCK(current, flags);
}

static inline int msleep_interruptible_and_clearsig(unsigned long long wait_ms)
{
	int rc = 0;
	sigset_t sigset;
	
	sigset = sigs_save();
	
	sigs_pending_clear();
	
	rc = msleep_interruptible(wait_us);
	if (rc > 0)
	{
		cond_resched();
		rc = -EINTR;
	}
	
	sigs_restor(sigset);
	
	return rc;
}
```
