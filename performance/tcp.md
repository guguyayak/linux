# 带宽时延积
```
在数据通信中，带宽时延乘积（英语：bandwidth-delay product；或称带宽延时乘积、带宽延时积等）指的是一个数据链路的能力（每秒比特）与来回通信延迟（单位秒）的乘积。
其结果是以比特（或字节）为单位的一个数据总量，等同在任何特定时间该网络线路上的最大数据量——已发送但尚未确认的数据。
```
# recv-Q send-Q
```
recv-Q 表示网络接收队列(receive queue)
表示收到的数据已经在本地接收缓冲，但是还有多少没有被用户进程取走，如果接收队列Recv-Q一直处于阻塞状态，可能是遭受了拒绝服务 denial-of-service 攻击。

send-Q 表示网路发送队列(send queue)
发送了，但是没有收到对方的Ack的, 还保留本地缓冲区.
如果发送队列Send-Q不能很快的清零，可能是有应用向外发送数据包过快，或者是对方接收数据包不够快。

这两个值通常应该为0，如果不为0可能是有问题的。packets在两个队列里都不应该有堆积状态。可接受短暂的非0情况。
```
# TX(发送) 和 RX(接收)
# MTU值，全名为：Maximum Transmission Unit 最大传输单元。
> 顾名思义，PMTU自动发现就是赋予应用程序权力让其自主发现端到端链路的MTU值，并绕开数据包分片这个坑。   
> 目前支持PMTU的协议为TCP以及UDP。   
# 拥塞窗口cwnd
# struct tcp_sock
```c
struct tcp_sock：
u32     rcv_nxt;        /* What we want to receive next         */
u32     copied_seq;     /* Head of yet unread data              */
u32     snd_wnd;        /* The window we expect to receive      */
u32     snd_cwnd;       /* Sending congestion window            */
u32     rcv_wnd;        /* Current receiver window              */
u32     srtt_us;        /* smoothed round trip time << 3 in usecs */
u32     snd_una;        /* First byte we want an ack for        */
u32     write_seq;      /* Tail(+1) of data held in tcp send buffer */
u32     snd_ssthresh;   /* Slow start size threshold            */
```
# [tcp performance-tuning](https://cromwell-intl.com/open-source/performance-tuning/tcp.html)
```
### /etc/sysctl.d/02-netIO.conf
### Kernel settings for TCP

# Provide adequate buffer memory.
# rmem_max and wmem_max are TCP max buffer size
# settable with setsockopt(), in bytes
# tcp_rmem and tcp_wmem are per socket in bytes.
# tcp_mem is for all TCP streams, in 4096-byte pages.
# The following are suggested on IBM's
# High Performance Computing page
net.core.rmem_max = 16777216
net.core.wmem_max = 16777216
net.core.rmem_default = 16777216
net.core.wmem_default = 16777216
net.ipv4.tcp_rmem = 4096 87380 16777216
net.ipv4.tcp_wmem = 4096 87380 16777216
# This server might have 200 clients simultaneously, so:
#   max(tcp_wmem) * 2 * 200 / 4096
net.ipv4.tcp_mem = 1638400 1638400 1638400

# Disable TCP SACK (TCP Selective Acknowledgement),
# DSACK (duplicate TCP SACK), and FACK (Forward Acknowledgement)
net.ipv4.tcp_sack = 0
net.ipv4.tcp_dsack = 0
net.ipv4.tcp_fack = 0

# Disable the gradual speed increase that's useful
# on variable-speed WANs but not for us
net.ipv4.tcp_slow_start_after_idle = 0 
```
# /proc/sys/net/core 
```
Documentation/sysctl/net.txt

rmem_default
------------

The default setting of the socket receive buffer in bytes.

rmem_max
--------

The maximum receive socket buffer size in bytes.

wmem_default
------------

The default setting (in bytes) of the socket send buffer.

wmem_max
--------

The maximum send socket buffer size in bytes.
```
# ipv4 tcp_mem & wmem & rmem
```
Documentation/networking/ip-sysctl.txt
/proc/sys/net/ipv4/* Variables:

tcp_mem - vector of 3 INTEGERs: min, pressure, max
        min: below this number of pages TCP is not bothered about its
        memory appetite.

        pressure: when amount of memory allocated by TCP exceeds this number
        of pages, TCP moderates its memory consumption and enters memory
        pressure mode, which is exited when memory consumption falls
        under "min".

        max: number of pages allowed for queueing by all TCP sockets.

        Defaults are calculated at boot time from amount of available
        memory.

tcp_rmem - vector of 3 INTEGERs: min, default, max
        min: Minimal size of receive buffer used by TCP sockets.
        It is guaranteed to each TCP socket, even under moderate memory
        pressure.
        Default: 1 page

        default: initial size of receive buffer used by TCP sockets.
        This value overrides net.core.rmem_default used by other protocols.
        Default: 87380 bytes. This value results in window of 65535 with
        default setting of tcp_adv_win_scale and tcp_app_win:0 and a bit
        less for default tcp_app_win. See below about these variables.

        max: maximal size of receive buffer allowed for automatically
        selected receiver buffers for TCP socket. This value does not override
        net.core.rmem_max.  Calling setsockopt() with SO_RCVBUF disables
        automatic tuning of that socket's receive buffer size, in which
        case this value is ignored.
        Default: between 87380B and 6MB, depending on RAM size.

tcp_wmem - vector of 3 INTEGERs: min, default, max
        min: Amount of memory reserved for send buffers for TCP sockets.
        Each TCP socket has rights to use it due to fact of its birth.
        Default: 1 page

        default: initial size of send buffer used by TCP sockets.  This
        value overrides net.core.wmem_default used by other protocols.
        It is usually lower than net.core.wmem_default.
        Default: 16K

        max: Maximal amount of memory allowed for automatically tuned
        send buffers for TCP sockets. This value does not override
        net.core.wmem_max.  Calling setsockopt() with SO_SNDBUF disables
        automatic tuning of that socket's send buffer size, in which case
        this value is ignored.
        Default: between 64K and 4MB, depending on RAM size.
```
