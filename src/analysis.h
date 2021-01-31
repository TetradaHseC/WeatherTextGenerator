#include <forecast.h>
#include <forecastprop.h>
#include <property.h>
#include <analprop.h>

const int MidleTemp[] = { -24, -15, -5, 2, 15, 19, 23, 23, 15, 4, -5, -15};
const int AbsTempStep = 4;
const int TempDiviationStep = 2;
const int WindStep = 2;
const int WindDefault = 10; 

struct Analysis {
    struct Property propertys[NOPA];
    struct Forecast forecast;
};

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
    struct Analysis analysis;
    analysis.propertys[AnalisysProperties::absDayTemp].propertyValue = AbsTempAnalisys((forecast.properties[ForecastProperties::highDayTemp].propertyValue + forecast.properties[ForecastProperties::lowDayTemp].propertyValue) / 2);
    analysis.propertys[AnalisysProperties::absNightTemp].propertyValue = AbsTempAnalisys((forecast.properties[ForecastProperties::highNightTemp].propertyValue + forecast.properties[ForecastProperties::lowNightTemp].propertyValue) / 2);
    analysis.propertys[AnalisysProperties::absFellTemp].propertyValue = AbsTempAnalisys((forecast.properties[ForecastProperties::highFellTemp].propertyValue + forecast.properties[ForecastProperties::lowFeelTemp].propertyValue) / 2);
    int month = forecast.properties[ForecastProperties::month].propertyValue;
    int relevantTemp;
    relevantTemp = (forecast.properties[ForecastProperties::highNightTemp].propertyValue + forecast.properties[ForecastProperties::lowNightTemp].propertyValue) / 2;
    analysis.propertys[AnalisysProperties::dayTempDeviation].propertyValue = DiviationAnalis(month, relevantTemp);
    relevantTemp = (forecast.properties[ForecastProperties::highDayTemp].propertyValue + forecast.properties[ForecastProperties::lowDayTemp].propertyValue) / 2;
    analysis.propertys[AnalisysProperties::dayTempDeviation].propertyValue = DiviationAnalis(month, relevantTemp);
    analysis.propertys[AnalisysProperties::absWind].propertyValue = WindAnalisys((forecast.properties[ForecastProperties::lowWind].propertyValue + forecast.properties[ForecastProperties::highWind].propertyValue) / 2);
    analysis.forecast = forecast;
    return analysis;
};