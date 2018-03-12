#include <stdio.h>
#include <stdlib.h>
#include "c_vector.h"

int main()
{
    // �������� ������.
    c_vector *vector = c_vector_create(1, 10);

    // ��������.
    printf("Source vector: \n");
    for (size_t i = 0; i < 10; ++i)
    {
        *( (uint8_t*) c_vector_push_back(vector) ) = i;

        printf("value[%Iu] = %Iu\n", i, i);
    }

    // ���������� ������ ��������� ��������.
    size_t indexes[9] = {8, 0, 10009090, 1, 8, 11, 8, 7, 1};

    // ���������  ���.
    printf("indexes:\n");
    for (size_t i = 0; i < 9; ++i)
    {
        printf("%Iu\n",(size_t)indexes[i]);
    }

    // ������ �������� � ���������� ���������.
    c_vector_erase_few(vector, indexes, 9, NULL);

    // ������� ���������� ������� ����� ��������.
    printf("Not source vector: \n");
    for (size_t i = 0; i < vector->size; ++i)
    {
        printf("vector[%Iu] = %Iu\n", i, (size_t)(*( (uint8_t*) c_vector_at(vector, i) )) );
    }

    // ������� ���������� ��������.
    printf("indexes: \n");
    for (size_t i = 0; i < 9; ++i)
    {
        printf("%Iu\n",(size_t)indexes[i]);
    }

    getchar();
    return 0;
}
