#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "c_vector.h"

// В целях упрощения - проверка возвращаемых значений не выполняется.

// c_vector (в отличии от c_list, c_hash_set и пр.) может хранить как указатели, так и
// непосредственно сами элементы, минимизируя таким образом избыточное потребление памяти,
// если это необходимо.

typedef struct
{
    float x, y, z;
} point_3f;

void print_point_3f(void *const _data);
size_t pred_point_3f(const void *const _data);

void print_p_int(void *const _data);
void del_p_int(void *const _data);

int main(int agrc, char **argv)
{
    // Пример использования вектора, содержащего элементы типа point_3f.
    {
        // Создадим вектор.
        // Начальная емкость вектора = 5 элементам.
        c_vector *const vector = c_vector_create(sizeof(point_3f), 5);

        // Добавим в конец вектора.
        for (size_t i = 0; i < 10; ++i)
        {
            // Вставляем в конец вектора неинициализированный элемент и получаем
            // его адрес.
            point_3f *p = c_vector_push_back(vector);
            // Инициализируем.
            p->x = i;
            p->y = i;
            p->z = i;
        }

        // Вывод содержимого вектора.
        c_vector_for_each(vector, print_point_3f);
        printf("\n");

        // Удаляем элемент с порядковым индексом 5, не задавая
        // специализированную функцию удаления.
        c_vector_erase(vector, 5, NULL);

        // Вывод содержимого вектора.
        c_vector_for_each(vector, print_point_3f);
        printf("\n");

        // Удаляем из вектора все элементы, индексы которых есть в массиве.
        // Массив indexes автоматически будет отсортирован по возрастанию.
        size_t indexes[] = {9, 100, 3, 5};
        c_vector_erase_few(vector, indexes, 4, NULL);

        // Выводим содержимое вектора.
        c_vector_for_each(vector, print_point_3f);
        printf("\n");

        // Удаляем из вектора все элементы, для данных которых предикат возвращает > 0,
        // не задавая специализированную функцию удаления.
        c_vector_remove_few(vector, pred_point_3f, NULL);

        // Выводим содержимое вектора.
        c_vector_for_each(vector, print_point_3f);
        printf("\n");

        // Выводим общую информацию о векторе.
        printf("capacity: %Iu\n", c_vector_capacity(vector));
        printf("size: %Iu\n\n", c_vector_size(vector));

        // Приказываем вектору сжаться!
        c_vector_compress(vector);

        // Выводим общую информацию о векторе.
        printf("capacity: %Iu\n", c_vector_capacity(vector));
        printf("size: %Iu\n\n", c_vector_size(vector));

        // Удаляем вектор, не задавая специальную функцию удаления.
        c_vector_delete(vector, NULL);
    }

    // Пример вектора, содержащего указатели на int в куче.
    {
        // Создаем вектор.
        // Начальная емкость вектора = 3 элементам.
        c_vector *const vector = c_vector_create(sizeof(int*), 3);

        // Добавляем в начало вектора.
        for (size_t i = 0; i < 10; ++i)
        {
            // Вставляем в начало вектора неинициализированный int*
            // и получаем его адрес.
            int **const p = c_vector_push_front(vector);
            // Инициализируем вставленный указатель.
            *p = (int*)malloc(sizeof(int));
            // Инициализируем данные, на которые указывает вставленный указатель.
            **p = i;
        }

        // Вывод содержимого вектора.
        c_vector_for_each(vector, print_p_int);
        printf("\n");

        // Удаляем элемент с заданным порядковым индексом.
        // Специальная функция удаления задана.
        c_vector_erase(vector, 3, del_p_int);

        // Вывод содержимого вектора.
        c_vector_for_each(vector, print_p_int);
        printf("\n");

        // Выводим общую информацию о векторе.
        printf("capacity: %Iu\n", c_vector_capacity(vector));
        printf("size: %Iu\n\n", c_vector_size(vector));

        // Приказываем вектору сжаться!
        c_vector_compress(vector);

        // Выводим общую информацию о векторе.
        printf("capacity: %Iu\n", c_vector_capacity(vector));
        printf("size: %Iu\n\n", c_vector_size(vector));

        // Удаляем вектор, задав специальную функцию удаления.
        c_vector_delete(vector, del_p_int);
    }

    getchar();
    return 0;
}

// Функция печати элемента структуры.
void print_point_3f(void *const _data)
{
    if (_data == NULL)
    {
        return;
    }

    const point_3f *const point = (const point_3f *const)_data;

    printf("x: %03.3f y: %03.3f z: %03.3f\n",
           point->x,
           point->y,
           point->z);

    return;
}

// Функция, определяющая, стоит ли удалять элемент.
size_t pred_point_3f(const void *const _data)
{
    if (_data == NULL)
    {
        return 0;
    }

    const point_3f *const point = (const point_3f *const)_data;

    return (point->x > 6);
}

// Функция печати элемента int*.
void print_p_int(void *const _data)
{
    if (_data == NULL)
    {
        return;
    }

    const int *const *const p_int = (const int *const *const)_data;

    printf("address: %p value: %i\n", *p_int, **p_int);

    return;
}

// Функция специального удаления для элемента типа int*.
void del_p_int(void *const _data)
{
    if (_data == NULL)
    {
        return;
    }

    int *const *const p_int = (int *const *const)_data;
    free(*p_int);

    return;
}
