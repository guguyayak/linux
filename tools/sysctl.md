# 内核缓存阈值
缓存是一项重要的性能改进，在大多数情况下，读缓存在绝大多数情况下是有益无害的（程序可以直接从RAM中读取数据）。
写缓存比较复杂，Linux内核将磁盘写入缓存，过段时间再异步将它们刷新到磁盘。这对加速磁盘I/O有很好的效果，但是当数据未写入磁盘时，丢失数据的可能性会增加。
当然，也存在缓存被写爆的情况。还可能出现一次性往磁盘写入过多数据，以致使系统卡顿。这些卡顿是因为系统认为，缓存太大用异步的方式来不及把它们都写进磁盘，于是切换到同步的方式写入。

## 缓存参数

```
$ sysctl -a | grep dirty
vm.dirty_background_bytes = 0
vm.dirty_background_ratio = 10
vm.dirty_bytes = 0
vm.dirty_ratio = 20
vm.dirty_writeback_centisecs = 500
vm.dirty_expire_centisecs = 3000
vm.dirtytime_expire_seconds = 43200
```

- vm.dirty_background_ratio 是内存可以填充脏数据的百分比。这些脏数据稍后会写入磁盘，pdflush/flush/kdmflush这些后台进程会稍后清理脏数据。比如，我有32G内存，那么有3.2G的脏数据可以待着内存里，超过3.2G的话就会有后台进程来清理。
- vm.dirty_ratio是可以用脏数据填充的绝对最大系统内存量，当系统到达此点时，必须将所有脏数据提交到磁盘，同时所有新的I/O块都会被阻塞，直到脏数据被写入磁盘。这通常是长I/O卡顿的原因，但这也是保证内存中不会存在过量脏数据的保护机制。
- vm.dirty_background_bytes和vm.dirty_bytes是另一种指定这些参数的方法。如果设置_bytes版本，则_ratio版本将变为0，反之亦然。
- vm.dirty_expire_centisecs 指定脏数据能存活的时间。在这里它的值是30秒。当 pdflush/flush/kdmflush 在运行的时候，他们会检查是否有数据超过这个时限，如果有则会把它异步地写到磁盘中。毕竟数据在内存里待太久也会有丢失风险。
- vm.dirty_writeback_centisecs 指定多长时间 pdflush/flush/kdmflush 这些进程会唤醒一次，然后检查是否有缓存需要清理。

## 可以通过下面方式看内存中有多少脏数据：一共有106页的脏数据
```
$ cat /proc/vmstat | egrep "dirty|writeback"
nr_dirty 106
nr_writeback 0
nr_writeback_temp 0
nr_dirty_threshold 3934012
nr_dirty_background_threshold 1964604
```

无论你选择哪种方式，都应该始终收集数据来支持你的更改，并帮助你确定是在改进还是变得更糟。我们可以从应用程序，/proc/vmstat, /proc/meminfo, iostat, vmstat 以及/proc/sys/vm里面获得大量有用信息。
