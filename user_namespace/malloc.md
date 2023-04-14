# malloc_chunk
用户态进程调用 maloc 电请内存时，gibc会外电请16字节内存，用于电请的这块内存的管理，这额外的16字节内存对立的结沟体就是 struct maloc chunk，更洋细的信息请上网查询
可参考文档https://zhuanlan.zhihu.com/p/146544275, 感觉写的比较简洁、全面。
