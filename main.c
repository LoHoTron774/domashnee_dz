#include <stdio.h>
#include "dynamic_massiv.h"
#include <windows.h>

static void print_status(const char *msg, size_t size, size_t cap) {
    printf("%s: size=%zu, capacity=%zu\n", msg, size, cap);
}

int main(void) {
    SetConsoleOutputCP(65001);
    DynamicMassiv arr;
    DMError err;
    int val;

    // 1. Создание с небольшой начальной вместимостью
    err = dm_create(&arr, 1);
    if (err != DM_OK) {
        printf("Ошибка создания массива\n");
        return 1;
    }

    print_status("После создания", dm_size(&arr), dm_capacity(&arr));

    // 2. Рост вместимости: 30 добавлений в конец — видно удвоение
    for (int i = 0; i < 30; ++i) {
        err = dm_push_back(&arr, i * 10);
        if (err != DM_OK) {
            printf("Ошибка push_back на шаге %d\n", i);
            break;
        }
        // Печатаем только когда вместимость изменилась (или просто каждый шаг — как удобнее)
        static size_t last_cap = 0;
        size_t cur_cap = dm_capacity(&arr);
        if (cur_cap != last_cap) {
            last_cap = cur_cap;
            print_status("Рост вместимости", dm_size(&arr), cur_cap);
        }
    }

    // 3. Пустой массив: проверка на пустом (у нас уже не пустой, сделаем отдельный тест)
    DynamicMassiv empty;
    dm_create(&empty, 1);
    printf("\nТест на пустом массиве:\n");
    err = dm_get(&empty, 0, &val);
    if (err == DM_ERR_OUT_OF_RANGE) {
        printf("Ожидаемая ошибка при чтении из пустого массива: DM_ERR_OUT_OF_RANGE\n");
    } else {
        printf("Неожиданный результат при чтении из пустого\n");
    }
    dm_destroy(&empty);

    // 4. Вставка в начало (позиция 0)
    printf("\nВставка в начало:\n");
    size_t old_size = dm_size(&arr);
    err = dm_insert(&arr, 0, -999);
    if (err == DM_OK) {
        print_status("После вставки в начало", dm_size(&arr), dm_capacity(&arr));
        err = dm_get(&arr, 0, &val);
        if (err == DM_OK) {
            printf("Первый элемент: %d (ожидается -999)\n", val);
        }
    }

    // 5. Удаление последнего элемента
    printf("\nУдаление последнего элемента:\n");
    old_size = dm_size(&arr);
    err = dm_erase(&arr, old_size - 1);
    if (err == DM_OK) {
        print_status("После удаления последнего", dm_size(&arr), dm_capacity(&arr));
    }

    // 6. Обращение за границу (чтение)
    printf("\nОбращение за границу:\n");
    size_t bad_index = dm_size(&arr) + 1;
    err = dm_get(&arr, bad_index, &val);
    if (err == DM_ERR_OUT_OF_RANGE) {
        printf("Корректно получена ошибка DM_ERR_OUT_OF_RANGE при чтении за границей\n");
    }

    // 7. Запись по индексу и чтение обратно
    printf("\nЗапись и чтение по индексу:\n");
    size_t idx = 5;
    if (idx < dm_size(&arr)) {
        dm_set(&arr, idx, 777);
        dm_get(&arr, idx, &val);
        printf("Элемент по индексу %zu: %d (ожидается 777)\n", idx, val);
    }

    // 8. Вставка ровно в позицию size (как push_back)
    printf("\nВставка по позиции == size:\n");
    size_t insert_pos = dm_size(&arr);
    dm_insert(&arr, insert_pos, 888);
    print_status("После вставки по позиции size", dm_size(&arr), dm_capacity(&arr));
    dm_get(&arr, dm_size(&arr) - 1, &val);
    printf("Последний элемент: %d (ожидается 888)\n", val);

    // Очистка
    dm_destroy(&arr);

    return 0;
}
