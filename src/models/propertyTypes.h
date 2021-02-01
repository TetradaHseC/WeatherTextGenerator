#ifndef PROPERTY_TYPES_H
#define PROPERTY_TYPES_H

typedef enum {
    Существительные=0,
    Прилагательные=1
} ЧастьРечи;

typedef enum {
    СВремени=0,
    СТемпературы=1,
    СОсадки=2,
} ПодчастиСуществительных;

typedef enum {
    День = 0,
    Ночь = 1
} EВремяДня;

typedef enum {
    ПВремени=0,
    ПТемпературы=1,
    ПСилаВетра=2,
    ПНарпавлениеВетра=3,
    ПДавления=4
} ПодчастиПрилагательных;

typedef enum {
    ВремяДня=0,
    Температура=1
} PropertyTypes;

#endif//PROPERTY_TYPES_H