#include "dynamic_massiv.h"
#include <stdlib.h>

DMError dm_create(DynamicMassiv *arr, size_t initial_capacity) {
    if (arr == NULL) return DM_ERR_NULL_PTR;
    if (initial_capacity == 0) initial_capacity = 1;  // минимальная вместимость

    arr->data = (int *)malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        return DM_ERR_MEMORY;
    }
    arr->size = 0;
    arr->capacity = initial_capacity;
    return DM_OK;
}

void dm_destroy(DynamicMassiv *arr) {
    if (arr == NULL) return;
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

static DMError dm_ensure_capacity(DynamicMassiv *arr) {
    if (arr->size < arr->capacity) return DM_OK;

    size_t new_capacity = arr->capacity * 2;
    int *new_data = (int *)realloc(arr->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
        return DM_ERR_MEMORY;
    }
    arr->data = new_data;
    arr->capacity = new_capacity;
    return DM_OK;
}

DMError dm_push_back(DynamicMassiv *arr, int value) {
    if (arr == NULL) return DM_ERR_NULL_PTR;

    DMError err = dm_ensure_capacity(arr);
    if (err != DM_OK) return err;

    arr->data[arr->size] = value;
    arr->size++;
    return DM_OK;
}

DMError dm_insert(DynamicMassiv *arr, size_t pos, int value) {
    if (arr == NULL) return DM_ERR_NULL_PTR;
    // Позиция может быть равна size — это вставка в конец
    if (pos > arr->size) return DM_ERR_OUT_OF_RANGE;

    DMError err = dm_ensure_capacity(arr);
    if (err != DM_OK) return err;

    // Сдвиг хвоста с конца, чтобы не затереть данные
    for (size_t i = arr->size; i > pos; --i) {
        arr->data[i] = arr->data[i - 1];
    }
    arr->data[pos] = value;
    arr->size++;
    return DM_OK;
}

DMError dm_erase(DynamicMassiv *arr, size_t pos) {
    if (arr == NULL) return DM_ERR_NULL_PTR;
    // При удалении позиция должна быть строго меньше size
    if (pos >= arr->size) return DM_ERR_OUT_OF_RANGE;

    // Сдвигаем хвост влево с начала позиции
    for (size_t i = pos; i < arr->size - 1; ++i) {
        arr->data[i] = arr->data[i + 1];
    }
    arr->size--;
    return DM_OK;
}

DMError dm_get(const DynamicMassiv *arr, size_t index, int *out_value) {
    if (arr == NULL || out_value == NULL) return DM_ERR_NULL_PTR;
    if (index >= arr->size) return DM_ERR_OUT_OF_RANGE;
    *out_value = arr->data[index];
    return DM_OK;
}

DMError dm_set(DynamicMassiv *arr, size_t index, int value) {
    if (arr == NULL) return DM_ERR_NULL_PTR;
    if (index >= arr->size) return DM_ERR_OUT_OF_RANGE;
    arr->data[index] = value;
    return DM_OK;
}

size_t dm_size(const DynamicMassiv *arr) {
    return (arr != NULL) ? arr->size : 0;
}

size_t dm_capacity(const DynamicMassiv *arr) {
    return (arr != NULL) ? arr->capacity : 0;
}
