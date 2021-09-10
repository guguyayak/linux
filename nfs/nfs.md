# IP时间监控

```c
nfsd_inetaddr_event
nfsd_inet6addr_event
struct notifier_block nfsd_inetaddr_notifier = {
	.notifier_all = nfsd_inetaddr_event,
};
```
# mount.nfs参数
```sass
[root@localhost ~]# mount -t nfs 192.168.182.204:/mnt/lmm /mnt/lmm/ -o
ac             bg             fsc            mountport=     noauto         nordirplus     rdirplus       rw             user
acdirmax=      context=       fscontext=     mountproto=    nocto          norelatime     rdma           sec=           users
acdirmin=      cto            group          mountvers=     nodev          noresvport     relatime       sharecache     vers=
acl            defaults       hard           namlen=lock    nodiratime     nosharecache   remount        soft           wsize=
acregmax=      defcontext=    intr           namlen=nolock  noexec         nostrictatime  resvport       strictatime
acregmin=      dev            iversion       _netdev        nofail         nosuid         retrans=       suid
actimeo=       diratime       lookupcache=   nfsvers=       nofsc          nouser         retry=         sync
async          dirsync        loop           noac           nointr         owner          ro             tcp
atime          exec           mand           noacl          noiversion     port=          rootcontext=   timeo=
auto           fg             mounthost=     noatime        nomand         proto=         rsize=         udp
```
# sunrpc
## svc_export_cache
```c
static struct cache_detail svc_export_cache_template = {
	.owner		= THIS_MODULE,
	.hash_size	= EXPORT_HASHMAX,
	.name		= "nfsd.export",
	.cache_put	= svc_export_put,
	.cache_request	= svc_export_request,
	.cache_parse	= svc_export_parse,
	.cache_show	= svc_export_show,
	.match		= svc_export_match,
	.init		= svc_export_init,
	.update		= export_update,
	.alloc		= svc_export_alloc,
};
```
## rpc_cmp_addr
```c
static inline bool rpc_cmp_addr(const struct sockaddr *sap1,
				const struct sockaddr *sap2)
{
	if (sap1->sa_family == sap2->sa_family) {
		switch (sap1->sa_family) {
		case AF_INET:
			return rpc_cmp_addr4(sap1, sap2);
		case AF_INET6:
			return rpc_cmp_addr6(sap1, sap2);
		}
	}
	return false;
}
```
## 线程睡眠位置
```c
static struct svc_xprt *svc_get_next_xprt(struct svc_rqst *rqstp, long timeout)
{
	if (likely(rqst_should_sleep(rqstp)))
		time_left = schedule_timeout(timeout);
	else
		__set_current_state(TASK_RUNNING);
}
```
# rpcdebug
```sass
[root@dk4 ~]# rpcdebug -vh
usage: rpcdebug [-v] [-h] [-m module] [-s flags...|-c flags...]
       set or cancel debug flags.

Module     Valid flags
rpc        xprt call debug nfs auth bind sched trans svcsock svcdsp misc cache all
nfs        vfs dircache lookupcache pagecache proc xdr file root callback client mount fscache pnfs pnfs_ld state all
nfsd       sock fh export svc proc fileop auth repcache xdr lockd all
nlm        svc client clntlock svclock monitor clntsubs svcsubs hostcache xdr all
```
# 性能调优
## [slot table entries](https://docs.microsoft.com/en-us/azure/azure-netapp-files/performance-linux-concurrency-session-slots)
```
NFSv3 does not have a mechanism to negotiate concurrency between the client and the server. The client and the server each defines its limit without consulting the other. For the best performance, you should line up the maximum number of client-side sunrpc slot table entries with that supported without pushback on the server. When a client overwhelms the server network stack’s ability to process a workload, the server responds by decreasing the window size for the connection, which is not an ideal performance scenario.
```
