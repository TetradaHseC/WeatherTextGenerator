#include "forecast.h"
#include "forecastprop.h"
#include "property.h"
#include "analprop.h"

const int MidleTemp[] = { -24, -15, -5, 2, 15, 19, 23, 23, 15, 4, -5, -15};
const int AbsTempStep = 4;
const int TempDiviationStep = 2;
const int WindStep = 2;
const int WindDefault = 10; 

typedef struct {
    struct StructProperty propertys[NOPA];
    Forecast forecast;
} Analysis;

int AbsTempAnalisys(int temp) {
    return temp / AbsTempStep;
}

int DiviationAnalis(int month, int temp) {
    int dTemp = temp - MidleTemp[month];
    return dTemp / TempDiviationStep;
}

int WindAnalisys(int wind) {
    return (wind - WindDefault) / WindStep;
}

Analysis GetAnalysis(Forecast forecast) {
    Analysis analysis;

    analysis.propertys[absDayTemp].propertyValue = AbsTempAnalisys((forecast.properties[highDayTemp].propertyValue + forecast.properties[lowDayTemp].propertyValue) / 2);
    analysis.propertys[absNightTemp].propertyValue = AbsTempAnalisys((forecast.properties[highNightTemp].propertyValue + forecast.properties[lowNightTemp].propertyValue) / 2);
    analysis.propertys[absFellTemp].propertyValue = AbsTempAnalisys((forecast.properties[highFellTemp].propertyValue + forecast.properties[lowFeelTemp].propertyValue) / 2);

    int month = forecast.properties[month].propertyValue;
    int relevantTemp;

    relevantTemp = (forecast.properties[highNightTemp].propertyValue + forecast.properties[lowNightTemp].propertyValue) / 2;
    analysis.propertys[dayTempDeviation].propertyValue = DiviationAnalis(month, relevantTemp);
    relevantTemp = (forecast.properties[highDayTemp].propertyValue + forecast.properties[lowDayTemp].propertyValue) / 2;
    analysis.propertys[dayTempDeviation].propertyValue = DiviationAnalis(month, relevantTemp);
    analysis.propertys[absWind].propertyValue = WindAnalisys((forecast.properties[lowWind].propertyValue + forecast.properties[highWind].propertyValue) / 2);
    analysis.forecast = forecast;

    return analysis;
};