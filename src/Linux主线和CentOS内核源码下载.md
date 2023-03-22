# Linux主线和CentOS内核源码下载
> 转，原文：https://zhuanlan.zhihu.com/p/357360744
## 手动下载方法
```
手动在https://git.centos.org/rpms/kernel/commits/c7?page=1 c7分支里查找.kernel.metadata的commit信息，
找到下面这些linux源码压缩包的sha1sum值，然后再到https://git.centos.org/sources/kernel/c7/上手动下载源码包.
wget https://git.centos.org/sources/kernel/c7/53f546c77c118d7cb6055fb430d5643b85cb0088
# 解压出linux-3.10.0-1127.19.1.el7目录
tar xf 53f546c77c118d7cb6055fb430d5643b85cb0088
cd linux-3.10.0-1127.19.1.el7
```
# [rpm包搜索下载](https://www.rpmfind.net/linux/rpm2html/)
> 内核源码包直接搜索kernel，再找对应版本，点进去可下载源码   
# [通用版本下载地址](https://vault.centos.org/)
# [不同版本内核源码在线查看](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree)
