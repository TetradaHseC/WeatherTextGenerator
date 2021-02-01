#ifndef FORECAST_H
#define FORECAST_H
// region FORECAST_H

#include "../models/forecastprop.h"
#include "../models/property.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <assert.h>

#define NOF 10 // Number of fields (in string)
#define elif else if
#define SIZE 255

#define scase(case_) if (strcmp(SWITCH_STRING_ARGUMENT, case_) == 0)

typedef struct {
    struct StructProperty properties[NOPW];
} Forecast;

char** str_split(char* a_str, const char a_delim, size_t *pcount)   // украдено отсюда
{                                                   // https://coderoad.ru/9210528/Split-строка-с-разделителями-в-C
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        *(result + idx) = 0;
    }

    if (pcount != NULL)
        *pcount = count;

    return result;
}

char** ReadFile(FILE* file, size_t *pcount) {
    char **text = calloc(SIZE, sizeof(char *));
    char *row = calloc(SIZE, sizeof(char));

    int i = 0; // счетчик строк
    while(fgets(row, SIZE, file) != NULL)
    {
        text[i] = row;
        i++;
        row = calloc(SIZE, sizeof(char));
    }
    free(row);

    *pcount = i;

    return text;
}

Forecast ParseForecast(char* str) {
    char** strProps = str_split(str, ';', NULL);
    Forecast forecast;

    char** date = str_split(strProps[0], '.', NULL);
    int i = 0;
    for (; i < 3; i++) {
        forecast.properties[i].propertyValue = atoi(date[i]);
    }

    for (int j = 1; j < 4; j++) {
        char** tempsStr = str_split(strProps[j], '.', NULL);
        forecast.properties[i++].propertyValue = atoi(tempsStr[0]);
        forecast.properties[i++].propertyValue = atoi(tempsStr[1]);
    }

    int fallout = 0;
    char* falloutStr = strProps[4];
#define SWITCH_STRING_ARGUMENT falloutStr
    scase("дождь") fallout = 1;
    scase("ливень") fallout = 2;
    scase("снег") fallout = 3;
#undef SWITCH_STRING_ARGUMENT

    forecast.properties[i++].propertyValue = fallout;

    size_t checkRange = 0;
    char** wStr = str_split(strProps[5], '-', &checkRange);
    forecast.properties[i++].propertyValue = atoi(wStr[0]);
    forecast.properties[i++].propertyValue = atoi(wStr[wStr[1] != NULL]);

    int direction=-1;
    char *directionStr = strProps[6];
#define SWITCH_STRING_ARGUMENT directionStr
    scase("С")
        direction = 0;
    scase("С-В")
        direction = 1;
    scase( "В")
        direction = 2;
    scase("Ю-В")
        direction = 3;
    scase("Ю")
        direction = 4;
    scase("Ю-З")
        direction = 5;
    scase("З")
        direction = 6;
    scase("С-З")
        direction = 7;
#undef SWITCH_STRING_ARGUMENT

    forecast.properties[i++].propertyValue = direction;

    char** impStr = str_split(strProps[7], '-', NULL);
    forecast.properties[i++].propertyValue = atoi(wStr[0]);
    forecast.properties[i++].propertyValue = atoi(wStr[wStr[1] != NULL]);

    forecast.properties[i++].propertyValue = atoi(strProps[8]);

    int event = 0;
    char* eventStr = strProps[9];
    if (strcmp(falloutStr,"туман")==0) {
        event = 1;
    } elif (strcmp(falloutStr,"гололедица")==0) {
        event = 2;
    }elif (strcmp(falloutStr,"гроза")==0){
        event = 3;
    }
    elif (strcmp(falloutStr,"метель")==0){
        event = 4;
    }
    forecast.properties[i++].propertyValue = event;
    assert(i == NOPW);

    return forecast;
}

//endregion
#endif //FORECAST_H
