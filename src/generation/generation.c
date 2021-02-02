
#include "generation.h"
#include "../db/db_interface.h"
#include "../models/propertyTypes.h"
#include "../models/analprop.h"
#include "../models/cases.h"
#include "../models/forecastprop.h"
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NO_ENDING -1, -1, -1
#define space " "
#define dospace(dest) strcat(dest, " ")


char *GetRandomWordBy(int partOfSpeech, int subpartOfSpeech, int propertyType, double propertyValue, int sex, int case_, int number) {
    char *temps;
    char *word = calloc(20, sizeof(char));
    struct Words daytime=GetWordsByPartsOfSpeech(partOfSpeech, subpartOfSpeech, propertyType, propertyValue);
    int rand_number= rand() % daytime.count;
    Word curWord = daytime.words[rand_number];
    temps = GetWordWithEnding(curWord.id, sex, case_, number);
    strcpy(word, temps);
    //free(temps);

    return word;
}

char *GenerateTextForProperties(int propc, Property *propv, Forecast forecast) {
    char *temps;

    setlocale(LC_ALL, "RUS");

    // region подлежащее-существительное
    char *result = calloc(600, sizeof(char));
    struct Words daytime=GetWordsByPartsOfSpeech(Существительные, СВремени, ВремяДня, День);
    int rand_number= rand() % daytime.count;
    Word curWord = daytime.words[rand_number];
    temps = GetWordWithEnding(curWord.id, NO_ENDING);
    char *wordValue = calloc(strlen(temps), sizeof(char));
    strcpy(wordValue, temps);
    free(temps);

    strcat(result,wordValue);
    free(wordValue);
    dospace(result);

    strcat(result, "будет");
    dospace(result);
    // endregion

    struct Words temperature=GetWordsByPartsOfSpeech(Прилагательные, ПТемпературы, Температура, propv[DayTempDeviation].propertyValue);
    rand_number=rand()%temperature.count;
    curWord=temperature.words[rand_number];
    temps = GetWordWithEnding(curWord.id,
                                  Мужской,
                                  -1,
                                  -1);
    wordValue = calloc(strlen(temps), sizeof(char));
    strcpy(wordValue, temps);
    free(temps);
    strcat(result,wordValue);
    free(wordValue);
    dospace(result);





    if (forecast.properties[Direction].propertyValue!=-1) {
        if (forecast.properties[AbsWind].propertyValue<=8) {
            // region направление
            strcat(result, "с");
            dospace(result);
            switch (forecast.properties[Direction].propertyValue) {
                case 0:
                    strcat(result, "северным");
                    break;
                case 1:
                    strcat(result, "северо-восточным");
                    break;
                case 2:
                    strcat(result, "восточным");
                    break;
                case 3:
                    strcat(result, "юго-восточным");
                    break;
                case 4:
                    strcat(result, "южным");
                    break;
                case 5:
                    strcat(result, "юго-западным");
                    break;
                case 6:
                    strcat(result, "западным");
                    break;
                case 7:
                    strcat(result, "северо-западным");
                    break;
            }
            dospace(result);
            // endregion

            // region сильность ветра
            temps = GetRandomWordBy(Прилагательные, ПСилаВетра, propv[AbsWind].propertyType, propv[AbsWind].propertyValue, NO_ENDING);
            strcat(result, temps);
            free(temps);
            dospace(result);
            strcat(result,"ветром.");
            dospace(result);
            // endregion

            // region совет
            if(propv[DayTempDeviation].propertyValue>0){
                strcat(result,"Cоветуем одеваться полегче");
                dospace(result);
            }
            else if(propv[DayTempDeviation].propertyValue<0){
                strcat(result,"Cоветуем одеваться потеплее");
                dospace(result);
            }
            else if(propv[DayTempDeviation].propertyValue==0){
                strcat(result,"Cоветуем одеваться по погоде");
                dospace(result);
            }
            // endregion
        }
        else{
            strcat(result,".");
            dospace(result);
            strcat(result, "Ожидается шторм, не укрывайтесь и не паркуйте автотранспорт под деревьями и шаткими конструкциями.");
            dospace(result);

        }
    }
    else {
        strcat(result, "без ветра");
        dospace(result);
        if (propv[DayTempDeviation].propertyValue > 0) {
            strcat(result, "советуем одеваться полегче");
        } else if (
                propv[DayTempDeviation].propertyValue < 0 ||
                (forecast.properties[HighImpuls].propertyValue-forecast.properties[HighWind].propertyValue)>5) {
            strcat(result, "советуем одеваться потеплее");
        } else if (propv[DayTempDeviation].propertyValue == 0) {
            strcat(result, "советуем одеваться по погоде");
        }
        dospace(result);
    }

    strcat(result,".");
    dospace(result);
    if (forecast.properties[Fallout].propertyValue!=0) {
        strcat(result, "Из осадков ожидается");
        dospace(result);

        temps = GetRandomWordBy(Существительные, СОсадки, propv[AbsFallout].propertyType, propv[AbsFallout].propertyValue, NO_ENDING);
        strcat(result,temps);
        free(temps);
        dospace(result);

        strcat(result,".");
        dospace(result);

    } else {
        strcat(result, "Осадков не ожидается.");
        dospace(result);
    }
    strcat(result,"Давление - ");
    sprintf(result+strlen(result), "%d", forecast.properties[Pressure].propertyValue);
    dospace(result);
    strcat(result,"мм.р.ст");
    dospace(result);
    strcat(result,".");
    dospace(result);
    if(forecast.properties[Event].propertyValue!=0){
        strcat(result,"Ожидается");
        dospace(result);
        switch (forecast.properties[Event].propertyValue) {
            case 1:
                strcat(result,"туман");//войны
                break;
            case 2:
                strcat(result,"гололедица");
                break;
            case 3:
                strcat(result,"гроза");
                break;
            case 4:
                strcat(result,"метель");
                break;
        }
        dospace(result);

    }
    temps=GetRandomWordBy(Существительные,СВремени,ВремяДня,Ночь,NO_ENDING);
    strcat(result,temps);
    free(temps);
    dospace(result);
    strcat(result,"будет");
    dospace(result);
    temps=GetRandomWordBy(Прилагательные,ПТемпературы,Температура,propv[AbsNightTemp].propertyValue,Женский,Именительный,Единственное );
    strcat(result,temps);
    free(temps);
    dospace(result);

    return result;
}

