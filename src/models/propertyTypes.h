#ifndef PROPERTY_TYPES_H
#define PROPERTY_TYPES_H

typedef enum {
    Существительные,
    Прилагательные
} ЧастьРечи;

typedef enum {
    СВремени=0,
    СТемпературы=1,
} ПодчастиСуществительных;

typedef enum {
    День = 0,
    Ночь = 1
} EВремяДня;

typedef enum {
    ПВремени=0,
    ПТемпературы=1,
    ПСилаВетра=2
} ПодчастиПрилагательных;

typedef enum {
    ВремяДня,
    Температура
} PropertyTypes;

#endif//PROPERTY_TYPES_H