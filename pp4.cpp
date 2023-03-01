#include <cstdlib>
#include <iostream>
#include <cstring>
#include <pthread.h>
using namespace std;
void *thread_job(void *arg)
{
    cout << "Thread is running..." << '\n';

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
    err = pthread_attr_getstacksize(&attr, &stack_size);

    if(err != 0)
    {
        cout << "Cannot get satck size: " << strerror(err) << endl;
        exit(-1);
    }

    cout << "Stack size: " << stack_size << '\n';
}

int main(int argc, char *argv[])
{
    int stack_size=1;
    if(argc!=1)
        stack_size=atoi(argv[1]);
    // Определяем переменные: идентификатор потока и код ошибки
    pthread_t thread;
    pthread_attr_t thread_attr;
    int err;
    // Инициализируем переменную для хранения атрибутов потока
    err = pthread_attr_init(&thread_attr);
    if(err != 0)
    {
        cout << "Cannot create thread attribute: " << strerror(err) << endl;
        exit(-1);
    }

    // Устанавливаем минимальный размер стека для потока (в байтах)
    err = pthread_attr_setstacksize(&thread_attr, stack_size*1024*1024);
    if(err != 0)
    {
        cout << "Setting stack size attribute failed: " << strerror(err) << endl;
        exit(-1);
    }

    // Создаём поток
    err = pthread_create(&thread, &thread_attr, thread_job, NULL);
    if(err != 0)
    {
        cout << "Cannot create a thread: " << strerror(err) << endl;
        exit(-1);
    }

    // Освобождаем память, занятую для хранения атрибутов потока
    pthread_attr_destroy(&thread_attr);
    
    // Ожидаем завершения созданного потока перед завершением
    // работы программы
    pthread_exit(NULL);
}