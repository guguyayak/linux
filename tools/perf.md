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
