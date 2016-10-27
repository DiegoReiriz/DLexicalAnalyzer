
#include <stdio.h>

#ifndef ANALIZADORLEXICO_ERRORS_H
#define ANALIZADORLEXICO_ERRORS_H

enum errors {
    ERROR_FOUND_NEW_LINE_ON_STRING,
    ERROR_LEXEME_OVERFLOW,
    ERROR_UNKNOW_ELEMENT};

void showError(enum errors code, int line);

#endif //ANALIZADORLEXICO_ERRORS_H
