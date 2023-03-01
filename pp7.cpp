#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstring>
#include <pthread.h>

using namespace std;

typedef double (*func_ptr_type)(double);

// Структура для передачи параметров в функцию потока
struct Param
{
    func_ptr_type func_ptr;
    double* array_ptr;
    int count_elements;
};

/* Функция, которую будет исполнять созданный поток */
void *thread_job(void *arg)
{
    Param* params = (Param*)arg;

    for(int i = 0; i < params->count_elements; ++i)
    {
        params->array_ptr[i] = params->func_ptr(params->array_ptr[i]);
    }
}

func_ptr_type get_func(int func)
{
    switch(func)
    {
    case 0:
        return &exp;
    case 1:
        return &sqrt;
    case 2:
        return &log;
    case 3:
        return &cos;
    case 4:
        return &sin;
    case 5:
        return &tan;
    case 6:
        return &abs;
    }
}
int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        cout << "недостаточно аргументов\n";
        exit(-1);
    }

    int count_threads = atoi(argv[1]);
    int array_size = atoi(argv[2]);
    int operation=atoi(argv[3]);

    if(array_size <= 0 || count_threads <= 0 || operation<=0 || operation>6)
    {
        cout << "неправильные параметры\n";
        exit(-1);
    }

    if(array_size < count_threads)
    {
        count_threads = array_size;
    }

    // Указатель на функцию
    func_ptr_type func_ptr = get_func(operation);

    srand(time(NULL));

    double* array = new double[array_size];

    for(int i = 0; i < array_size; i++)
    {
        array[i] = (rand() % 10);
        cout  << array[i] << '\n';
    }
    
    // Массив аргументов, которые будут переданы в функции потоков
    Param* params = new Param[count_threads];
    // Массив потоков
    pthread_t* threads = new pthread_t[count_threads];

    int count_elements_for_thread = array_size / count_threads;

    int err;

    for(int i = 0; i < count_threads; i++)
    {
        params[i].func_ptr = func_ptr;
        params[i].count_elements = count_elements_for_thread;
        params[i].array_ptr = &array[count_elements_for_thread * i];

        if(i == count_threads - 1)
        {
            params[i].count_elements += array_size % count_threads;
        }

        // Создание потока
        err = pthread_create(&threads[i], NULL, thread_job, (void*)&params[i]);
        if(err != 0)
        {
        cout << "Cannot create a thread: " << strerror(err) << endl;
        exit(-1);
        }

    }  

    // Ждем завершения всех созданных потоков
    for(int i = 0; i < count_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    
    cout << "\n------------------\n";
    for(int i = 0; i < array_size; i++)
    {
        cout  << array[i] << '\n';
    }
    delete[] params;
    delete[] array;
    delete[] threads;
}