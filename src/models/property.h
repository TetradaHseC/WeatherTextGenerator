#ifndef PROPERTY_H
#define PROPERTY_H

#define NOPW 17

struct StructProperty {
    char propertyType[20];
    short propertyValue;
};

typedef struct {
    struct StructProperty properties[NOPW];
} Forecast;

#endif
