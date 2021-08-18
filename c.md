# [getopt函数](https://segmentfault.com/a/1190000019806454)
```c
#include <unistd.h>
int getopt(int argc, char * const argv[],const char *optstring);

extern char *optarg;  //存储选项的参数
extern int optind    //指向下一个扫描的位置
        , opterr    //是否显示错误信息
        , optopt;   //读取到的字符如果不在opstring（上面例子是"alRtS"）中，则存入optopt中

#include <getopt.h>

int getopt_long(int argc, char * const argv[],const char *optstring,const struct option *longopts, int *longindex);

int getopt_long_only(int argc, char * const argv[],const char *optstring,const struct option *longopts, int *longindex);
```
