# io
```
messagescan=no
data_errors=1

hd=default,vdbench=/root/vdbench_50406,user=root,shell=ssh

hd=hd1,system=10.17.81.232
hd=hd2,system=10.17.81.233
hd=hd3,system=10.17.81.234

fsd=default,depth=2,width=100,files=400,size=30K
fsd=fsd1,anchor=/mnt/fs002/vdbench/vd1_iops
fsd=fsd2,anchor=/mnt/fs002/vdbench/vd2_iops
fsd=fsd3,anchor=/mnt/fs002/vdbench/vd3_iops

fwd=format,threads=24,xfersize=30K
#fwd=default,threads=32,xfersize=4K,fileselect=random,fileio=random,openflags=o_direct
fwd=default,threads=32,xfersize=4K,fileselect=random,fileio=sequential
fwd=fwd1,fsd=fsd1,host=hd1
fwd=fwd2,fsd=fsd2,host=hd2
fwd=fwd3,fsd=fsd3,host=hd3

rd=rd1,fwd=fwd*,fwdrate=max,format=restart,elapsed=600000000,warmup=60,interval=1,rdpct=60
#rd=rd1,fwd=fwd*,fwdrate=max,format=(no,complete),elapsed=600000000,warmup=60,interval=1,rdpct=60
```
# 数据校验脚本
```
messagescan=no
data_errors=1

hd=hd1,system=10.17.91.22,vdbench=/home/vdbench,shell=ssh,user=root
fsd=default,journal=/var/crash/,openflags=o_direct
fsd=fsd1,anchor=/mnt/volume2/lmm,depth=2,width=5,files=1000,size=(256k,50,100m,50),shared=yes

fwd=format,threads=32,xfersize=1M
fwd=default,threads=32,xfersize=(128k)
#fwd=fwd1,fsd=fsd1,hd=hd1,threads=16,xfersize=128k,fileio=random,fileselect=sequential,rdpct=10
fwd=fwd1,fsd=fsd1,hd=hd1

rd=rd1,fwd=*,fwdrate=max,elapsed=300,interval=1,pause=5,format=restart,forrdpct=0

# 生成校验
./vdbench -f lmm -jn
# 检测校验
./vdbench -f lmm -jro
```
