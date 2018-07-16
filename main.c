#include <stdio.h>
#include <stdlib.h>

#include "c_vector.h"

// В целях упрощения - проверка возвращаемых значений не выполняется.

int main(int agrc, char **argv)
{
    // Создадим вектор.
    c_vector *vector = c_vector_create(sizeof(uint8_t), 10);

    // Заполним вектор.
    printf("Source vector: \n");
    for (size_t i = 0; i < 10; ++i)
    {
        // Пытаемся добавить в конец вектора неинициализированный элемент.
        uint8_t *h_value = c_vector_push_back(vector);

        // Инициализируем добавленный элемент.
        *h_value = i;

        printf("value[%Iu] = %Iu\n", i, i);
    }
    printf("\n");

    // Сформируем массив удаляемых индексов.
    size_t indexes[] = {8, 0, 10009090, 1, 8, 11, 8, 7, 1};

    // Отобразим  его.
    printf("indexes:\n");
    for (size_t i = 0; i < sizeof(indexes) / sizeof(size_t); ++i)
    {
        printf("%Iu\n", indexes[i]);
    }
    printf("\n");

    // Удалим элементы с некоторыми индексами.
    c_vector_erase_few(vector, indexes, 9, NULL);

    // Выведем содержимое вектора после удаления.
    printf("Not source vector: \n");
    for (size_t i = 0; i < vector->size; ++i)
    {
        uint8_t *h_value = c_vector_at(vector, i);
        printf("vector[%Iu] = %Iu\n", i, (size_t)(*h_value) );
    }
    printf("\n");

    // Выведем содержимое индексов.
    printf("indexes: \n");
    for (size_t i = 0; i < sizeof(indexes) / sizeof(size_t); ++i)
    {
        printf("%Iu\n", indexes[i]);
    }
    printf("\n");

    getchar();
    return 0;
}
