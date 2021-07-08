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
