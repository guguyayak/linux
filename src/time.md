# 内核表示N秒
> expired_time = N * HZ; // HZ表示1s内系统的时钟滴答数，是个内核宏；   
# 时间获取
> 内核相关函数详细见 include/linux/jiffies.h   
```c
#ifdef __KERNEL__
#define get_current_jiffies() get_jiffies_64()
jiffies_to_msecs(get_jiffies_64())
#endif
```
