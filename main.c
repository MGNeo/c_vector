#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "c_vector.h"

#define CAPACITY 5

typedef struct s_object
{
    int i;
    float f;
} object;

// Функция вывода содержимого элемента вектора типа object.
void print_object(void *const _data)
{
    if (_data == NULL)
    {
        return;
    }

    const object *const obj = (object*)_data;

    printf("i: %i, f: %f\n", obj->i, obj->f);
}

// Функция, определяющая, нужно ли удалять элемент типа object.
size_t pred_object(const void *const _data)
{
    if (_data == NULL)
    {
        return 0;
    }

    const object *const obj = (object*)_data;

    if (obj->f > 9)
    {
        return 1;
    }
    return 0;
}

int main(int agrc, char **argv)
{
    size_t error;
    c_vector *vector;

    // Пытаемся создать вектор емкостью CAPACITY, который содержит объекты типа object.
    vector = c_vector_create(sizeof(object), CAPACITY, &error);

    // Если вектор создать не удалось, выводим причину ошибки.
    if (vector == NULL)
    {
        printf("create error: %Iu\n", error);
        printf("Program end.\n");
        getchar();
        return -1;
    }

    // Вставляем в конец вектора 11 объектов.
    for (size_t i = 0; i < 11; ++i)
    {
        object *const obj = c_vector_push_back(vector, &error);
        // Если вставить объект не удалось, выводим причину ошибки.
        if (obj == NULL)
        {
            printf("push back error: %Iu\n", error);
            printf("Proogram end.\n");
            getchar();
            return -2;
        }

        // Инициализируем вставленный объект.
        obj->i = i;
        obj->f = i + 3.14f;
    }

    // Покажем размер вектора.
    {
        error = 0;
        const size_t size = c_vector_size(vector, &error);
        // Если size == 0 и error > 0, значит возникла ошибка.
        if ( (size == 0) && (error > 0) )
        {
            printf("size error: %Iu\n", error);
            printf("Program end.\n");
            getchar();
            return -3;
        }
        // Покажем размер.
        printf("size: %Iu\n", size);
    }

    // Покажем емкость вектора.
    {
        error = 0;
        const size_t capacity = c_vector_capacity(vector, &error);
        // Если capacity == 0 и error > 0, значит произошла ошибка.
        if ( (capacity == 0) && (error > 0) )
        {
            printf("capacity error: %Iu\n", error);
            printf("Program end.\n");
            getchar();
            return -4;
        }
        // Покажем емкость.
        printf("capacity: %Iu\n", capacity);
    }

    // Используя функцию обхода, выведем содержимое каждого элемента вектора.
    {
        const ptrdiff_t r_code = c_vector_for_each(vector, print_object);
        // Если произошла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("for each error, r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -5;
        }
    }

    // Удалим из вектора несколько элементов, используя массив индексов.
    {
        size_t indexes[5] = {0, 2, 1, 3, 99};
        error = 0;
        // Специализированная функция для удаления объекта типа object не нужна.
        const size_t d_count = c_vector_erase_few(vector, indexes, 5, NULL, &error);
        // Если произошла ошибка, покажем ее.
        if ( (d_count == 0) && (error > 0) )
        {
            printf("erase few error: %Iu\n", error);
            printf("Program end.\n");
            getchar();
            return -6;
        }
        // Покажем, сколько элементов было удалено.
        printf("d_count: %Iu\n", d_count);
    }

    // Используя функцию обхода, выведем содержимое каждого элемента вектора.
    {
        const ptrdiff_t r_code = c_vector_for_each(vector, print_object);
        // Если произошла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("for each error, r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -7;
        }
    }

    // Удалим из вектора все элементы, f которых > 9.
    {
        error = 0;
        const size_t d_count = c_vector_remove_few(vector, pred_object, NULL, &error);
        // Если произошла ошибка, покажем ее.
        if ( (d_count == 0) && (error > 0) )
        {
            printf("remove few error: %Iu\n", error);
            printf("Program end.\n");
            getchar();
            return -8;
        }
        // Покажем, сколько элементов было удалено.
        printf("d_count: %Iu\n", d_count);
    }

    // Используя функцию обхода, выведем содержимое каждого элемента вектора.
    {
        const ptrdiff_t r_code = c_vector_for_each(vector, print_object);
        // Если произошла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("for each error, r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -9;
        }
    }

    // Удалим вектор.
    {
        const ptrdiff_t r_code = c_vector_delete(vector, NULL);
        // Если возникла ошибка, покажем ее.
        if (r_code < 0)
        {
            printf("delete error, r_code: %Id\n", r_code);
            printf("Program end.\n");
            getchar();
            return -10;
        }
    }

    getchar();
    return 0;
}
