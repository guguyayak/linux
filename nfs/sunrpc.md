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
