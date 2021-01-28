#ifndef DB_MODELS_H
#define DB_MODELS_H

typedef struct {
    int propertyType;
    double propertyValue;
} Property;

typedef struct {
    int id;
    int partOfSpeech;
    int subpartOfSpeech;
    int o_sex; // пол
    int o_number;
    int propertiesCount;
    Property properties[];
} Word;

typedef struct {
    int id;
    char *string;
} WordValue;

struct Words { int size, count; Word *words; };

#endif // DB_MODELS_H
