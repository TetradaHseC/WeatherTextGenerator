#ifndef PROPERTY_TYPES_H
#define PROPERTY_TYPES_H

typedef enum {
    Существительные=0,
    Прилагательные=1
} ЧастьРечи;

typedef enum {
    СВремени=0,
    СОсадки=1,
} ПодчастиСуществительных;

typedef enum {
    ПВремени=0,
    ПТемпературы=1,
    ПСилаВетра=2,
    ПДавления=3
} ПодчастиПрилагательных;

typedef enum {
    ВремяДня=0,
    Температура=1
} PropertyTypes;

typedef enum {// PropertyValue
    День = 0,
    Ночь = 1
} EВремяДня;

#endif//PROPERTY_TYPES_H