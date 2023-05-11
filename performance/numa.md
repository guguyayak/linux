# 查看系统CPU及node信息--指令：lscpu
> cpu信息目录：/sys/devices/system/cpu/   
# 查看node信息
> 查看node物理内存信息：cat /sys/devices/system/node/[nodeid]/meminfo   
> 查看有物理内存的node：cat /sys/devices/system/node/has_normal_memory    
# 查看网卡物理上连接哪个 numa node
>  cat /sys/class/net/enp4s0f1/device/numa_node
# numa tools
```
numastat —— per-NUMA-node memory statistics
查看指定pid占用内存在各个numa上的分布情况：
numastat -p [pid]
查看numa信息，如CPU核，内存大小，各个node间距离：
numactl -H
numa内存带宽测试，即把内存数据读到CPU的速度：
time numactl -N 0 -m 0 ./a.out
-N 运行程序使用CPU核所在node编号
-m 运行程序使用内存所在node编号
a.out c语言写的简单的memcpy函数执行，就是不停的copy一块内存；
```
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
或者：ps -ef | grep "\[nfsd\]" | awk '{print $2}' | xargs  -i taskset -pc 0-7 {}

测试结果：
单节点，38nfs客户端对35服务端节点；
服务端bypass（写缓存，bypass dataio）：echo w > /proc/hdcfs/cli_bypass
测试执行：iozone -s 2g -r 1m -i 0 -+n -t 64 -I
前面测试：577MB/s
屏蔽无numa内存的CPU，把nfsd线程绑核：800MB/s
提升比：(800-577)/577 = 38%
```
# 获取页大小：getconf PAGE_SIZE
# 屏蔽CPU core
> grub里面有个isolcpus可以指定需要屏蔽的core的列表   
```
文件： /boot/efi/EFI/kylin/grub.cfg
### BEGIN /etc/grub.d/10_linux ###
menuentry 'Kylin Linux Advanced Server (4.19.90-25.2.v2101.gfb01.ky10.aarch64) V10 (Tercel)' --class kylin --class gnu-linux --class gnu --class os --unrestricted $menuentry_id_option 'gnulinux-4.19.90-25.2.v2101.gfb01.ky10.aarch64-advanced-aa9832a8-63c7-4252-85dc-718ac93b8240' {
        load_video
        set gfxpayload=keep
        insmod gzio
        insmod part_gpt
        insmod xfs
        set root='hd1,gpt2'
        if [ x$feature_platform_search_hint = xy ]; then
          search --no-floppy --fs-uuid --set=root --hint-ieee1275='ieee1275//sas/disk@20000,gpt2' --hint-bios=hd1,gpt2 --hint-efi=hd1,gpt2 --hint-baremetal=ahci1,gpt2  11e76df3-36c5-4e23-b801-ceb23978f3d8
        else
          search --no-floppy --fs-uuid --set=root 11e76df3-36c5-4e23-b801-ceb23978f3d8
        fi
        linux   /vmlinuz-4.19.90-25.2.v2101.gfb01.ky10.aarch64 root=/dev/mapper/klas-root ro crashkernel=1024M rd.lvm.lv=klas/root rd.lvm.lv=klas/swap acpi=on video=VGA-1:640x480-32@60me smmu.bypassdev=0x1000:0x17 smmu.bypassdev=0x1000:0x15 video=efifb:off video=VGA-1:640x480-32@60me isolcpus=8-31,40-63,72-95,104-127
        initrd /initramfs-4.19.90-25.2.v2101.gfb01.ky10.aarch64.img
}
```
> 查看屏蔽的core：isolcpus=8-31,40-63,72-95,104-127
```
[root@v10node38 lmm]# cat /proc/cmdline
BOOT_IMAGE=/vmlinuz-4.19.90-25.2.v2101.gfb01.ky10.aarch64 root=UUID=007b46ac-4958-411e-b0cf-4f1dbc9276c4 ro crashkernel=1024m acpi=on video=VGA-1:640x480-32@60me smmu.bypassdev=0x1000:0x17 smmu.bypassdev=0x1000:0x15 crashkernel=1024M,high video=efifb:off video=VGA-1:640x480-32@60me isolcpus=8-31,40-63,72-95,104-127
```
# [转：numa介绍](https://blog.csdn.net/yk_wing4/article/details/87474172)
