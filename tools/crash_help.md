# [crash help pages](https://crash-utility.github.io/help.html)
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

  In some kernel versions, the vfsmount structures of the mounted
  filesystems are linked by the LIST_HEAD "vfsmntlist", which uses the
  mnt_list list_head of each vfsmount structure in the list.  To dump each
  vfsmount structure in the list, append the -s option:

    crash> list -H vfsmntlist vfsmount.mnt_list -s vfsmount
    c3fc9e60
    struct vfsmount {
      mnt_hash = {
        next = 0xc3fc9e60,
        prev = 0xc3fc9e60
      },
      mnt_parent = 0xc3fc9e60,
      mnt_mountpoint = 0xc3fc5dc0,
      mnt_root = 0xc3fc5dc0,
      mnt_instances = {
        next = 0xc3f60a74,
        prev = 0xc3f60a74
      },
      mnt_sb = 0xc3f60a00,
      mnt_mounts = {
        next = 0xf7445e08,
        prev = 0xf7445f88
      },
      mnt_child = {
        next = 0xc3fc9e88,
        prev = 0xc3fc9e88
      },
      mnt_count = {
        counter = 209
      },
      mnt_flags = 0,
      mnt_devname = 0xc8465b20 "/dev/root",
      mnt_list = {
        next = 0xf7445f9c,
        prev = 0xc02eb828
      },
      mnt_owner = 0
    }
```
