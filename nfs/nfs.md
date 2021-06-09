# IP时间监控

```c
nfsd_inetaddr_event
nfsd_inet6addr_event
struct notifier_block nfsd_inetaddr_notifier = {
	.notifier_all = nfsd_inetaddr_event,
};
```
