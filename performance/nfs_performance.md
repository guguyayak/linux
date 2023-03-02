# [oracle nfs performance](https://docs.oracle.com/cd/E19620-01/805-4448/6j47cnj0m/index.html)
# [nfs performance](https://www.admin-magazine.com/HPC/Articles/Useful-NFS-Options-for-Tuning-and-Management)
## 修改nfs业务网卡MTU值增大带宽
```
MTU

Changing the network MTU (maximum transmission unit) is also a good way to affect performance, but it is not an NFS tunable. Rather, it is a network option that you can tune on the system to improve NFS performance. The MTU is the maximum amount of data that can be sent via an Ethernet frame. The default MTU is typically 1500 (1,500 bytes per frame), but this can be changed fairly easily.

For the greatest effect on NFS performance, you will have to change the MTU on both the NFS server and the NFS clients. You should check both of these systems before changing the value to determine the largest MTU you can use. You also need to check for the largest MTU the network switches and routers between the NFS server and the NFS clients can accommodate (refer to the hardware documentation). Most switches, even non-managed "home" switches, can accommodate an MTU of 9000 (commonly called "jumbo packets").

The MTU size can be very important because it determines packet fragments on the network. If your chunk size is 8KB and the MTU is 1500, it will take six Ethernet frames to transmit the 8KB. If you increase the MTU to 9000 (9,000 bytes), the number of Ethernet frames drops to one.

The most common recommendation for better NFS performance is to set the MTU on both the NFS server and the NFS client to 9000 if the underlying network can accommodate it. A study by Dell a few years back examined the effect of an MTU of 1500 compared with an MTU of 9000. Using Netperf, they found that the bandwidth increased by about 33% when an MTU of 9000 was used.
```
# [Using Linux nfsiostat to troubleshoot nfs performance issue](https://www.howtouselinux.com/post/use-linux-nfsiostat-to-troubleshoot-nfs-performance-issue)
