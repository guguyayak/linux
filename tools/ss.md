              skmem:(r<rmem_alloc>,rb<rcv_buf>,t<wmem_alloc>,tb<snd_buf>,
                            f<fwd_alloc>,w<wmem_queued>,o<opt_mem>,
                            bl<back_log>,d<sock_drop>)

              <rmem_alloc>
                     the memory allocated for receiving packet

              <rcv_buf>
                     the total memory can be allocated for receiving packet

              <wmem_alloc>
                     the memory used for sending packet (which has  been  sent
                     to layer 3)

              <snd_buf>
                     the total memory can be allocated for sending packet

              <fwd_alloc>
                     the memory allocated by the socket as cache, but not used
                     for receiving/sending  packet  yet.  If  need  memory  to
                     send/receive  packet,  the  memory  in this cache will be
                     used before allocate additional memory.

              <wmem_queued>
                     The memory allocated for sending packet  (which  has  not
                     been sent to layer 3)

              <ropt_mem>
                     The  memory used for storing socket option, e.g., the key
                     for TCP MD5 signature

              <back_log>
                     The memory used for the sk backlog queue.  On  a  process
                     context,  if  the  process is receiving packet, and a new
                     packet is received, it will be put into  the  sk  backlog
                     queue, so it can be received by the process immediately

              <sock_drop>
                     the  number  of packets dropped before they are de-multi‐
                     plexed into the socket