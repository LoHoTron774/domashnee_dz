#ifndef DYNAMIC_MASSIV_H
#define DYNAMIC_MASSIV_H

#include <stddef.h>

typedef enum {
    DM_OK = 0,
    DM_ERR_NULL_PTR = 1,
    DM_ERR_OUT_OF_RANGE = 2,
    DM_ERR_MEMORY = 3
} DMError;

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicMassiv;

// Создание: начальная вместимость, возвращает код ошибки, массив через параметр-указатель
DMError dm_create(DynamicMassiv *arr, size_t initial_capacity);

// Уничтожение
void dm_destroy(DynamicMassiv *arr);

// Добавление в конец
DMError dm_push_back(DynamicMassiv *arr, int value);

// Вставка по позиции: позиция может быть равна size (в конец)
DMError dm_insert(DynamicMassiv *arr, size_t pos, int value);

// Удаление по позиции: позиция должна быть < size
DMError dm_erase(DynamicMassiv *arr, size_t pos);

// Чтение по индексу: результат в *out_value, признак ошибки — возвращаемое значение
DMError dm_get(const DynamicMassiv *arr, size_t index, int *out_value);

// Запись по индексу
DMError dm_set(DynamicMassiv *arr, size_t index, int value);

// Получение размера и вместимости
size_t dm_size(const DynamicMassiv *arr);
size_t dm_capacity(const DynamicMassiv *arr);

#endif // DYNAMIC_MASSIV_H
