# 查看系统CPU及node信息--指令：lscpu
> cpu信息目录：/sys/devices/system/cpu/   
# 查看node信息
> 查看node物理内存信息：cat /sys/devices/system/node/[nodeid]/meminfo   
> 查看有物理内存的node：cat /sys/devices/system/node/has_normal_memory    
# 关于numa的一个调优实例
```
问题现象：
1、nfs大块写性能差，增加nfs客户端并发、bypass后端实际写动作，性能都没有明显提升；
2、iperf测试带宽，偶现性能差情况，10Gb网卡只跑到4Gb；
问题解决：
1、把nfsd线程绑核，它会在距离网卡最近的地方收包；
2、网卡插在cpu1上，nfsd如果在CPU2上，执行就会慢一倍；
3、把没有插内存的node上的core都屏蔽了；
nfsd线程绑核：ps -elf|grep "\[nfsd\]" | awk '{print $4}'|while read pid;do taskset -pc 0-7 $pid;done
```
# [转：numa介绍](https://blog.csdn.net/yk_wing4/article/details/87474172)
