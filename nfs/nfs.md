# IP时间监控

```c
nfsd_inetaddr_event
nfsd_inet6addr_event
struct notifier_block nfsd_inetaddr_notifier = {
	.notifier_all = nfsd_inetaddr_event,
};
```
# mount.nfs参数
```
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

acregmin=1,acregmax=1 // 文件的属性有效时间，调小nfs客户端可能会频繁发getattr请求
lookupcache="none" // 不缓存 lookup 结果，nfs客户端访问文件每次都会发起 lookup 请求

[root@localhost ~]# mount -t nfs -o
ac             bg             fsc(和服务端无关，NFS客户端磁盘缓存)            mountport=     noauto         nordirplus（readdir不获取属性信息）     rdirplus（readdir不获取属性信息）       rw（挂载目录读写权限）             user
acdirmax=      context=       fscontext=     mountproto=    nocto          norelatime     rdma（rdma网络，我们现在不支持）           sec=（安全认证方式，默认sys，我们只支持sys）           users
acdirmin=      cto            group          mountvers=     nodev          noresvport     relatime       sharecache     vers=（nfs协议版本，我们只支持3，还要4.0，4.1，4.2）
acl            defaults       hard（指定此参数挂载卡住NFS客户端一直尝试）           namlen=lock    nodiratime     nosharecache   remount（重新挂载）        softhard（指定此参数挂载卡住NFS客户端尝试一定次数反错）           wsize=（一次写请求的最大字节数，协议规定此值最大1048576，即1M）
acregmax=（文件的属性有效时间，调小nfs客户端可能会频繁发getattr请求）      defcontext=    intr           namlen=nolock  noexec         nostrictatime  resvport       strictatime
acregmin=（文件的属性有效时间，调小nfs客户端可能会频繁发getattr请求）      dev            iversion       _netdev        nofail         nosuid         retrans=（重传次数）       suid
actimeo=       diratime       lookupcache= （会影响NFS客户端dentry缓存，不影响服务端）  nfsvers=（应该通vers）       nofsc（参考fsc）          nouser         retry=（重试次数）         sync（nfs客户端同步读写）
async（nfs客户端异步读写，sync和async应该只会影响NFS客户端，具体影响没研究过）          dirsync        loop           noac           nointr         owner          rorw（挂载目录只读权限）             tcp
atime          exec           mand           noacl          noiversion     port=          rootcontext=   timeo=
auto           fg             mounthost=     noatime        nomand         proto=         rsize=（一次读请求的最大字节数，协议规定此值最大1048576，即1M）         udp
```
# [exports参数解释](https://blog.csdn.net/qq_36357820/article/details/78488077)
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
## rpc ip addr
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

static int rpc_ntop4(const struct sockaddr *sap,
                     char *buf, const size_t buflen)
{
        const struct sockaddr_in *sin = (struct sockaddr_in *)sap;

        return snprintf(buf, buflen, "%pI4", &sin->sin_addr);
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
NFSv3 does not have a mechanism to negotiate concurrency between the client and the server. The client and the server each defines its limit without consulting the other.  
For the best performance, you should line up the maximum number of client-side sunrpc slot table entries with that supported without pushback on the server.   
When a client overwhelms the server network stack’s ability to process a workload, the server responds by decreasing the window size for the connection,   
which is not an ideal performance scenario.  
```
> Littles Law: (concurrency = operation rate × latency in seconds)  

# nfs 结构体转换宏
```c
// include/linux/nfs_fs.h
static inline struct nfs_inode *NFS_I(const struct inode *inode)
{
	return container_of(inode, struct nfs_inode, vfs_inode);
}

static inline struct nfs_server *NFS_SB(const struct super_block *s)
{
	return (struct nfs_server *)(s->s_fs_info);
}

static inline struct nfs_fh *NFS_FH(const struct inode *inode)
{
	return &NFS_I(inode)->fh;
}

static inline struct nfs_server *NFS_SERVER(const struct inode *inode)
{
	return NFS_SB(inode->i_sb);
}

static inline struct rpc_clnt *NFS_CLIENT(const struct inode *inode)
{
	return NFS_SERVER(inode)->client;
}

static inline const struct nfs_rpc_ops *NFS_PROTO(const struct inode *inode)
{
	return NFS_SERVER(inode)->nfs_client->rpc_ops;
}
```
# samething
```c
const struct dentry_operations nfs_dentry_operations = {
	.d_revalidate	= nfs_lookup_revalidate,
	.d_weak_revalidate	= nfs_weak_revalidate,
	.d_delete	= nfs_dentry_delete,
	.d_iput		= nfs_dentry_iput,
	.d_automount	= nfs_d_automount,
	.d_release	= nfs_d_release,
};
EXPORT_SYMBOL_GPL(nfs_dentry_operations);
```
# nfs uid gid设置
> struct svc_rqst.rq_cred.cr_uid/cr_gid 保存nfs客户端请求的用户信息   
> 通过函数 nfsd_setuser 把 uid gid信息封装到 current->cred   
> 底层文件系统通过宏 current_fsuid() 获取 uid信息   
```c
int nfsd_setuser(struct svc_rqst *rqstp, struct svc_export *exp)
{
	struct cred *new;
	new->fsuid = rqstp->rq_cred.cr_uid;
	new->fsgid = rqstp->rq_cred.cr_gid;
	put_cred(override_creds(new));
}
```

# nfs s_op
```
crash> p *$4->s_op
$9 = {
  alloc_inode = 0xffffffffc0da87d0 <nfs_alloc_inode>,
  destroy_inode = 0x0,
  free_inode = 0xffffffffc0da8820 <nfs_free_inode>,
  dirty_inode = 0x0,
  write_inode = 0xffffffffc0db80a0 <nfs_write_inode>,
  drop_inode = 0xffffffffc0da89a0 <nfs_drop_inode>,
  evict_inode = 0xffffffffc0daa220 <nfs_evict_inode>,
  put_super = 0x0,
  sync_fs = 0x0,
  freeze_super = 0x0,
  freeze_fs = 0x0,
  thaw_super = 0x0,
  unfreeze_fs = 0x0,
  statfs = 0xffffffffc0dab200 <nfs_statfs>,
  remount_fs = 0x0,
  umount_begin = 0xffffffffc0dabc20 <nfs_umount_begin>,
  show_options = 0xffffffffc0dabb00 <nfs_show_options>,
  show_devname = 0xffffffffc0dabb70 <nfs_show_devname>,
  show_path = 0xffffffffc0dabb50 <nfs_show_path>,
  show_stats = 0xffffffffc0dac7c0 <nfs_show_stats>,
  quota_read = 0x0,
  quota_write = 0x0,
  get_dquots = 0x0,
  nr_cached_objects = 0x0,
  free_cached_objects = 0x0,
  kabi_reserved1 = 0,
  kabi_reserved2 = 0,
  kabi_reserved3 = 0,
  kabi_reserved4 = 0
}
```
