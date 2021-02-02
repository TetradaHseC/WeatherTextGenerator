#ifndef CASES_H
#define CASES_H

typedef enum {
    Мужской=0,
    Женский,
    Средний
} Sexes;

typedef enum {
    Именительный=0,
    Родительный,
    Дательный,
    Винительный,
    Творительный,
    Предложный
} Cases;

typedef enum {
    Единственное=0,
    Множественное
} Number;

#endif
