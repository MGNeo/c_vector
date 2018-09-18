/*
    Файл реализации вектора c_vector
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "c_vector.h"

struct s_c_vector
{
    size_t size;
    size_t capacity;
    size_t size_of_element;
    void *data;
};

// Если расположение задано, в него помещается код.
static void error_set(size_t *const _error,
                      const size_t _code)
{
    if (_error != NULL)
    {
        *_error = _code;
    }
}

// Компаратор для сортировки массива, на который указывает _indexes.
static int comp_sort(const void *const _index_a,
                     const void *const _index_b)
{
    if ( (_index_a == NULL) || (_index_b == NULL) )
    {
        return 0;
    }

    const size_t index_a = *((size_t*)_index_a);
    const size_t index_b = *((size_t*)_index_b);

    if (index_a > index_b)
    {
        return 1;
    } else {
        if (index_a == index_b)
        {
            return 0;
        } else {
            return -1;
        }
    }
}

// Создание пустого вектора.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Позволяет создавать вектор с нулевой емкостью.
c_vector *c_vector_create(const size_t _size_of_element,
                          const size_t _capacity,
                          size_t *const _error)
{
    if (_size_of_element == 0)
    {
        error_set(_error, 1);
        return NULL;
    }

    void *new_data = NULL;
    if (_capacity > 0)
    {
        // Определим размер данных и проконтролируем возможное арифметическое переполнение.
        const size_t data_size = _size_of_element * _capacity;
        if ((data_size == 0) || (data_size / _size_of_element != _capacity))
        {
            error_set(_error, 2);
            return NULL;
        }
        // Попытаемся выделить память под данные.
        new_data = malloc(data_size);
        if (new_data == NULL)
        {
            error_set(_error, 3);
            return NULL;
        }
    }
    // Попытаемся выделить память под вектор.
    c_vector *const new_vector = malloc(sizeof(c_vector));
    if (new_vector == NULL)
    {
        free(new_data);
        error_set(_error, 4);
        return NULL;
    }
    new_vector->size = 0;
    new_vector->capacity = _capacity;
    new_vector->size_of_element = _size_of_element;
    new_vector->data = new_data;
    return new_vector;
}

// Удаляет вектор.
// В случае успеха возвращает > 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_delete(c_vector *const _vector,
                          void (*const _del_func)(void *const _data))
{
    if (c_vector_clear(_vector, _del_func) < 0)
    {
        return -1;
    }
    free(_vector->data);

    free(_vector);

    return 1;
}

// Добавляет элемент в конец вектора.
// В случае успеха возвращает указатель на добавленный неинициализированный элемент.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
void *c_vector_push_back(c_vector *const _vector,
                         size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }

    if (_vector->size == _vector->capacity)
    {
        // Определим новую емкость.
        size_t new_capacity = (size_t)(_vector->capacity * 1.5f);
        if (new_capacity < _vector->capacity)
        {
            error_set(_error, 2);
            return NULL;
        }
        new_capacity += 1;
        if (new_capacity == 0)
        {
            error_set(_error, 3);
            return NULL;
        }

        // Определим новый размер data.
        const size_t new_data_size = new_capacity * _vector->size_of_element;
        if ( (new_data_size == 0) ||
             (new_data_size / new_capacity != _vector->size_of_element) )
        {
            error_set(_error, 4);
            return NULL;
        }
        // Попытаемся выделить память под данные.
        void *const new_data = realloc(_vector->data, new_data_size);
        if (new_data == NULL)
        {
            error_set(_error, 5);
            return NULL;
        }

        _vector->data = new_data;
        _vector->capacity = new_capacity;
    }
    ++_vector->size;
    return (uint8_t*)_vector->data + (_vector->size - 1) * _vector->size_of_element;
}

// Добавляет элемент в произвольную позицию вектора [0; +size].
// В случае успеха возвращает указатель на добавленный неинициализированный элемент.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Вставка в позицию > size запрещена, это не считается ошибкой, функция вернет NULL.
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в расположение ошибки.
void *c_vector_insert(c_vector *const _vector,
                      const size_t _index,
                      size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_index > _vector->size)
    {
        return NULL;
    }
    if (_vector->size == _vector->capacity)
    {
        // Определим новую емкость.
        size_t new_capacity = (size_t)(_vector->capacity * 1.5f);
        if (new_capacity < _vector->capacity)
        {
            error_set(_error, 2);
            return NULL;
        }
        new_capacity += 1;
        if (new_capacity == 0)
        {
            error_set(_error, 3);
            return NULL;
        }
        // Определим новый размер data.
        const size_t new_data_size = new_capacity * _vector->size_of_element;
        if ( (new_data_size == 0) ||
             (new_data_size / new_capacity != _vector->size_of_element) )
        {
            error_set(_error, 4);
            return NULL;
        }

        // Попытаемся выделить память под данные.
        void *const new_data = malloc(new_data_size);
        if (new_data == NULL)
        {
            error_set(_error, 5);
            return NULL;
        }
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
                    // Вставка в середину.
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
// Если вектор пуст, возвращает NULL.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
void *c_vector_back(const c_vector *const _vector,
                    size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_vector->size == 0)
    {
        return NULL;
    }
    return (uint8_t*)_vector->data + (_vector->size - 1) * _vector->size_of_element;
}

// Возвращает указатель на первый элемент вектора.
// Если вектор пуст, возвращает NULL.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
void *c_vector_front(const c_vector *const _vector,
                     size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_vector->size == 0)
    {
        return NULL;
    }
    return _vector->data;
}

// Возвращает указатель на элемент с заданным индексом.
// Если индекс оказался >= размеру вектора, функция возвращает NULL, это не считается ошибкой.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
// Так как функция может возвращать NULL и в случае успеха, и в случае ошибки, для детектирования
// ошибки перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
void *c_vector_at(const c_vector *const _vector,
                  const size_t _index,
                  size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_index >= _vector->size)
    {
        return NULL;
    }
    return (uint8_t*)_vector->data + _index * _vector->size_of_element;
}

// Вставляет неинициализированный элемент в начало вектора.
// В случае успеха возвращает указатель на неинициализированный элемент.
// В случае ошибки возвращает NULL, и если _error != NULL, в заданное расположение
// помещается код причины ошибки (> 0).
void *c_vector_push_front(c_vector *const _vector,
                          size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return NULL;
    }
    if (_vector->size == _vector->capacity)
    {
        // Определим новую емкость.
        size_t new_capacity = (size_t)(_vector->capacity * 1.5f);
        if (new_capacity < _vector->capacity)
        {
            error_set(_error, 2);
            return NULL;
        }
        new_capacity += 1;
        if (new_capacity == 0)
        {
            error_set(_error, 3);
            return NULL;
        }
        // Определим новый размер data.
        const size_t new_data_size = new_capacity * _vector->size_of_element;
        if ( (new_data_size == 0) ||
             (new_data_size / new_capacity != _vector->size_of_element) )
        {
            error_set(_error, 4);
            return NULL;
        }

        // Попытаемся выделить память под данные.
        void *const new_data = malloc(new_data_size);

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
// Если элемент был удален, возвращает > 0.
// Если в векторе не было элементов, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_pop_back(c_vector *const _vector,
                            void (*const _del_data)(void *const _data))
{
    if (_vector == NULL) return -1;
    if (_vector->size == 0) return 0;
    if (_del_data != NULL)
    {
        _del_data( (uint8_t*)_vector->data + (_vector->size - 1) * _vector->size_of_element );
    }
    --_vector->size;
    return 1;
}

// Удаляет первый элемент.
// Если элемент был удален, возвращает > 0.
// Если в векторе не было элементов, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_pop_front(c_vector *const _vector,
                             void (*const _del_data)(void *const _data))
{
    if (_vector == NULL) return -1;
    if (_vector->size == 0) return 0;
    if (_del_data != NULL)
    {
        _del_data( _vector->data );
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

// Удаляет элемент с заданным индексом.
// Если элемент был удален, возвращает > 0.
// Если индекс >= size, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_erase(c_vector *const _vector,
                         const size_t _index,
                         void (*_del_data)(void * const _data))
{
    if (_vector == NULL) return -1;
    if (_index >= _vector->size) return 0;
    if (_del_data != NULL)
    {
        _del_data( (uint8_t*)_vector->data + _index * _vector->size_of_element );
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

// Удаление нескольких елементов с заданными индексами, от начала, от 0.
// В случае успеха возвращает кол-во удаленных элементов.
// В случае ошибки возвращает 0, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
// Так как функция может возвращать 0 и в случае успеха, и в случае ошибки, для детектирования ошибки
// перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
// Если какой-либо индекс >= _vector->size - это не считается ошибкой.
// Функция сортирует массив, на который указывает _indexes, а так же избавляется от одинаковых индексов.
size_t c_vector_erase_few(c_vector *const _vector,
                          size_t *const _indexes,
                          const size_t _indexes_count,
                          void (*const _del_data)(void *const _data),
                          size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return 0;
    }
    if (_indexes == NULL)
    {
        return 0;
    }
    if (_indexes_count == 0)
    {
        return 0;
    }
    if (_vector->size == 0) return 0;

    // Отсортируем массив с индексами.
    qsort(_indexes, _indexes_count, sizeof(size_t), comp_sort);

    // Если в массиве вообще нет корректных индексов - умываем руки.
    if (_indexes[0] >= _vector->size) return 0;

    // Избавимся от повторяющихся индексов.
    size_t i_index = 0;
    for (size_t i = 1; (i < _indexes_count) && (_indexes[i] < _vector->size); ++i)
    {
        if (_indexes[i] != _indexes[i - 1])
        {
            _indexes[++i_index] = _indexes[i];
        }
    }

    // Удалим и сдвинем.
    i_index = 0;

    // Макросы дублирования кода для избавления от проверок внутри цикла.

    // Открытие цикла.
    #define C_VECTOR_ERASE_FEW_BEGIN\
    for (size_t i = _indexes[0]; i < _vector->size; ++i)\
    {\
        if (_indexes[i_index] == i)\
        {

    // Закрытие цикла.
    #define C_VECTOR_ERASE_FEW_END\
            ++i_index;\
            } else {\
                if (i_index > 0)\
                {\
                    memmove((uint8_t*)_vector->data + (i - i_index) * _vector->size_of_element,\
                            (uint8_t*)_vector->data + i * _vector->size_of_element,\
                            _vector->size_of_element);\
                }\
            }\
        }

    // Функция удаления данных задана.
    if (_del_data != NULL)
    {
        C_VECTOR_ERASE_FEW_BEGIN

        _del_data((uint8_t*)_vector->data + i * _vector->size_of_element);

        C_VECTOR_ERASE_FEW_END
    } else {
        // Функция удаления данных не задана.
        C_VECTOR_ERASE_FEW_BEGIN

        C_VECTOR_ERASE_FEW_END
    }

    #undef C_VECTOR_ERASE_FEW_BEGIN
    #undef C_VECTOR_ERASE_FEW_END

    _vector->size -= i_index;

    return i_index;
}

// Вырезает все элементы, для данных которых _pred_data() возвращает > 0.
// В случае успеха возвращает кол-во удаленных элементов.
// В случае ошибки возвращает 0, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
// Так как функция может возвращать 0 и в случае успеха, и в случае ошибки, для детектирования ошибки
// перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
size_t c_vector_remove_few(c_vector *const _vector,
                           size_t (*const _pred_data)(const void *const _data),
                           void (*const _del_data)(void *const _data),
                           size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return 0;
    }
    if (_pred_data == NULL)
    {
        error_set(_error, 2);
        return 0;
    }
    if (_vector->size == 0) return 0;

    size_t offset = 0;

    // Макросы дублирования кода для исключения проверок из цикла.

    // Открытие цикла.
    #define C_VECTOR_REMOVE_FEW_BEGIN\
    for (size_t i = 0; i < _vector->size; ++i)\
    {\
        if (_pred_data((uint8_t*)_vector->data + i * _vector->size_of_element) > 0)\
        {

    // Закрытие цикла.
    #define C_VECTOR_REMOVE_FEW_END\
            ++offset;\
        } else {\
            if (offset > 0)\
            {\
                memmove((uint8_t*)_vector->data + (i - offset) * _vector->size_of_element,\
                        (uint8_t*)_vector->data + i * _vector->size_of_element,\
                        _vector->size_of_element);\
            }\
        }\
    }

    // Функция удаления данных задана.
    if (_del_data != NULL)
    {
        C_VECTOR_REMOVE_FEW_BEGIN

        _del_data( (uint8_t*)_vector->data + i * _vector->size_of_element );

        C_VECTOR_REMOVE_FEW_END
    } else {
        // Функция удаления данных не задана
        C_VECTOR_REMOVE_FEW_BEGIN

        C_VECTOR_REMOVE_FEW_END
    }

    #undef C_VECTOR_REMOVE_FEW_BEGIN
    #undef C_VECTOR_REMOVE_FEW_END

    _vector->size -= offset;
    return offset;
}

// Очищает вектор от данных.
// Емкость не изменяется.
// В случае успешного очищения, возвращает > 0.
// Если в векторе нет элементов, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_clear(c_vector *const _vector,
                         void (*const _del_data)(void *const _data))
{
    if (_vector == NULL) return -1;
    if (_vector->size == 0) return 0;

    if (_del_data != NULL)
    {
        for (size_t i = 0; i < _vector->size; ++i)
        {
            _del_data((uint8_t*)_vector->data + i * _vector->size_of_element);
        }
    }
    _vector->size = 0;
    return 2;
}

// Ужимает вектор, перераспределяя память.
// Если size == 0, емкость так же приравнивается к 0.
// В случае успешного ужимания возвращает > 0.
// Если размер был равен емкости, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_compress(c_vector *const _vector)
{
    if (_vector == NULL) return -1;

    if (_vector->capacity == _vector->size) return 0;

    // Новая емкость.
    const size_t new_capacity = _vector->size;

    if (new_capacity > 0)
    {
        // Новый размер data. Проверка на арифметическое переполнение не нужна потому, что новая емкость
        // гарантировано меньше или равна старой.
        const size_t new_data_size = new_capacity * _vector->size_of_element;

        void *const new_data = realloc(_vector->data, new_data_size);
        if (new_data == NULL) return -2;

        _vector->data = new_data;
    } else {
        free(_vector->data);
        _vector->data = NULL;
    }
    _vector->capacity = new_capacity;
    return 2;
}

// Задание вектору новой емкости.
// Если новая емкость меньше текущего размера, лишние элементы обрезаются.
// В случае успешного изменения емкости возвращает > 0.
// Если новая емкость была равна старой, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_set_capacity(c_vector *const _vector,
                                const size_t _capacity,
                                void (*const _del_data)(void *const _data))
{
    if (_vector == NULL) return -1;
    if (_capacity == _vector->capacity) return 0;

    // Новая емкость больше старой.
    if (_capacity > _vector->capacity)
    {
        // Определим новый размер data.
        const size_t new_data_size = _capacity * _vector->size_of_element;
        if ( (new_data_size == 0) ||
             (new_data_size / _capacity != _vector->size_of_element) )
        {
             return -2;
        }

        // Попытаемся выделить память под новую data.
        void *const new_data = malloc(new_data_size);
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
                if (_del_data != NULL)
                {
                    for (size_t i = 0; i < _vector->size; ++i)
                    {
                        _del_data((uint8_t*)_vector->data + i * _vector->size_of_element);
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
            const size_t new_data_size = _capacity * _vector->size_of_element;
            // Проверка арифметического переполнения не нужна, потому что переполнение возникнуть не может.
            void *const new_data = malloc(new_data_size);
            if (new_data == NULL) return -4;
            memcpy(new_data, _vector->data, new_data_size);
            // Если новая емкость еще и меньше размера.
            if (_capacity < _vector->size)
            {
                if (_del_data != NULL)
                {
                    for (size_t i = _capacity; i < _vector->size; ++i)
                    {
                        _del_data((uint8_t*)_vector->data + i * _vector->size_of_element);
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

// Проходит по всем присутствующим в векторе элементам и выполняет над каждым элементом действие _action_data.
// В случае успеха возвращает > 0.
// Если вектор пуст, возвращает 0.
// В случае ошибки возвращает < 0.
ptrdiff_t c_vector_for_each(c_vector *const _vector,
                            void (*const _action_data)(void *const _data))
{
    if (_vector == NULL) return -1;
    if (_action_data == NULL) return -2;
    if (_vector->size == 0) return 0;
    for (size_t i = 0; i < _vector->size; ++i)
    {
        _action_data((uint8_t*)_vector->data + i * _vector->size_of_element);
    }
    return 1;
}

// Небезопасное обращение к элементу с заданным индексом.
// Никакие проверки не выполняются.
// Возвращает указатель на элемент.
void *c_vector_unsafe_at(const c_vector *const _vector,
                         const size_t _index)
{
    return (uint8_t*)_vector->data + _index * _vector->size_of_element;
}

// Возвращает размер вектора (кол-во элементов в векторе).
// В случае ошибки возвращает 0, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
// Так как функция может возвращать 0 и в случае успеха, и в случае ошибки, для детектирования ошибки
// перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
size_t c_vector_size(const c_vector *const _vector,
                     size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return 0;
    }

    return _vector->size;
}

// Возвращает емкость вектора (кол-во элементов, которое может быть вставленно в вектор).
// В слушае ошибки возвращает 0, и если _error != NULL, в заданное расположение помещается
// код причины ошибки (> 0).
// Так как функция может возвращать 0 и в случае успеха, и в случае ошибки, для детектирования ошибки
// перед вызовом функции необходимо поместить 0 в заданное расположение ошибки.
size_t c_vector_capacity(const c_vector *const _vector,
                         size_t *const _error)
{
    if (_vector == NULL)
    {
        error_set(_error, 1);
        return 0;
    }

    return _vector->capacity;
}
