# list
```c
  The list of currently-registered file system types are headed up by a
  struct file_system_type pointer named "file_systems", and linked by
  the "next" field in each file_system_type structure.  The following
  sequence displays the structure address followed by the name and
  fs_flags members of each registered file system type:

    crash> p file_systems
    file_systems = $1 = (struct file_system_type *) 0xc03adc90
    crash> list file_system_type.next -s file_system_type.name,fs_flags c03adc90
    c03adc90
      name = 0xc02c05c8 "rootfs",
      fs_flags = 0x30,
    c03abf94
      name = 0xc02c0319 "bdev",
      fs_flags = 0x10,
    c03acb40
      name = 0xc02c07c4 "proc",
      fs_flags = 0x8,
    c03e9834
      name = 0xc02cfc83 "sockfs",
      fs_flags = 0x10,
```
