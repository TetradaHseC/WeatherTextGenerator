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
    int sex; // пол
    int case_; // падеж
    int propertiesCount;
    Property properties[];
} Word;

typedef struct {
    int id;
    char *string;
} WordValue;

#endif // DB_MODELS_H
