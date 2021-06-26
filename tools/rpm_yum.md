# centos
## 查询主机安装包
> rpm -qa|grep tcpdump

获取头文件所属软件包：yum provides /usr/include/rpc/types.h

累出软件包包含文件：rpm -ql glibc-headers | grep rpc

下载软件包：yumdownloader tcp_wrappers-libs.x86_64

rpm2cpio filename | cpio -div