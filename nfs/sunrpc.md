# socket 处理接口函数所在文件
```c
/*
 * linux/net/sunrpc/svcsock.c
 * These are the RPC server socket internals.
 */
 
 /*
 * linux/net/sunrpc/xprtsock.c
 * Client-side transport implementation for sockets.
 */
```


# nfs client
## xs_setup_tcp xprt数据接收函数初始化
```c
static struct rpc_xprt *xs_setup_tcp(struct xprt_create *args)
{
	...
	INIT_WORK(&transport->recv_worker, xs_tcp_data_receive_workfn);
	INIT_DELAYED_WORK(&transport->connect_worker, xs_tcp_setup_socket);
	...
}
```
# nfs server
## svc_xprt_ops
```c
static struct svc_xprt_ops svc_tcp_ops = {
	.xpo_create = svc_tcp_create,
	.xpo_recvfrom = svc_tcp_recvfrom,
	.xpo_sendto = svc_tcp_sendto,
	.xpo_release_rqst = svc_release_skb,
	.xpo_detach = svc_tcp_sock_detach,
	.xpo_free = svc_sock_free,
	.xpo_prep_reply_hdr = svc_tcp_prep_reply_hdr,
	.xpo_has_wspace = svc_tcp_has_wspace,
	.xpo_accept = svc_tcp_accept,
	.xpo_secure_port = svc_sock_secure_port,
	.xpo_kill_temp_xprt = svc_tcp_kill_temp_xprt,
};

svc_send -> xprt->xpt_ops->xpo_sendto(svc_tcp_sendto) -> svc_sendto
```
# sock ops
```c
crash> struct socket 0xffff94f2b57fcd80
struct socket {
  state = SS_CONNECTED,
  type = 1,
  flags = 0,
  wq = 0xffff94f2a7babe00,
  file = 0x0,
  sk = 0xffff94f2b8e11f00,
  ops = 0xffffffff89ea7820
}
crash> sym 0xffffffff89ea7820
ffffffff89ea7820 (R) inet_stream_ops
crash> sym 0xffffffff89c6e540
ffffffff89c6e540 (T) inet_sendmsg /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/net/ipv4/af_inet.c: 730
crash> sym 0xffffffff89c6e6d0
ffffffff89c6e6d0 (T) inet_recvmsg /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/net/ipv4/af_inet.c: 764
crash> sym 0xffffffff89c6e5f0
ffffffff89c6e5f0 (T) inet_sendpage /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/net/ipv4/af_inet.c: 746

crash> sym 0xffffffff8a302320
ffffffff8a302320 (D) tcp_prot
crash> sym 0xffffffff89c42a10
ffffffff89c42a10 (T) tcp_sendmsg /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/net/ipv4/tcp.c: 1073
crash> sym 0xffffffff89c3ff70
ffffffff89c3ff70 (T) tcp_recvmsg /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/net/ipv4/tcp.c: 1571
crash> sym 0xffffffff89c42450
ffffffff89c42450 (T) tcp_sendpage /usr/src/debug/kernel-3.10.0-862.el7/linux-3.10.0-862.el7.x86_64/net/ipv4/tcp.c: 999
```
