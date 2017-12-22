#include "c_vector.h"

// Создание пустого весктора.
// В случае ошибки возвращает NULL.
// Позволяет создавать вектор с нулевой емкостью.
c_vector *c_vector_create(const size_t _size_of_element, const size_t _capacity)
{
    if (_size_of_element == 0) return NULL;
    c_vector *new_vector = (c_vector*)malloc(sizeof(c_vector));
    if (new_vector == NULL) return NULL;
    void *new_data = NULL;
    if (_capacity > 0)
    {
        const size_t data_size = _size_of_element * _capacity;
        if (data_size / _size_of_element != _capacity)
        {
            free(new_vector);
            return NULL;
        }
        new_data = malloc(data_size);
        if (new_data == NULL)
        {
            free(new_vector);
            return NULL;
        }
    }
    new_vector->size = 0;
    new_vector->capacity = _capacity;
    new_vector->size_of_element = _size_of_element;
    new_vector->data = new_data;
    return new_vector;
}

// Удаляет вектор.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_delete(c_vector *const _vector, void (*const _del_func(void *const _data)))
{
    if (_vector == NULL) return -1;
    if (_vector->size > 0)
    {
        if (_del_func != NULL)
        {
            for (size_t i = 0; i < _vector->size; ++i)
            {
                _del_func( (uint8_t*)_vector->data + i * _vector->size_of_element);
            }
        }
    }
    if (_vector->capacity > 0)
    {
        free(_vector->data);
    }
    free(_vector);
    return 1;
}

// Добавляет элемент в конец вектора.
// В случае успеха возвращает указатель на неинициализированные данные.
// В случае ошибки возвращает NULL.
void *c_vector_push_back(c_vector *const _vector)
{
    if (_vector == NULL) return NULL;
    if (_vector->size == _vector->capacity)
    {
        const size_t new_capacity = _vector->capacity * 1.5 + 1;
        // Проверка сразу двух возможных вариантов переполнения.
        if (new_capacity * _vector->size_of_element < _vector->capacity * _vector->size_of_element) return NULL;
        void *new_data = realloc(_vector->data, new_capacity * _vector->size_of_element);
        if (new_data == NULL) return NULL;
        _vector->data = new_data;
        _vector->capacity = new_capacity;
    }
    ++_vector->size;
    return (uint8_t*)_vector->data + (_vector->size - 1) * _vector->size_of_element;
}

// Добавляет неинициализированный элемент в произвольную позицию вектора [0; +size].
// В случае успеха возвращает указатель на вставленный неинициализированные данные.
// В случае ошибки возвращает NULL.
void *c_vector_insert(c_vector *const _vector, const size_t _index)
{
    if (_vector == NULL) return NULL;
    if (_index > _vector->size) return NULL;
    if (_vector->size == _vector->capacity)
    {
        const size_t new_capacity = _vector->capacity * 1.5 + 1;
        // Проверка сразу двух возможных вариантов переполнения.
        if (new_capacity * _vector->size_of_element < _vector->capacity * _vector->size_of_element) return -3;
        void *new_data = malloc(new_capacity * _vector->size_of_element);
        if (new_data == NULL) return NULL;
        if (_vector->size > 0)
        {
            if (_index == 0)
            {
                // Вставка в начало.
                memcpy((uint8_t*)new_data + _vector->size_of_element,
                       _vector->data,
                       _vector->size * _vector->size_of_element);
            } else{
                if (_index == _vector->size)
                {
                    // Вставка в конец.
                    memcpy(new_data,
                           _vector->data,
                           _vector->size * _vector->size_of_element);
                } else {
                    // Вставка в середину.
                    memcpy(new_data,
                           _vector->data,
                           _index  * _vector->size_of_element);

                    memcpy((uint8_t*)new_data + (_index + 1) * _vector->size_of_element,
                           (uint8_t*)_vector->data + _index * _vector->size_of_element,
                           (_vector->size - _index) * _vector->size_of_element);
                }
            }
            free(_vector->data);
        }
        _vector->data = new_data;
        _vector->capacity = new_capacity;
        ++_vector->size;
        return (uint8_t*)_vector->data + _index * _vector->size_of_element;
    } else {
        if (_vector->size > 0)
        {
            if (_index == 0)
            {
                // Вставка в начало.
                memmove((uint8_t*)_vector->data + _vector->size_of_element,
                        _vector->data,
                        _vector->size * _vector->size_of_element);
            } else {
                if (_index == _vector->size)
                {
                    // Вставка в конец.
                } else {
                    memmove((uint8_t*)_vector->data + (_index + 1) * _vector->size_of_element,
                            (uint8_t*)_vector->data + _index * _vector->size_of_element,
                            (_vector->size - _index) * _vector->size_of_element);
                }
            }
        }
        ++_vector->size;
        return (uint8_t*)_vector->data + _index * _vector->size_of_element;
    }
}

// Возвращает указатель на последний элемент вектора.
// В случае ошибки возвращает NULL.
void *c_vector_back(const c_vector *const _vector)
{
    if (_vector == NULL) return NULL;
    if (_vector->size == 0) return NULL;
    return (uint8_t*)_vector->data + (_vector->size - 1) * _vector->size_of_element;
}

// Возвращает указатель на первый элемент вектора.
// В случае ошибки возвращает NULL.
void *c_vector_front(const c_vector *const _vector)
{
    if (_vector == NULL) return NULL;
    if (_vector->size == 0) return NULL;
    return _vector->data;
}

// Возвращает указатель на элемент с заданным индексом.
// В случае ошибки возвращает NULL.
void *c_vector_at(const c_vector *const _vector, const size_t _index)
{
    if (_vector == NULL) return NULL;
    if (_index >= _vector->size) return NULL;
    return (uint8_t*)_vector->data + _index * _vector->size_of_element;
}

// Вставляет элемент в начало вектора.
// В случае успеха возвращает указатель на неинициализированные данные.
// В случае ошибки возвращает NULL.
void *c_vector_push_front(c_vector *const _vector)
{
    if (_vector == NULL) return NULL;
    if (_vector->size == _vector->capacity)
    {
        const size_t new_capacity = _vector->capacity * 1.5 + 1;
        // Проверка сразу двух возможных вариантов переполнения.
        if (new_capacity * _vector->size_of_element < _vector->capacity * _vector->size_of_element) return NULL;
        void *new_data = malloc(new_capacity * _vector->size_of_element);
        if (new_data == NULL) return NULL;
        if (_vector->size > 0)
        {
            memcpy((uint8_t*)new_data + _vector->size_of_element,
                   _vector->data,
                   _vector->size * _vector->size_of_element);
            free(_vector->data);
        }
        _vector->data = new_data;
        _vector->capacity = new_capacity;
        ++_vector->size;
        return (uint8_t*)_vector->data;
    } else {
        if (_vector->size > 0)
        {
            memmove((uint8_t*)_vector->data + _vector->size_of_element,
                    _vector->data,
                    _vector->size * _vector->size_of_element);
        }
        ++_vector->size;
        return (uint8_t*)_vector->data;
    }
}

// Удаляет последний элемент.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_pop_back(c_vector *const _vector, void (*const _del_func(void *const _data)))
{
    if (_vector == NULL) return -1;
    if (_vector->size == 0) return -2;
    if (_del_func != NULL)
    {
        _del_func((uint8_t*)_vector->data + (_vector->size - 1) * _vector->size_of_element);
    }
    --_vector->size;
    return 1;
}

// Удаляет первый элемент.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_pop_front(c_vector *const _vector, void (*const _del_func(void *const _data)))
{
    if (_vector == NULL) return -1;
    if (_vector->size == 0) return -2;
    if (_del_func != NULL)
    {
        _del_func( _vector->data );
    }
    if (_vector->size > 1)
    {
        memmove(_vector->data,
                (uint8_t*)_vector->data + _vector->size_of_element,
                (_vector->size - 1) * _vector->size_of_element);
    }
    --_vector->size;
    return 1;
}

// Вырезает элемент с заданным индексом.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_erase(c_vector *const _vector, const size_t _index, void (*_del_func(void * const _data)))
{
    if (_vector == NULL) return -1;
    if (_index >= _vector->size) return -2;
    if (_del_func != NULL)
    {
        _del_func( (uint8_t*)_vector->data + _index * _vector->size_of_element );
    }
    if (_vector->size > 1)
    {
        if (_index == 0)
        {
            // Вырезание из самого начала.
            memmove(_vector->data,
                    (uint8_t*)_vector->data + _vector->size_of_element,
                    (_vector->size - 1) * _vector->size_of_element);
        } else {
            if (_index == (_vector->size - 1))
            {
                // Вырезание с конца.
            } else {
                // Вырезание из середины.
                memmove((uint8_t*)_vector->data + _index * _vector->size_of_element,
                        (uint8_t*)_vector->data + (_index + 1) * _vector->size_of_element,
                        (_vector->size - _index - 1) * _vector->size_of_element);
            }
        }
    }
    --_vector->size;
    return 1;
}

// Очищает вектор от данных.
// Емкость не изменяется.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_clear(c_vector *const _vector, void (*const _del_func(void *const _data)))
{
    if (_vector == NULL) return -1;
    if (_del_func != NULL)
    {
        for (size_t i = 0; i < _vector->size; ++i)
        {
            _del_func((uint8_t*)_vector->data + i * _vector->size_of_element);
        }
    }
    _vector->size = 0;
    return 1;
}

// Клонирует вектор.
// В случае ошибки возвращает NULL.
void *c_vector_clone(const c_vector *const _vector)
{
    if (_vector == NULL) return NULL;
    c_vector *new_vector = (c_vector*)malloc(sizeof(c_vector));
    if (new_vector == NULL) return NULL;
    void *new_data = malloc(_vector->capacity * _vector->size_of_element);
    if (new_data == NULL)
    {
        free(new_vector);
        return NULL;
    }
    new_vector->capacity = _vector->capacity;
    new_vector->size = _vector->size;
    new_vector->size_of_element = _vector->size_of_element;
    if (_vector->size > 0)
    {
        memcpy(new_data, _vector->data, _vector->size * _vector->size_of_element);
    }
    new_vector->data = new_data;
    return new_vector;
}

// Ужимает вектор, перераспределяя память.
// Если size == 0, емкость так же приравнивается к 0.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_compress(c_vector *const _vector)
{
    if (_vector == NULL) return -1;
    size_t new_capacity = _vector->size;
    if (new_capacity > 0)
    {
        void *new_data = realloc(_vector->data, new_capacity * _vector->size_of_element);
        if (new_data == NULL) return -2;
        _vector->data = new_data;
    } else {
        free(_vector->data);
        _vector->data = NULL;
    }
    _vector->capacity = new_capacity;
    return 1;
}

// Задание вектору новой емкости.
// Если новая емкость меньше текущечего размера, лишние элементы обрезаются.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_set_capacity(c_vector *const _vector, const size_t _capacity, void (*const _del_func(void *const _data)))
{
    if (_vector == NULL) return -1;
    if (_capacity == _vector->capacity) return 1;
    if (_capacity > _vector->capacity)
    {
        // Проверка переполнения.
        if (_capacity * _vector->size_of_element < _vector->capacity * _vector->size_of_element)
        {
            return -2;
        }
        void *new_data = malloc(_capacity * _vector->size_of_element);
        if (new_data == NULL) return -3;
        if (_vector->size > 0)
        {
            memcpy(new_data, _vector->data, _vector->size * _vector->size_of_element);
        }
        if (_vector->capacity > 0)
        {
            free(_vector->data);
        }
        _vector->data = new_data;
        _vector->capacity = _capacity;
        return 2;
    } else {
        // Новая емкость равна 0.
        if (_capacity == 0)
        {
            if (_vector->size > 0)
            {
                if (_del_func != NULL)
                {
                    for (size_t i = 0; i < _vector->size; ++i)
                    {
                        _del_func((uint8_t*)_vector->data + i * _vector->size_of_element);
                    }
                }
            }
            if (_vector->capacity > 0)
            {
                free(_vector->data);
            }
            _vector->size = 0;
            _vector->capacity = 0;
            _vector->data = NULL;
            return 3;
        } else {
            // Новая емкость больше нуля, но меньше старой емкости.
            void *new_data = malloc(_capacity * _vector->size_of_element);
            if (new_data == NULL) return -4;
            memcpy(new_data, _vector->data, _capacity * _vector->size_of_element);
            // Если новая емкость еще и меньше размера.
            if (_capacity < _vector->size)
            {
                if (_del_func != NULL)
                {
                    for (size_t i = _capacity; i < _vector->size; ++i)
                    {
                        _del_func((uint8_t*)_vector->data + i * _vector->size_of_element);
                    }
                }
                _vector->size = _capacity;
            }
            free(_vector->data);
            _vector->data = new_data;
            _vector->capacity = _capacity;
            return 4;
        }
    }
}

// Проходит по всему размеру вектора и выполняет над данными каждого элемента действие _func.
// В случае успеха возвращает > 0, иначе < 0.
ptrdiff_t c_vector_for_each(c_vector *const _vector, void (*const _func(void *const _data)))
{
    if (_vector == NULL) return -1;
    if (_func == NULL) return -2;
    if (_vector->size == 0) return -3;
    for (size_t i = 0; i < _vector->size; ++i)
    {
        _func((uint8_t*)_vector->data + i * _vector->size_of_element);
    }
    return 1;
}

// Небезопасное обращение к элементу с заданным индексом.
// Никакие проверки не выполняются.
// Возвращает указатель на элемент.
void *c_vector_unsafe_at(const c_vector *const _vector, const size_t _index)
{
    return (uint8_t*)_vector->data + _index * _vector->size_of_element;
}
