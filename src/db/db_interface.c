#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err34-c"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "db_interface.h"

#define DB_LOCATION "../res/db.db"

// region sql commands
#define SELECT_ALL \
    "SELECT "\
        "words.id, part_of_speech, subpart_of_speech, o_sex, o_number "\
    "FROM "\
        "words, parts_of_speech, own_propertires, properties "\
    "WHERE "\
        "words.id == parts_of_speech.id and words.id == own_propertires.id and words.id == properties.id"

#define AND_PROPERTY_EQUALS " and ((properties.type == %d and properties.value == %f) or properties.type != %d)"

#define GET_ALL_PROPERTIES "SELECT * FROM properties"
// endregion

void PrintWord(Word word);

int check() {
    Property property1 = { 1, 1.0 };
    Property property2 = { 3, -6.0 };
    Property arr[] = {property1, property2};
    struct Words words = GetWordsByProperties(2, arr);
    for (int i = 0; i < words.count; ++i) {
        PrintWord(words.words[i]);
    }

    printf("%s", GetWordWithEnding(1, 1, 1, 1));

    return 0;
}

void PrintWord(Word word) {
    printf("{ words.id:%d, part_of_speech:%d, subpart_of_speech:%d, o_sex:%d, o_number:%d }\n", word.id, word.partOfSpeech, word.subpartOfSpeech, word.o_sex, word.o_number);
}

struct Words wordsToReturn = { 0, 0, 0 };

void ClearWordsToReturn() {
    wordsToReturn.size = 0;
    wordsToReturn.count = 0;
    free(wordsToReturn.words);
}

// region get words
// region callbacks
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedLocalVariable"
int AddPropertyToWord(void *data, int argc, char **argv, char **azColName) {
    int id;

    Property newProperty;

    sscanf(argv[0], "%d", &id);
    sscanf(argv[1], "%d", &newProperty.propertyType);
    sscanf(argv[2], "%lf", &newProperty.propertyValue);

    Word *pword = NULL;

    if (id > wordsToReturn.count || wordsToReturn.words[id].id != id) {
        for (int i = 0; i < wordsToReturn.count; ++i) {
            if (wordsToReturn.words[i].id == id) {
                pword = &wordsToReturn.words[i];
                break;
            }
        }
    } else {
        pword = &wordsToReturn.words[id];
    }

    if (pword->propertiesSize == pword->propertiesCount) {
        Property *properties = calloc(pword->propertiesSize * 2 + 1, sizeof(Property));

        for (int i = 0; i < pword->propertiesCount; ++i) {
            properties[i] = pword->properties[i];
        }
        free(pword->properties);
        pword->properties = properties;
        pword->propertiesSize = pword->propertiesSize * 2 + 1;
    }
    pword->properties[pword->propertiesCount++] = newProperty;

    return 0;
}

int CallbackGettingWordsArray(void *data, int argc, char **argv, char **azColName) {
#define INT(variable, index) sscanf(argv[index], "%d", &variable)
#define NULLABLE_INT(variable, index, def) if (argv[index] != NULL) INT(variable, index); else variable = def
    if (wordsToReturn.size == wordsToReturn.count) {
        Word *words = calloc(wordsToReturn.size * 2 + 1, sizeof(Word));

        for (int i = 0; i < wordsToReturn.count; ++i) {
            words[i] = wordsToReturn.words[i];
        }
        free(wordsToReturn.words);
        wordsToReturn.words = words;
        wordsToReturn.size = wordsToReturn.size * 2 + 1;
    }

    Word word = {};
    // SELECT words.id, part_of_speech, subpart_of_speech, o_sex, o_number
    INT(word.id, 0);
    INT(word.partOfSpeech, 1);
    INT(word.subpartOfSpeech, 2);
    NULLABLE_INT(word.o_sex, 3, -1);
    NULLABLE_INT(word.o_number, 4, -1);

    wordsToReturn.words[wordsToReturn.count++] = word;

    return 0;
}
#pragma clang diagnostic pop
// endregion

// region requests
struct Words GetAllWords() {
    struct Words NoWords = { 0, 0, };
    sqlite3 *connection;
    char *errMsg = 0;
    int resCode;

    resCode = sqlite3_open(DB_LOCATION, &connection);

    if (resCode)
        return NoWords;

    ClearWordsToReturn();
    resCode = sqlite3_exec(connection, SELECT_ALL, CallbackGettingWordsArray, 0, &errMsg);

    if (resCode) {
        sqlite3_close(connection);
        return NoWords;
    }

    resCode = sqlite3_exec(connection, GET_ALL_PROPERTIES, AddPropertyToWord, 0, &errMsg);

    sqlite3_close(connection);

    if (resCode)
        return NoWords;

    return wordsToReturn;
}

struct Words GetWordsByProperties(int propc, Property *propv) {
    struct Words NoWords = { 0, 0, };
    sqlite3 *connection;
    char *errMsg = 0;
    int resCode;

    resCode = sqlite3_open(DB_LOCATION, &connection);

    if (resCode)
        return NoWords;

    ClearWordsToReturn();

    char command[800] = { 0 };
    sprintf(command, "%s", SELECT_ALL);

    for (int i = 0; i < propc; ++i) {
        sprintf(command+strlen(command),
                AND_PROPERTY_EQUALS,
                propv[i].propertyType, propv[i].propertyValue, propv[i].propertyType);
    }

    resCode = sqlite3_exec(connection, command, CallbackGettingWordsArray, 0, &errMsg);

    if (resCode) {
        sqlite3_close(connection);
        return NoWords;
    }

    memset(command, 0, sizeof(char) * 800);
    sprintf(command, "%s WHERE 1 == 1", GET_ALL_PROPERTIES);

    for (int i = 0; i < propc; ++i) {
        sprintf(command + strlen(command),
                AND_PROPERTY_EQUALS,
                propv[i].propertyType, propv[i].propertyValue, propv[i].propertyType);
    }

    resCode = sqlite3_exec(connection, command, AddPropertyToWord, 0, &errMsg);

    sqlite3_close(connection);

    if (resCode)
        return NoWords;

    return wordsToReturn;
}
// endregion
// endregion

// region get ending
char * endingToReturn = NULL;

int CallbackGetEnding(void *data, int argc, char **argv, char **azColName) {
    free(endingToReturn);
    endingToReturn = calloc(strlen(argv[0]), sizeof(char));
    sscanf(endingToReturn, "%s", argv[0]);

    return 0;
}

char * GetEnding(int wordId, int sex, int case_, int number) {
    sqlite3 *connection;
    char *errMsg = 0;
    int resCode;

    resCode = sqlite3_open(DB_LOCATION, &connection);

    if (resCode) return NULL;

    char command[101]; // 88+12 (88 constant symbols + 6 symbols for word.id + 2 symbols for sex, case, number) + \0
    sprintf(command,
            "SELECT ending FROM word_variativity WHERE id == %d AND sex == %d AND case == %d AND number == %d",
            wordId, sex, case_, number);

    resCode = sqlite3_exec(connection, command, CallbackGetEnding, 0, &errMsg);

    sqlite3_close(connection);

    if (resCode) return NULL;

    return endingToReturn;
}
// endregion

// region get ending
char *wordWithEndingToReturn = NULL;

int CallbackGetWordWithEnding(void *data, int argc, char **argv, char **azColName) {
    free(wordWithEndingToReturn);
    wordWithEndingToReturn = calloc(strlen(argv[0]) + strlen(argv[1]) + 1, sizeof(char));
    sprintf(wordWithEndingToReturn, "%s%s", argv[0], argv[1]); // word, ending

    return 0;
}

char * GetWordWithEnding(int wordId, int sex, int case_, int number) {
    sqlite3 *connection;
    char *errMsg = 0;
    int resCode;

    resCode = sqlite3_open(DB_LOCATION, &connection);

    if (resCode) return NULL;

    // select[20] + from[32] + where[83] + 2*id[4] + sex[2] + case[2] + number[2] + \0
    char commandGetWordAndEnding[149];
    sprintf(commandGetWordAndEnding,
            "SELECT word, ending "
            "FROM words, word_variativity wa "
            "WHERE words.id == %d AND wa.id == %d AND wa.sex == %d AND wa.'case' == %d AND wa.number == %d",
            wordId, wordId, sex, case_, number);

    resCode = sqlite3_exec(connection, commandGetWordAndEnding, CallbackGetWordWithEnding, 0, &errMsg);

    sqlite3_close(connection);

    if (resCode) return NULL;

    return wordWithEndingToReturn;
}
// endregion

#pragma clang diagnostic pop
