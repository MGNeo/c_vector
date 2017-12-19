#ifndef C_VECTOR_H
#define C_VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef struct s_c_vector
{
    size_t size;
    size_t capacity;
    size_t size_of_element;
    void *data;
} c_vector;

ptrdiff_t c_vector_create(const size_t _size_of_element, const size_t _capacity);

ptrdiff_t c_vector_delete(c_vector *const _vector, void (*const _del_func(void *const _data)));

ptrdiff_t c_vector_insert(c_vector *const _vector, const size_t _index);
ptrdiff_t c_vector_erase(c_vector *const _vector, const size_t _index, void (*_del_func(void *const _data)));

ptrdiff_t c_vector_push_back(c_vector *const _vector);
ptrdiff_t c_vector_pop_back(c_vector *const _vector, void (*const _del_func(void *const _data)));

ptrdiff_t c_vector_push_front(c_vector *const _vector);
ptrdiff_t c_vector_pop_front(c_vector *const _vector, void (*const _del_func(void *const _data)));

void *c_vector_at(c_vector *const _vector, const size_t _index);// Отсортировать функции в *.c в порядке объявления в *.h
void *c_vector_back(c_vector *const _vector);
void *c_vector_front(c_vector *const _vector);

ptrdiff_t c_vector_compress(c_vector *const _vector);
ptrdiff_t c_vector_set_capacity(c_vector *const _vector, const size_t _capacity, void (*const _del_func(void *const _data)));

size_t c_vector_get_size(const c_vector *const _vector);
size_t c_vector_get_capacity(const c_vector *const _vector);
size_t c_vector_get_size_of_element(const c_vector *const _vector);
void *c_vector_get_data(c_vector *const _vector);

void *c_vector_clone(const c_vector *const _vector);

ptrdiff_t c_vector_clear(c_vector *const _vector, void (*const _del_func(void *const _data)));

ptrdiff_t c_vector_for_each(c_vector *const _vector, void (*const _func(void *const _data)));

#endif
