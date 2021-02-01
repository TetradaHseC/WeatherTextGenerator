#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include "../models/db_models.h"

int check();

// при новом запуске прошлые значения стираются!
struct Words GetAllWords();

// при новом запуске прошлые значения стираются!
struct Words GetWordsByProperties(int propc, Property *propv);

struct Words GetWordsByPartsOfSpeech(int partOfSpeech, int subpartOfSpeech, int propertyType, double propertyValue);

// после использования окончания - стереть оригинал
// при новом запуске прошлые значения стираются!
char * GetEnding(int wordId, int sex, int case_, int number);

// после использования окончания - стереть оригинал
// при новом запуске прошлые значения стираются!
char * GetWordWithEnding(int wordId, int sex, int case_, int number);

#endif DB_INTERFACE_H
