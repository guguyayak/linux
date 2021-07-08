# 字符串替换
- 1. sed替换的基本语法为:
> sed 's/原字符串/替换字符串/'   
> 示例  
> echo a_b_c | sed 's/\_/./g'  
> 输出：a.b.c  
> echo a_b_c | sed 's/_/./'  
> 输出：a.b_c  

# sed在文件指定位置插入内容
## 在指定位置后：sed -i '/passcode/a\to write' test.txt
## 在指定位置前：sed -i '/passcode/i\to write' test.txt
## 示例：
```shell
[root@node43 ~]# cat test.txt
aaa
[service]
bbb
[root@node43 ~]# sed -i '/\[service\]/a\behind\nbehind' test.txt
[root@node43 ~]# cat test.txt
aaa
[service]
behind
behind
bbb
[root@node43 ~]# sed -i '/\[service\]/i\front' test.txt
[root@node43 ~]# cat test.txt
aaa
front
[service]
behind
behind
bbb
```

# awk编辑输出结果：
[root@node43 system]# grep "ExecStart=/usr/sbin/" rpc* | awk -F '[: ]' '{print "    sed -i " "/"$2"/c\\"$2 " " $NF " " $1}'
    sed -i /ExecStart=/usr/sbin/rpc.gssd/c\ExecStart=/usr/sbin/rpc.gssd $GSSDARGS rpc-gssd.service
    sed -i /ExecStart=/usr/sbin/rpc.gssd/c\ExecStart=/usr/sbin/rpc.gssd $GSSDARGS rpcgssd.service
    sed -i /ExecStart=/usr/sbin/rpc.idmapd/c\ExecStart=/usr/sbin/rpc.idmapd $RPCIDMAPDARGS rpcidmapd.service
    sed -i /ExecStart=/usr/sbin/rpc.rquotad/c\ExecStart=/usr/sbin/rpc.rquotad $RPCRQUOTADOPTS rpc-rquotad.service
    sed -i /ExecStart=/usr/sbin/rpc.statd/c\ExecStart=/usr/sbin/rpc.statd $STATDARGS rpc-statd.service

## 打印所有列的内容

awk '/rs16945916/ {print $0}' test.txt > allcol.txt
