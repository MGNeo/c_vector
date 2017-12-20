# Назначение c_vector

Контейнер **c_vector** реализует динамический массив.

## Пример использования c_vector

Создание вектора, размер элемента которого = sizeof(size_t), емкость = 1:

```c++
c_vector *vector = c_vector_create(sizeof(size_t), 1);
```

Вставка в произвольную позицию (в начало) неинициализированного элемента:

```c++
const size_t index = 0;
c_vector_insert(vector, index);
```

Безопасное обращение к элементу с заданным индексом:

```c++
const size_t index = 0;
*( (size_t*) c_vector_at(vector, index) ) = 100500;
```

Небезопасное обращение к элементу с заданным индексом (никакие проверки не выполняются):

```c++
const size_t index = 0;
*( (size_t*) c_vector_unsafe_at(vector, index) ) = 100500;
```

Клонирование вектора:

```c++
c_vector *clone = c_vector_clone(vector);
```
Удаление элемента с заданным индексом:

```c++
const size_t index = 0;
c_vector_erase(vector, index, NULL);
```

Печать всех элементов вектора:

```c++
size_t i = 0;
void print_func(void *const _data)
{
  pritnf("vector[%Iu] = %Iu\n", i++, *( (size_t*) _data));
}
c_vector_for_each(vector, print_func);
```

Удаление вектора:

```c++
c_vector_delete(vector, NULL);
```

## Сравнение прозводительности **c_vector** и **std::vector** (**Visual Studio 2013**)

### Конфигурация системы и железа:

**Процессор:** *Intel Core i3-6006U*

**Оперативная память:** *6 ГБ*

**ОС:** *Windows 10, Корпоративная 2016 с долгосрочным обслуживанием*

**Сборка в:** *VS 2013 для x64* 

### Вставка в конец вектора 10^8 элементов типа size_t, начальная емкость вектора 1:

**std::vector:** 

```c++
const size_t COUNT = 1E8;
size_t t1, t2;

vector<size_t> vector;
vector.reserve(1);

t1 = clock();
for (size_t i = 0; i < COUNT; ++i)
{
  vector.push_back(i);
}
t2 = clock();
cout << "Time: " << t2 - t1 << " ms." << endl;
```

~ 1867 мс., ~ 764.5 МБ

**c_vector:**

```c++
const size_t COUNT = 1E8;
size_t t1, t2;
c_vector *vector = c_vector_create(sizeof(size_t), 1);
	
t1 = clock();
for (size_t i = 0; i < COUNT; ++i)
{
	c_vector_push_back(vector);
	*( (size_t*) c_vector_back(vector) ) = i;
}
t2 = clock();
printf("Time: %Iu ms.\n", t2 - t1);
```

~ 2056 мс., ~ 764 МБ

### Удаление произвольного элемента типа size_t, 10^5 удалений:

**std::vector** ~ 1285 мс.

**c_vector** ~ 1270 мс.

### Сложение всех элементов типа size_t вектора с использованием .at(), размер вектора 10^8:

**std::vector** ~ 101 мс.

**c_vector** ~ 173 мс.

### Сложение всех элементов вектора с использованием цикла:

**std::vector (range-based)** ~ 105 мс.

**c_vector (for_each)** ~ 82 мс.

### Вставка в произвольную позицию 10^5 элементов типа size_t, начальная емкость вектора 1:
**std::vector** ~ 2417 мс.

**c_vector** ~ 2420 мс.
