# service脚本服务依赖设置
## 需求：service nfs-server start启动nfsd服务，启动完成后启动 bind_nfsd_to_CPU.service
1. 增加Before=bind_nfsd_to_CPU.service项，保证nfsd服务在bind_nfsd_to_CPU.service之前启动；
2. 但是，只增加Before项，执行service nfs-server start并不会启动bind_nfsd_to_CPU.service；
3. 需要加Wants项，这样执行service nfs-server start会触发启动bind_nfsd_to_CPU.service；
```
# nfs-server.service
[Unit]
Description=NFS server and services
DefaultDependencies=no
Requires= network.target proc-fs-nfsd.mount
Requires= nfs-mountd.service
Wants=rpcbind.socket network-online.target
Wants=rpc-statd.service nfs-idmapd.service
Wants=rpc-statd-notify.service
Wants=nfsdcld.service

After= network-online.target local-fs.target
After= proc-fs-nfsd.mount rpcbind.socket nfs-mountd.service
After= nfs-idmapd.service rpc-statd.service
After= nfsdcld.service
Before= rpc-statd-notify.service

# GSS services dependencies and ordering
Wants=auth-rpcgss-module.service
After=rpc-gssd.service gssproxy.service rpc-svcgssd.service

Wants=bind_nfsd_to_CPU.service
Before=bind_nfsd_to_CPU.service

Wants=bind_nfsd_to_CPU.service
Before=bind_nfsd_to_CPU.service

[Service]
Type=oneshot
RemainAfterExit=yes
ExecStartPre=/usr/sbin/exportfs -r
ExecStart=/usr/sbin/rpc.nfsd -N 4
ExecStop=/usr/sbin/rpc.nfsd 0
ExecStopPost=/usr/sbin/exportfs -au
ExecStopPost=/usr/sbin/exportfs -f

ExecReload=/usr/sbin/exportfs -r

[Install]
WantedBy=multi-user.target


# bind_nfsd_to_CPU.service
[Unit]
Description=Bind nfsd to CPU 0-7
DefaultDependencies=no

[Service]
Type=oneshot
# This service needs to run any time any nfs service
# is started, so changes to local config files get
# incorporated.  Having "RemainAfterExit=no" (the default)
# ensures this happens.
RemainAfterExit=no
ExecStart=/usr/lib/systemd/scripts/nfsd_taskset.sh


# nfsd_taskset.sh
#!/bin/sh

ps -elf | grep "\[nfsd\]" | awk '{print $4}' | while read pid; do taskset -pc 0-7 $pid; done
```
