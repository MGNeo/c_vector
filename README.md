# c_vector

Сравнение прозводительности **c_vector** и **std::vector** (**Visual Studio 2013**).

## Конфигурация системы и железа:

**Процессор:** *Intel Core i3-6006U*

**Оперативная память:** *6 ГБ*

**ОС:** *Windows 10, Корпоративная 2016 с долгосрочным обслуживанием*

**Сборка в:** *VS 2013 для x64* 

## Вставка в конец вектора 10^8 элементов типа size_t, начальная емкость вектора 1:

**std::vector** ~ 1867 мс., ~ 764.5 МБ

**c_vector** ~ 2056 мс., ~ 764 МБ

## Удаление произвольного элемента типа size_t, 10^5 удалений:

**std::vector** ~ 1285 мс.

**c_vector** ~ 1270 мс.

## Сложение всех элементов типа size_t вектора с использованием .at(), размер вектора 10^8:

**std::vector** ~ 101 мс.

**c_vector** ~ 173 мс.

## Сложение всех элементов вектора с использованием цикла:

**std::vector (range-based)** ~ 105 мс.

**c_vector (for_each)** ~ 82 мс.

## Вставка в произвольную позицию 10^5 элементов типа size_t, начальная емкость вектора 1:
**std::vector** ~ 2417 мс.

**c_vector** ~ 2420 мс.
