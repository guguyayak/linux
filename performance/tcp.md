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
```
