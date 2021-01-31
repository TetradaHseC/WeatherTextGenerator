#include <forecastprop.h>
#include <property.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <assert.h>

#define NOF 10 // Number of fields (in string)
#define elif else if
#define SIZE 255

struct Forecast {
    struct Property properties[NOPW];
};

/*

char** Split(char* str, char key) {
    char** props = (char**)malloc(NOPW * 10 * (sizeof(char)));
    for (int i = 0; i < NOPW; i++) {
        for (int j = 0; i < 10; i++) {
            props[i][j] = 0;
        }
    }

    int i = 0;
    int j = 0;
    while (str[i] != '/n') {
        props = 
        if ()
    }
}

*/

char** str_split(char* a_str, const char a_delim)   // украдено отсюда 
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
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char** ReadFile(FILE* file) {
    char text[SIZE*SIZE];
    fgets(text , SIZE*SIZE, file );

    return str_split(text, '\n');
}

struct Forecast ParseForecast(char* str) {
    char** strProps = str_split(str, ';');
    struct Forecast forecast;

    char** date = str_split(strProps[0], '.');
    int i = 0;
    for (; i < 3; i++) {
        forecast.properties[i].propertyValue = atoi(date[i]);
    }

    for (int j = 1; j < 4; j++) {
        char** tempsStr = str_split(strProps[j], '.');
        forecast.properties[i++].propertyValue = atoi(tempsStr[0]);
        forecast.properties[i++].propertyValue = atoi(tempsStr[2]);
    }

    int fallout;
    char* falloutStr = strProps[4];
    if (falloutStr == "нет") {
        fallout = 0;
    } elif(falloutStr == "дождь") {
        fallout = 1;
    } elif(falloutStr == "ливень") {
        fallout = 2;
    } elif(falloutStr == "снег") {
        fallout = 3;
    } else {
        fallout = 4;
    }

    forecast.properties[i++].propertyValue = fallout;

    char** wStr = str_split(strProps[5], '-');
    forecast.properties[i++].propertyValue = atoi(wStr[0]);
    forecast.properties[i++].propertyValue = atoi(wStr[1]);

    int direction;
    char* directionStr = strProps[6];
    if (falloutStr == "С") {
        direction = 0;
    } elif(falloutStr == "С-В") {
        direction = 1;
    } elif(falloutStr == "В") {
        direction = 2;
    } elif(falloutStr == "Ю-В") {
        direction = 3;
    } elif(falloutStr == "Ю") {
        direction = 4;
    } elif(falloutStr == "Ю-З") {
        direction = 5;
    } elif(falloutStr == "З") {
        direction = 6;
    } elif(falloutStr == "С-З") {
        direction = 7;
    }
    forecast.properties[i++].propertyValue = direction;

    char** impStr = str_split(strProps[7], '-');
    forecast.properties[i++].propertyValue = atoi(wStr[0]);
    forecast.properties[i++].propertyValue = atoi(wStr[1]);

    forecast.properties[i++].propertyValue = atoi(strProps[8]);

    int event = 0;
    char* eventStr = strProps[9];
    if (falloutStr == "туман") {
        event = 1;
    } elif(falloutStr == "гололедица") {
        event = 2;
    } 
    forecast.properties[i++].propertyValue = event;
    

    return forecast;
}