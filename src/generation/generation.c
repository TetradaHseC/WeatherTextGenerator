#include "generation.h"
#include "../db/db_interface.h"
#include "../models/propertyTypes.h"
#include "../models/analprop.h"
#include <locale.h>
#include <string.h>
#include <stdlib.h>

char *GetRandomWordBy(int partOfSpeech, int subpartOfSpeech, int propertyType, double propertyValue) {
    char *temps;
    char *word = calloc(20, sizeof(char));
    struct Words daytime=GetWordsByPartsOfSpeech(partOfSpeech, subpartOfSpeech, propertyType, propertyValue);
    int rand_number= rand() % daytime.count;
    Word curWord = daytime.words[rand_number];
    temps = GetWordWithEnding(curWord.id, -1, 0, 0);
    strcpy(word, temps);
    free(temps);

    return word;
}

char *GenerateTextForProperties(int propc, Property *propv, Forecast forecast) {
    char *temps;
    int storm_flag=0;

    setlocale(LC_ALL, "RUS");

    char *result = calloc(400, sizeof(char));
    char space[] = " ";
    struct Words daytime=GetWordsByPartsOfSpeech(Существительные, СВремени, ВремяДня, День);
    int rand_number= rand() % daytime.count;
    Word curWord = daytime.words[rand_number];
    temps = GetWordWithEnding(curWord.id, -1, 0, 0);
    char *wordValue = calloc(strlen(temps), sizeof(char));
    strcpy(wordValue, temps);
    free(temps);

    strcat(result,wordValue);
    free(wordValue);
    strcat(result,space);

    strcat(result, "будет");
    strcat(result, space);

    struct Words temperature=GetWordsByPartsOfSpeech(Прилагательные, ПТемпературы, Температура, propv[DayTempDeviation].propertyValue);
    rand_number=rand()%temperature.count;
    curWord=temperature.words[rand_number];
    temps = GetWordWithEnding(curWord.id, -1, 0, 0);
    wordValue = calloc(strlen(temps), sizeof(char));
    strcpy(wordValue, temps);
    free(temps);

    strcat(result,wordValue);
    free(wordValue);
    strcat(result,space);
    if(propv[DayTempDeviation].propertyValue!=0){
    strcat(result,"обычного");
    strcat(result,space);

    else{
        strcat(result,"как обычно");
        strcat(result,space);


    }



    if (forecast.properties[Direction].propertyValue!=-1) {
        if (forecast.properties[AbsWind].propertyValue<=8) {
            strcat(result, "с");
            strcat(result, space);
            if (forecast.properties[Direction].propertyValue == 0) {
                strcat(result, "северным");
                strcat(result, space);
            }
            if (forecast.properties[Direction].propertyValue == 1) {
                strcat(result, "северо-восточным");
                strcat(result, space);
            }
            if (forecast.properties[Direction].propertyValue == 2) {
                strcat(result, "восточным");
                strcat(result, space);
            }
            if (forecast.properties[Direction].propertyValue == 3) {
                strcat(result, "юго-восточным");
                strcat(result, space);
            }
            if (forecast.properties[Direction].propertyValue == 4) {
                strcat(result, "южным");
                strcat(result, space);
            }

            if (forecast.properties[Direction].propertyValue == 5) {
                strcat(result, "юго-западным");
                strcat(result, space);
            }
            if (forecast.properties[Direction].propertyValue == 6) {
                strcat(result, "западным");
                strcat(result, space);
            }
            if (forecast.properties[Direction].propertyValue == 7) {
                strcat(result, "северо-западным");
                strcat(result, space);
            }
            temps = GetRandomWordBy(Прилагательные, ПСилаВетра, propv[AbsWind].propertyType, propv[AbsWind].propertyValue);
            strcat(result, temps);
            free(temps);
            strcat(result, space);
            strcat(result,"ветром.");
            strcat(result, space);
            if(propv[DayTempDeviation].propertyValue>0){
                strcat(result,"советуем одеваться чуть легче");
                strcat(result,space);
            }
            else if(propv[DayTempDeviation].propertyValue<0){
                strcat(result,"советуем одеваться чуть теплее");
                strcat(result,space);
            }


        }
        else{
            strcat(result,".");
            strcat(result, space);
            strcat(result, "Ожидается шторм, не укрывайтесь и не паркуйте автотранспорт под деревьями и шаткими конструкциями.");
            strcat(result, space);

        }
    }
    else{
        strcat(result, "без ветра");
        strcat(result, space);
    }
    if(forecast.properties[Fallout].propertyValue!=0){

    }




   return result;
}
