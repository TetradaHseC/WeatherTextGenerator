#include "../models/db_models.h"

int check();

// при новом запуске прошлые значения стираются!
struct Words GetAllWords();

// при новом запуске прошлые значения стираются!
struct Words GetWordsByProperties(int propc, Property *propv);

// после использования окончания - стереть оригинал
// при новом запуске прошлые значения стираются!
char * GetEnding(int wordId, int sex, int case_, int number);

// после использования окончания - стереть оригинал
// при новом запуске прошлые значения стираются!
char * GetWordWithEnding(int wordId, int sex, int case_, int number);
