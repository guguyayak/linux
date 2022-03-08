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
#fwd=default,threads=32,xfersize=4K,fileselect=random,fileio=random,openflags=directio
fwd=default,threads=32,xfersize=4K,fileselect=random,fileio=random
fwd=fwd1,fsd=fsd1,host=hd1
fwd=fwd2,fsd=fsd2,host=hd2
fwd=fwd3,fsd=fsd3,host=hd3


rd=rd1,fwd=fwd*,fwdrate=max,format=restart,elapsed=600000000,warmup=60,interval=1,rdpct=60
```
