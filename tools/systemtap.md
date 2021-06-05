# 运行脚本参数
> $1 ... $<NN>把参数解释成数字  
  @1 ... @<NN>把参数解释成字符串  
       In addition, script arguments given at the end of the command line may be inserted.  Use $1 ... $<NN> for insertion unquoted, @1 ...
       @<NN>  for  insertion as a string literal.  The number of arguments may be accessed through $# (as an unquoted number) or through @#
       (as a quoted number).  These may be used at any place a token may begin, including within the preprocessing stage.  Reference to  an
       argument number beyond what was actually given is an error.

# @cast()解释指针内容

       TYPECASTING
       Once  a pointer (see the CONTEXT VARIABLES section of stapprobes(3stap)) has been saved into a script integer variable, the transla‐
       tor loses the type information necessary to access members from that pointer.  Using the @cast() operator tells the  translator  how
       to interpret the number as a typed pointer.

              @cast(p, "type_name"[, "module"])->member

       This  will  interpret p as a pointer to a struct/union named type_name and dereference the member value.  Further ->subfield expres‐
       sions may be appended to dereference more levels. Note that for direct dereferencing of a  pointer  {kernel,user}_{char,int,...}($p)
       should  be  used.  (Refer to stapfuncs(5) for more details.)   NOTE: the same dereferencing operator -> is used to refer to both di‐
       rect containment or pointer indirection.  Systemtap automatically determines which.  The optional module tells the translator  where
       to look for information about that type.  Multiple modules may be specified as a list with : separators.  If the module is not spec‐
       ified, it will default either to the probe module for dwarf probes, or to "kernel" for functions and all other probes types.

       The translator can create its own module with type information from a header surrounded by angle brackets, in case normal  debuginfo
       is  not  available.   For  kernel headers, prefix it with "kernel" to use the appropriate build system.  All other headers are built
       with default GCC parameters into a user module.  Multiple headers may be specified in sequence to resolve a codependency.

              @cast(tv, "timeval", "<sys/time.h>")->tv_sec
              @cast(task, "task_struct", "kernel<linux/sched.h>")->tgid
              @cast(task, "task_struct",
                    "kernel<linux/sched.h><linux/fs_struct.h>")->fs->umask
