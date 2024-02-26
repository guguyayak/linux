dd if=/dev/zero bs=1M | pv | dd of=/mnt/calibnet_t01888/test/hog4 bs=1M  
裁剪ko：strip --strip-debug net/sunrpc/sunrpc.ko  

[网络流量统计实用工具nicstat](https://www.linuxidc.com/Linux/2013-07/86941.htm)

# 免密
> ssh-keygen  
> ssh-copy-id IP
# nprsh
```
nprsh -on 173.7.0.1-48 "[指令]"
nprsh -f nodelist "pkill -9 iozone"
nodelist 文件保存节点ip
```
# lsof（list open files）
> -p<进程号> 列出指定进程号所打开的文件   
# slabtop 查看系统 slab 使用情况
# [auditd 监控文件变化](https://eternalcenter.com/auditd/)
