# 日志打印技巧
> Documentation/printk-formats.txt  
## 由dentry打印文件名
```c
dentry names:
        %pd{,2,3,4}
        %pD{,2,3,4}

        For printing dentry name; if we race with d_move(), the name might be
        a mix of old and new ones, but it won't oops.  %pd dentry is a safer
        equivalent of %s dentry->d_name.name we used to use, %pd<n> prints
        n last components.  %pD does the same thing for struct file.
example:
printk(KERN_ERR
	"nfsd_create: parent %pd2 not locked!\n",
	dentry);
```
## print IP
```c
IPv4 addresses:

        %pI4    1.2.3.4
        %pi4    001.002.003.004
        %p[Ii][hnbl]

        For printing IPv4 dot-separated decimal addresses. The 'I4' and 'i4'
        specifiers result in a printed address with ('i4') or without ('I4')
        leading zeros.

        The additional 'h', 'n', 'b', and 'l' specifiers are used to specify
        host, network, big or little endian order addresses respectively. Where
        no specifier is provided the default network/big endian order is used.

IPv6 addresses:

        %pI6    0001:0002:0003:0004:0005:0006:0007:0008
        %pi6    00010002000300040005000600070008
        %pI6c   1:2:3:4:5:6:7:8

        For printing IPv6 network-order 16-bit hex addresses. The 'I6' and 'i6'
        specifiers result in a printed address with ('I6') or without ('i6')
        colon-separators. Leading zeros are always used.

        The additional 'c' specifier can be used with the 'I' specifier to
        print a compressed IPv6 address as described by
        http://tools.ietf.org/html/rfc5952
example:
        if (myaddr.ss_family == AF_INET)
                dprintk("RPC:       %s %pI4:%u: %s (%d)\n", __func__,
                                &((struct sockaddr_in *)&myaddr)->sin_addr,
                                port, err ? "failed" : "ok", err);
        else
                dprintk("RPC:       %s %pI6:%u: %s (%d)\n", __func__,
                                &((struct sockaddr_in6 *)&myaddr)->sin6_addr,
                                port, err ? "failed" : "ok", err);
```
## 打印调用栈
> dump_stack();
