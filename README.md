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

### Вставка в конец вектора 10^8 элементов (типа size_t), начальная емкость вектора 1:

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

### Удаление произвольного элемента вектора (типа size_t), 10^5 удалений:

**std::vector:**

```c++
t1 = clock();
for (size_t i = 0; i < COUNT - 1; ++i)
{
	const size_t index = rand() % vector.size();
	vector.erase(vector.begin() + index);
}
t2 = clock();
cout << "Time: " << t2 - t1 << " ms." << endl;
```

~ 1285 мс.

**c_vector:** 

```c++
t1 = clock();
for (size_t i = 0; i < COUNT - 1; ++i)
{
	const size_t index = rand() % vector->size;
	c_vector_erase(vector, index, NULL);
}
t2 = clock();
printf("Time: %Iu ms.\n", t2 - t1);
```

~ 1270 мс.

### Сложение всех элементов вектора (типа size_t) с использованием .at(), размер вектора 10^8:

**std::vector:** 

```c++
size_t sum = 0;
t1 = clock();
for (size_t i = 0; i < COUNT; ++i)
{
	sum += vector.at(i);
}
t2 = clock();
cout << "Time: " << t2 - t1 << " ms. sum: " << sum << endl;
```

~ 128 мс.

**c_vector:**

```c++
size_t sum = 0;
t1 = clock();
for (size_t i = 0; i < COUNT; ++i)
{
	sum += *( (size_t*) c_vector_at(vector, i) );
}
t2 = clock();
printf("Time: %Iu ms. sum: \n", t2 - t1, sum);
```

~ 173 мс.

### Сложение всех элементов вектора (типа size_t) с использованием цикла, размер вектора 10^8:

**std::vector (range-based):**

```c++
size_t sum = 0;
t1 = clock();
for (auto &e : vector)
{
	sum += e;
}
t2 = clock();
cout << "Time: " << t2 - t1 << " ms. sum: " << sum << endl;
``` 

~ 105 мс.

**c_vector (for_each):**

```c++
size_t sum = 0;
void sum_func(void *_data)
{
	sum += *((size_t*)_data);
}
t1 = clock();
c_vector_for_each(vector, sum_func);
t2 = clock();
printf("Time: %Iu ms. sum: \n", t2 - t1, sum);
```

~ 82 мс.

### Вставка в произвольные позиции 10^5 элементов (типа size_t), начальная емкость вектора 1:
**std::vector:**

```c++
vector.push_back(0);
t1 = clock();
for (size_t i = 0; i < COUNT; ++i)
{
	const size_t index = rand() % vector.size();
	vector.insert(vector.begin() + index, i);
}
t2 = clock();
cout << "Time: " << t2 - t1 << " ms." << endl;
```

~ 2417 мс.

**c_vector:** 

```c++
c_vector_push_back(vector);
t1 = clock();
for (size_t i = 0; i < COUNT; ++i)
{
	const size_t index = rand() % vector->size;
	c_vector_insert(vector, index);
	*( (size_t*) c_vector_at(vector, index) ) = i;
}
t2 = clock();
printf("Time: %Iu ms.\n", t2 - t1);
```

~ 2420 мс.
