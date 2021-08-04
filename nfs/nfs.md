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
