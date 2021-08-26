# 创建 detach 线程：
```c
pthread_t tid;
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
pthread_create(&tid, &attr, THREAD_FUNCTION, arg);
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
