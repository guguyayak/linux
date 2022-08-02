# iperf
> server: iperf3 -p 8100 -s   
> client: iperf3 -c 40.41.41.232 -p 8100 -P 40  
> 服务端iperf绑核：taskset -c 0-7 iperf -s
# dstat -- 查看CPU、磁盘、网络等使用情况
> 查看网卡速率：dstat -nf   
# mpstat 30 -- 查看CPU使用情况

# [sar](https://www.thomas-krenn.com/en/wiki/Collect_and_report_Linux_System_Activity_Information_with_sar)

# /dev/urandom
> dd if=/dev/urandom of=500MBfile bs=1M count=500   
> /dev/urandom 生成随机数，可拉升CPU使用率；   

# [火焰图](https://www.brendangregg.com/FlameGraphs/cpuflamegraphs.html) [tools in git](https://github.com/brendangregg/FlameGraph)
# htop查看CPU使用率
# 把一块内存挂载到一个目录上
> mount -t tmpfs -o size=1024m tmpfs /mnt/ram   
# [tcp_trace.stp](https://sourceware.org/systemtap/examples/network/tcp_trace.stp)
# 打印列解释
```
Tx-Q
send buffer 中数据量 + 没有收到ack的数据量
skc_state == 10 表示是TCP_LISTEN
if(@cast(sk, "sock_common")->skc_state == 10)
        new_txq = @cast(sk, "sock")->sk_max_ack_backlog
else
        new_txq = @cast(sk, "tcp_sock")->write_seq -
                        @cast(sk, "tcp_sock")->snd_una

Rx-Q
不懂。接收队列？
new_rxq = @cast(sk, "tcp_sock")->rcv_nxt -
                @cast(sk, "tcp_sock")->copied_seq

PMTU
@icsk_pmtu_cookie       Last pmtu seen by socket
new_pmtu = @cast(sk, "inet_connection_sock")->icsk_pmtu_cookie

SndCwnd
发送拥塞窗口
new_snd_cwnd = @cast(sk, "tcp_sock")->snd_cwnd
u32     snd_cwnd;       /* Sending congestion window            */

SndWnd
new_snd_wnd = @cast(sk, "tcp_sock")->snd_wnd
u32     snd_wnd;        /* The window we expect to receive      */

RcvWnd
new_rcvwnd = @cast(sk, "tcp_sock")->rcv_wnd
/* Current receiver window              */

SSRT
new_srtt = @choose_defined(@cast(sk, "tcp_sock")->srtt_us,
                           @cast(sk, "tcp_sock")->srtt)
/* smoothed round trip time << 3 in usecs */

Ssthreshold
u32     snd_ssthresh;   /* Slow start size threshold            */
/* 慢启动阈值的算法是取当前拥塞窗口和慢启动阈值的3/4 中的最大值。 */
new_ssthresh = tcp_current_ssthresh(sk)

Rexmit：重传相关的？

Length：应该是报文携带数据大小。
```
```c
enum {
        TCP_ESTABLISHED = 1,
        TCP_SYN_SENT,
        TCP_SYN_RECV,
        TCP_FIN_WAIT1,
        TCP_FIN_WAIT2,
        TCP_TIME_WAIT,
        TCP_CLOSE,
        TCP_CLOSE_WAIT,
        TCP_LAST_ACK,
        TCP_LISTEN,
        TCP_CLOSING,    /* Now a valid state */

        TCP_MAX_STATES  /* Leave at the end! */
};
```
