#include <cstdlib>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <chrono>
using namespace std;
struct Param
{
    chrono::steady_clock::time_point begin ;
    int i;
};
/* Функция, которую будет исполнять созданный поток */
void *thread_job(void *arg)
{
    Param* p = (Param *)arg;
    auto end_begin = chrono::steady_clock::now();
    auto time = chrono::duration_cast<std::chrono::microseconds>(end_begin - p->begin);
    double mul = 1;
    int i = 0;
    for(; i < p->i; i++)
    {
        mul *=2;
    }

    auto end = chrono::steady_clock::now();
    auto time2 = chrono::duration_cast<std::chrono::microseconds>(end - end_begin);
    cout << "Кол-во операций "<<i << " Время запуска потока " << time.count() <<" время функции "<< time2.count()<< '\n';
    
}
int main(int argc, char *argv[])
{
 pthread_t thread;
    int err;
    Param param;
    for(int i = 1; i <= 100000000; i *= 10)
    {
        param.begin = chrono::steady_clock::now();
        param.i=i;
        // Создаём поток
        err = pthread_create(&thread, NULL, thread_job, (void*)&param);
        err = pthread_join(thread, NULL);
        if(err)
        {
            cout << "Cannot join a thread: " << strerror(err) << '\n';
            exit(-1);
        }
        // Если при создании потока произошла ошибка, выводим
        // сообщение об ошибке и прекращаем работу программы
        if(err)
        {
            cout << "Cannot create a thread: " << strerror(err) << endl;
            exit(-1);
        }
        // Ожидаем завершения созданного потока перед завершением
        // работы программы
        
    }
    pthread_exit(NULL);
    

}
