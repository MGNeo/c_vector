/*
    Заголовочный файл вектора c_vector
    Разработка, отладка и сборка производилась в:
    ОС: Windows 10/x64
    IDE: Code::Blocks 17.12
    Компилятор: default Code::Blocks 17.12 MinGW

    Разработчик: Глухманюк Максим
    Эл. почта: mgneo@yandex.ru
    Место: Российская Федерация, Самарская область, Сызрань
    Дата: 16.03.2018
    Лицензия: GPLv3
*/

#ifndef C_VECTOR_H
#define C_VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct s_c_vector
{
    size_t size;
    size_t capacity;
    size_t size_of_element;
    void *data;
} c_vector;

c_vector *c_vector_create(const size_t _size_of_element,
                          const size_t _capacity);

ptrdiff_t c_vector_delete(c_vector *const _vector,
                          void (*const _del_data)(void *const _data));

void *c_vector_insert(c_vector *const _vector,
                      const size_t _index);

ptrdiff_t c_vector_erase(c_vector *const _vector,
                         const size_t _index,
                         void (*const _del_data)(void *const _data));

size_t c_vector_erase_few(c_vector *const _vector,
                          size_t *const _indexes,
                          const size_t _indexes_count,
                          void (*const _del_data)(void *const _data));

size_t c_vector_remove_few(c_vector *const _vector,
                           size_t (*const _comp_data)(const void *const _data),
                           void (*const _del_data)(void *const _data));

void *c_vector_push_back(c_vector *const _vector);

ptrdiff_t c_vector_pop_back(c_vector *const _vector,
                            void (*const _del_data)(void *const _data));

void *c_vector_push_front(c_vector *const _vector);

ptrdiff_t c_vector_pop_front(c_vector *const _vector,
                             void (*const _del_data)(void *const _data));

void *c_vector_at(const c_vector *const _vector,
                  const size_t _index);

void *c_vector_back(const c_vector *const _vector);

void *c_vector_front(const c_vector *const _vector);

void *c_vector_unsafe_at(const c_vector *const _vector,
                         const size_t _index);

ptrdiff_t c_vector_compress(c_vector *const _vector);

ptrdiff_t c_vector_set_capacity(c_vector *const _vector,
                                const size_t _capacity,
                                void (*const _del_data)(void *const _data));

ptrdiff_t c_vector_clear(c_vector *const _vector,
                         void (*const _del_data)(void *const _data));

ptrdiff_t c_vector_for_each(c_vector *const _vector,
                            void (*const _action_data)(void *const _data));

#endif
