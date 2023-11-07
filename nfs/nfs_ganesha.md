# nfs v3 decode
```
(gdb) p len
$36 = 28
(gdb) p delta
$37 = 1048660
(gdb) bt
#0  xdr_ioq_getbytes (xdrs=0x7f29c7009e80, addr=0x7f29d6c0e278 "", len=28) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/xdr_ioq.c:562
#1  0x00007f2a170c17f2 in xdr_opaque_decode (xdrs=0x7f29c7009e80, cp=0x7f29d6c0e278 "", cnt=28)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:440
#2  0x00007f2a170c1b4a in xdr_opaque_auth_decode_it (xdrs=0x7f29c7009e80, oa=0x7f29d6c0e270)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/auth_inline.h:118
#3  0x00007f2a170c1c73 in xdr_opaque_auth_decode (xdrs=0x7f29c7009e80, oa=0x7f29d6c0e270, buf=0x7f28fb5fbae0)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/auth_inline.h:146
#4  0x00007f2a170c2677 in xdr_call_decode (xdrs=0x7f29c7009e80, cmsg=0x7f29d6c0e088, buf=0x7f28fb5fbad8)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/rpc_callmsg.c:238
#5  0x00007f2a170c45ed in xdr_dplx_decode (xdrs=0x7f29c7009e80, dmsg=0x7f29d6c0e088)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/rpc_dplx_msg.c:458
#6  0x00007f2a170d8616 in svc_vc_decode (req=0x7f29d6c0e000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:911
#7  0x00007f2a170d43c1 in svc_request (xprt=0x7f29ed91aa00, xdrs=0x7f29c7009e80)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1298
#8  0x00007f2a170d85ba in svc_vc_recv (xprt=0x7f29ed91aa00) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:896
#9  0x00007f2a170d4306 in svc_rqst_xprt_task_recv (wpe=0x7f29ed91ac38) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1276
#10 0x00007f2a170d4ed3 in svc_rqst_epoll_loop (wpe=0x7f2a09532618) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1663
#11 0x00007f2a170e156d in work_pool_thread (arg=0x7f2941c00000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/work_pool.c:184
#12 0x00007f2a0ce4047a in ?? () from /usr/lib64/libc.so.6
#13 0x00007f2a0cec30e0 in ?? () from /usr/lib64/libc.so.6
(gdb) p len
$50 = 24
(gdb) p delta
$51 = 1048620
(gdb) bt
#0  xdr_ioq_getbytes (xdrs=0x7f2925c00d80, addr=0x7f295080b000 "C", len=24) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/xdr_ioq.c:562
#1  0x00007f2a1725e06e in xdr_opaque_decode (xdrs=0x7f2925c00d80, cp=0x7f295080b000 "C", cnt=24)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:440
#2  0x00007f2a1725e3a6 in xdr_bytes_decode (xdrs=0x7f2925c00d80, cpp=0x7f295080c818, sizep=0x7f295080c810, maxsize=64)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:603
#3  0x00007f2a1725e5c4 in xdr_bytes (xdrs=0x7f2925c00d80, cpp=0x7f295080c818, sizep=0x7f295080c810, maxsize=64)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:666
#4  0x00007f2a1725f0c9 in xdr_nfs_fh3 (xdrs=0x7f2925c00d80, objp=0x7f295080c810)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/Protocols/XDR/xdr_nfs23.c:214
#5  0x00007f2a172609fc in xdr_WRITE3args (xdrs=0x7f2925c00d80, objp=0x7f295080c810)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/Protocols/XDR/xdr_nfs23.c:917
#6  0x00007f2a170ce1b1 in svcauth_none_checksum (req=0x7f295080c000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_auth_none.c:62
#7  0x00007f2a1714345a in nfs_rpc_process_request (reqdata=0x7f295080c000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/MainNFSD/nfs_worker_thread.c:984
#8  0x00007f2a171458a6 in nfs_rpc_valid_NFS (req=0x7f295080c000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/MainNFSD/nfs_worker_thread.c:1716
#9  0x00007f2a170d86b1 in svc_vc_decode (req=0x7f295080c000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:923
#10 0x00007f2a170d43c1 in svc_request (xprt=0x7f29ed91aa00, xdrs=0x7f2925c00d80)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1298
#11 0x00007f2a170d85ba in svc_vc_recv (xprt=0x7f29ed91aa00) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:896
#12 0x00007f2a170d4306 in svc_rqst_xprt_task_recv (wpe=0x7f29ed91ac38) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1276
#13 0x00007f2a170d4ed3 in svc_rqst_epoll_loop (wpe=0x7f2a09532618) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1663
#14 0x00007f2a170e156d in work_pool_thread (arg=0x7f2974dff140) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/work_pool.c:184
#15 0x00007f2a0ce4047a in ?? () from /usr/lib64/libc.so.6
#16 0x00007f2a0cec30e0 in ?? () from /usr/lib64/libc.so.6
(gdb) p len
$39 = 1048576
(gdb) p delta
$40 = 1048576
(gdb) bt
#0  xdr_ioq_getbytes (xdrs=0x7f29c7009e80, addr=0x7f29d6ceb7c0 "", len=1048576) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/xdr_ioq.c:562
#1  0x00007f2a1725e06e in xdr_opaque_decode (xdrs=0x7f29c7009e80, cp=0x7f29d6ceb7c0 "", cnt=1048576)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:440
#2  0x00007f2a1725e3a6 in xdr_bytes_decode (xdrs=0x7f29c7009e80, cpp=0x7f29d6c0e838, sizep=0x7f29d6c0e830, maxsize=67108864)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:603
#3  0x00007f2a1725e5c4 in xdr_bytes (xdrs=0x7f29c7009e80, cpp=0x7f29d6c0e838, sizep=0x7f29d6c0e830, maxsize=67108864)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/ntirpc/rpc/xdr_inline.h:666
#4  0x00007f2a17260aa0 in xdr_WRITE3args (xdrs=0x7f29c7009e80, objp=0x7f29d6c0e810)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/Protocols/XDR/xdr_nfs23.c:925
#5  0x00007f2a170ce1b1 in svcauth_none_checksum (req=0x7f29d6c0e000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_auth_none.c:62
#6  0x00007f2a1714345a in nfs_rpc_process_request (reqdata=0x7f29d6c0e000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/MainNFSD/nfs_worker_thread.c:984
#7  0x00007f2a171458a6 in nfs_rpc_valid_NFS (req=0x7f29d6c0e000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/MainNFSD/nfs_worker_thread.c:1716
#8  0x00007f2a170d86b1 in svc_vc_decode (req=0x7f29d6c0e000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:923
#9  0x00007f2a170d43c1 in svc_request (xprt=0x7f29ed91aa00, xdrs=0x7f29c7009e80)
    at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1298
#10 0x00007f2a170d85ba in svc_vc_recv (xprt=0x7f29ed91aa00) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_vc.c:896
#11 0x00007f2a170d4306 in svc_rqst_xprt_task_recv (wpe=0x7f29ed91ac38) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1276
#12 0x00007f2a170d4ed3 in svc_rqst_epoll_loop (wpe=0x7f2a09532618) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/svc_rqst.c:1663
#13 0x00007f2a170e156d in work_pool_thread (arg=0x7f2941c00000) at /Jenkins_deb/workspace/EulerK510_build_Samba/UniStorOS_V100R001B80_Product/NFS-Ganesha-3.2/src/libntirpc/src/work_pool.c:184
#14 0x00007f2a0ce4047a in ?? () from /usr/lib64/libc.so.6
#15 0x00007f2a0cec30e0 in ?? () from /usr/lib64/libc.so.6
```
