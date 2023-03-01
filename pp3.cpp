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
    double mul = 1;

    auto begin = chrono::steady_clock::now();

    for(int i = 0; i < *p; i++)
    {
        mul *=2;
    }

    auto end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<std::chrono::microseconds>(end - begin);
    cout<<" время функции "<< time.count();
}
int main(int argc, char *argv[])
{
 pthread_t thread;
    int err;
    for(int i = 1; i <= 100000000; i *= 10)
    {
        auto begin = chrono::steady_clock::now();
        // Создаём поток
        err = pthread_create(&thread, NULL, thread_job, (void*)&i);
        auto end = chrono::steady_clock::now();

        auto time = chrono::duration_cast<std::chrono::microseconds>(end - begin);

        err = pthread_join(thread, NULL);
        if(err)
        {
            cout << "Cannot join a thread: " << strerror(err) << '\n';
            exit(-1);
        }

        cout << " |Кол-во операций "<<i << " Время запуска потока " << time.count() << '\n';

        // Если при создании потока произошла ошибка, выводим
        // сообщение об ошибке и прекращаем работу программы
        if(err)
        {
            cout << "Cannot create a thread: " << strerror(err) << endl;
            exit(-1);
        }
    }
    
    // Ожидаем завершения созданного потока перед завершением
    // работы программы
    pthread_exit(NULL);
}
