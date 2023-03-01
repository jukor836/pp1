#include <cstdlib>
#include <iostream>
#include <cstring>
#include <pthread.h>
using namespace std;
void *thread_job(void *arg)
{
    pthread_t thread = pthread_self();
    pthread_attr_t attr;

    int err;
    err = pthread_getattr_np(thread, &attr);

    if(err != 0)
    {
        cout << "Cannot get thread attribute: " << strerror(err) << endl;
        exit(-1);
    }

    size_t stack_size;
    void* stack_addr;
    err = pthread_attr_getstack(&attr, &stack_addr, &stack_size);

    if(err != 0)
    {
        cout << "Cannot get stack: " << strerror(err) << endl;
        exit(-1);
    }

    size_t guard_size;
    err = pthread_attr_getguardsize(&attr, &guard_size);

    if(err != 0)
    {
        cout << "Cannot get guard size: " << strerror(err) << endl;
        exit(-1);
    }

    int detach_state;
    err = pthread_attr_getdetachstate(&attr, &detach_state);

    if(err != 0)
    {
        cout << "Cannot get detach state: " << strerror(err) << endl;
        exit(-1);
    }

    cout << "Stack size: " << stack_size << '\n';
    cout << "Guard size: " << guard_size << '\n';
    cout << "Stack address: " << stack_addr << '\n';
    detach_state == PTHREAD_CREATE_JOINABLE ? cout << "Thread is joinable\n" : 
                                              cout << "Thread is detached\n";

}

int main()
{
    pthread_t thread;
    int err;
    err = pthread_create(&thread, NULL, thread_job, NULL);
    if(err != 0)
    {
        cout << "Cannot create a thread: " << strerror(err) << endl;
        exit(-1);
    }
    pthread_exit(NULL);
}