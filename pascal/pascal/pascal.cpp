// NickGramma.cpp
// основная функция

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#include "lexan.h"
#include "sall.h"

// интерпретация входа
int interpretate(FILE* input, FILE* output, FILE* result) {
    // программные исключения
    try {
        // синтаксический разбор
        return (new syntaxan (new lexan(input, output), output, result))->parse();
    }
    catch (texception *e) {
        printf("\nException raised - ");
        fprintf(result, "\nException raised - ");
        e->get_info(result);
    }
    catch (...) {
        printf("\nUnexpected exception raised\n\n");
        fprintf(result, "\nUnexpected exception raised\n\n");
    }
    return 0; // неудача
}

// главная функция
int main()
{
    // открываем входной поток
    FILE* input = fopen("..\\text.txt", "rt");
    // открываем выходной поток (анализ)
    FILE* output = fopen("..\\output.txt", "w");
    // открываем выходной поток (результат)
    FILE* result = fopen("..\\result.txt", "w");
    setlocale(LC_ALL, ".1251");
    if (!input) {
        // проблемы с входным файлом
        printf("\nException raised - main: error while open input file\n\n");
        fprintf(result, "\nException raised - main: error while open input file\n\n");
        return 0;
    }
    // выполнение: 1 - удача, 0 - неудача
    int i = interpretate(input, output, result);
    fclose(input);
    fclose(output);
    fclose(result);
    return i;
}