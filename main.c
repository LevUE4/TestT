#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 3
#define COLS 10

// Определяем массив, представляющий клавиатуру QWERTY
const char keyboard[ROWS][COLS] = {
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ' '},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', ' ', ' ', ' '}
};

// Получаем координаты символа на клавиатуре
bool get_position(char key, int *row, int *col) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (keyboard[i][j] == key) {
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    return false;
}

// Проверка, являются ли две клавиши соседними
bool is_neighbor(char current, char next) {
    int row1, col1, row2, col2;
    
    if (!get_position(current, &row1, &col1) || !get_position(next, &row2, &col2)) {
#ifdef DEBUG
        printf("Символ '%c' или '%c' не найден на клавиатуре.\n", current, next);
#endif
        return false;
    }
    
    int row_diff = row1 - row2;
    int col_diff = col1 - col2;
    bool neighbor = (abs(row_diff) <= 1 && abs(col_diff) <= 1 &&
                    (row_diff == 0 || col_diff == 0 || row_diff == -col_diff));

#ifdef DEBUG
    printf("Проверка соседства: '%c' и '%c' -> %s\n", current, next, neighbor ? "соседи" : "не соседи");
#endif
    return neighbor;
}

// Проверка, можно ли набрать слово только с использованием смежных клавиш и допускается повтор символов
bool is_typable(const char *word) {
    for (int i = 1; word[i] != '\0'; ++i) {
        if (!is_neighbor(word[i - 1], word[i])) {
#ifdef DEBUG
            printf("Слово \"%s\" не типируемо из-за символов '%c' и '%c'\n", word, word[i - 1], word[i]);
#endif
            return false;
        }
    }
#ifdef DEBUG
    printf("Слово \"%s\" типируемо\n", word);
#endif
    return true;
}

// Основная функция для подсчета и печати "типируемых" слов
int count_typable_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Не удалось открыть файл %s\n", filename);
        return -1;
    }
    
    int count = 0;
    char word[100];
    
    while (fscanf(file, "%s", word) != EOF) {
#ifdef DEBUG
        printf("Проверяем слово: %s\n", word);
#endif
        if (is_typable(word)) {
#ifdef DEBUG
            printf("Типируемое слово: %s\n", word);
#endif
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        return 1;
    }
    
    int typable_words = count_typable_words(argv[1]);
    if (typable_words >= 0) {
        printf("Количество слов, которые можно набрать: %d\n", typable_words);
    }
    
    return 0;
}
