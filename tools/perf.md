# perf
```sh
[root@c75n81p77 wangsen]# cat perf.sh
#!/bin/bash

if [ $# -ne 2 ]; then
        echo "$0 output pid"
        exit
fi

o=$1
p=$2

perf top -g -p $2 &> $o
[root@c75n81p77 wangsen]# ls
clear_iostat.sh  iozone_nfs  iozone.r  module.sh  perf.sh  read.sh  rw.sh
[root@c75n81p77 wangsen]# cat module.sh
#!/bin/bash

src=/cliparastor/tools/client
tag=/lib/modules/3.10.0-862.el7.x86_64/extra/

rm -rf $tag/$.ko

for i in dac.ko dlm.ko hrdma.ko kfnccli.ko knal.ko kutil.ko parastor.ko
do
        cp $src/$i $tag
done
[root@c75n81p77 wangsen]#
[root@c75n81p77 wangsen]# ls /lib/modules/3.10.0-862.el7.x86_64/extra/
dac.ko  dlm.ko  hrdma.ko  kfnccli.ko  knal.ko  kutil.ko  parastor.ko
```
## perf top -g -C 0-5
> -C后是CPU id，可以是一个值，可以是个范围值   
# [火焰图](https://www.brendangregg.com/FlameGraphs/cpuflamegraphs.html) [tools in git](https://github.com/brendangregg/FlameGraph)
> perf record -F 99 -p 13204 -g -- sleep 30   
> perf report -n --stdio   
> perf script | ./stackcollapse-perf.pl > out.perf-folded && ./flamegraph.pl out.perf-folded > perf.svg   
> 第一行是抓数据。第二行是简单解析数据（可以不用）。第三行是把第一行抓的数据生成一个能在浏览器中看的火焰图。   
> https://github.com/brendangregg/FlameGraph   第三行用的脚本可以在这里下载   
