#include <cstdlib>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <chrono>
using namespace std;
/* Функция, которую будет исполнять созданный поток */
void *thread_job(void *arg)
{
    int* p = (int *)arg;
    for (int i=0;i<5;i++)
        cout<<" Параметр  "<<i<<"="<< p[i];
}
int main(int argc, char *argv[])
{
    int nums[] = {10, 11, 12, 13, 14};
    pthread_t thread;
    int err;

        err = pthread_create(&thread, NULL, thread_job, (void*)&nums);

        err = pthread_join(thread, NULL);
        if(err)
        {
            cout << "Cannot join a thread: " << strerror(err) << '\n';
            exit(-1);
        }
        if(err)
        {
            cout << "Cannot create a thread: " << strerror(err) << endl;
            exit(-1);
        }

    pthread_exit(NULL);
}
