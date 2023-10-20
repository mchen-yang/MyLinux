#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int number=0;
pthread_rwlock_t rwlock;
void *write_pthread(void * arg)
{
        while(1)
        {       //加写锁
                pthread_rwlock_wrlock(&rwlock);
                number++;
                printf("write_pthread id:%ld, number is %d\n",pthread_self(),number);
                pthread_rwlock_unlock(&rwlock);
                //解锁
                sleep(1);
        }
}
void *read_pthread(void * arg)
{
        while(1)
        {       //加读锁
                pthread_rwlock_wrlock(&rwlock);
                printf("read_pthread id: %ld,number is %d\n",pthread_self(),number);
                pthread_rwlock_unlock(&rwlock);
                //解锁
               sleep(1);
        }
}

int main()
{
        pthread_t p[8];
        pthread_rwlock_init(&rwlock,NULL);
        for(int i=0;i<3;i++)
        {
                pthread_create(&p[i],NULL,write_pthread,NULL);
        }
        for(int i=3;i<8;i++)
        {
                pthread_create(&p[i],NULL,read_pthread,NULL);
                 }
        for(int j=0;j<8;j++)
        {
                pthread_join(p[j],NULL);
        }
        return 0;
}
