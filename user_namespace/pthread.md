# 创建 detach 线程：
```c
pthread_t tid;
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
pthread_create(&tid, &attr, THREAD_FUNCTION, arg);
```
# pidwait and daemon
- 1. rpc.statd fork子进程执行sm-notify，并waitpid(pid, NULL, 0)；
- 2. 而子进程执行sm-notify（通过execv），sm-notify 调用daemon(0, 0)，waitpid就会收到返回值，不比等待sm-notify完全执行完成。
```c
// rpc.statd:
switch (pid = fork()) {
case 0:
	run_sm_notify(out_port);
	break;
case -1:
	break;
default:
	waitpid(pid, NULL, 0);
}

// sm-notify
daemon(0, 0)
```
# pthread_mutex_t
```c
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *print_msg(void *arg)
{
        int i = 0;
        pthread_mutex_lock(&mutex);
        for (; i < 15; i++) {
                printf("output : %d\n", i);

                usleep(100);
        }
        pthread_mutex_unlock(&mutex);
}

int main()
{
        pthread_t id1;
        pthread_t id2;

        pthread_mutex_init(&mutex, NULL);

        pthread_create(&id1, NULL, print_msg, NULL);
        pthread_create(&id2, NULL, print_msg, NULL);

        pthread_join(id1, NULL);
        pthread_join(id2, NULL);

        pthread_mutex_destroy(&mutex);
        return 0;
}
```
# eventfd
```c
#include <sys/eventfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main(int argc, char**argv[])
{
    int efd, j, i = 0;
    uint64_t u;
    ssize_t s;

    if (argc < 2)
    {
        printf("number of argc is wrong!\n");
        return 0;
    }

    efd = eventfd(0,0);
    if (-1 == efd)
    {
        printf("failed to create eventfd\n");
    }

    switch(fork())
    {
        case 0:
        {
            for(j=1; j<argc;j++)
            {
                printf("child writing %s to efd\n", argv[j]);
                u = strtoull(argv[j], NULL, 0);
                s = write(efd, &u, sizeof(uint64_t));
                if (s!=sizeof(uint64_t))
                {
                    printf("write efd failed\n");
                }
                sleep(1);
            }
            printf("Child completed write loop\n");
            exit(0);
        }
        default:
                printf("Parents about to read\n");
                while (++i < 3) {
                        s = read(efd, &u, sizeof(uint64_t));
                        if (s != sizeof(uint64_t))
                        {
                                printf("read efd failed\n");
                        }
                        printf("Parents read %llu (0x%llx) from efd\n", u, u);
                }
                break;
        case -1:
        {
            printf("fork error\n");
        }
    }

        close(efd);
        s = write(efd, &u, sizeof(uint64_t));
        if (s!=sizeof(uint64_t))
                printf("write efd failed %d\n", s);
        wait();

    return 0;
}
```
