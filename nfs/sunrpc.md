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
