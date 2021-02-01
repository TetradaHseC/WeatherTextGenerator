#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "forecast.h"
#include "../models/forecastprop.h"
#include "../models/property.h"
#include "../models/analprop.h"

const int MidleTemp[] = { -24, -15, -5, 2, 15, 19, 23, 23, 15, 4, -5, -15};
const int MidleFallout[] = {3, 3, 1, 0, 0, 0, 0, 1, 2, 2, 3, 3};
const int AbsTempStep = 4; 
const int TempDevStep = 2; 
const int WindStep = 2;
const int ImpulsStep = 2;
const int PressureStep = 5;
const int WindDefault = 10; 
const int ImpulsDefault = 10;
const int PressureDefault = 710;

typedef struct {
    struct StructProperty propertys[NOPW];
    Forecast forecast;
} Analysis;

int AbsTempAnalisys(int temp) {
    return temp / AbsTempStep;
}

int TempDevAnalysis(int month, int temp) {
    int dTemp = temp - MidleTemp[month];
    return dTemp / TempDevStep;
}

int WindAnalysis(int wind) {
#define ifinrangeretv(fir, sec) value++; if ((fir <= wind) && (wind < sec)) return value

    int value = 0;
    ifinrangeretv(0, 0.5);
    ifinrangeretv(0.5, 1.7);
    ifinrangeretv(1.8, 3.3);
    ifinrangeretv(3.3, 5.2);
    ifinrangeretv(5.2, 7.4);
    ifinrangeretv(7.4, 9.8);
    ifinrangeretv(9.8, 12.4);
    ifinrangeretv(12.4, 15.2);
    ifinrangeretv(15.2, 100.0);

    return (wind - WindDefault) / WindStep;
}

int FalloutDevAnalisys(int month, int fallout) {
    return fallout - MidleFallout[month];
}

int AbsImpulsAnalisys(int impuls) {
    return (impuls - ImpulsDefault) / ImpulsStep;
}

int AbsPressureAnalisys(int pressure) {
    return (pressure - PressureDefault) / PressureStep;
}

Analysis GetAnalysis(Forecast forecast) {
    Analysis analysis;

    analysis.propertys[AbsDayTemp].propertyValue = AbsTempAnalisys((forecast.properties[HighDayTemp].propertyValue + forecast.properties[LowDayTemp].propertyValue) / 2);
    analysis.propertys[AbsNightTemp].propertyValue = AbsTempAnalisys((forecast.properties[HighNightTemp].propertyValue + forecast.properties[LowNightTemp].propertyValue) / 2);
    analysis.propertys[AbsFellTemp].propertyValue = AbsTempAnalisys((forecast.properties[HighFellTemp].propertyValue + forecast.properties[LowFeelTemp].propertyValue) / 2);

    int month = forecast.properties[Month].propertyValue;
    int relevantTemp;

    relevantTemp = (forecast.properties[HighNightTemp].propertyValue + forecast.properties[LowNightTemp].propertyValue) / 2;
    analysis.propertys[DayTempDeviation].propertyValue = TempDevAnalysis(month, relevantTemp);
    relevantTemp = (forecast.properties[HighDayTemp].propertyValue + forecast.properties[LowDayTemp].propertyValue) / 2;
    analysis.propertys[DayTempDeviation].propertyValue = TempDevAnalysis(month, relevantTemp);
    analysis.propertys[AbsWind].propertyValue = WindAnalysis((forecast.properties[LowWind].propertyValue + forecast.properties[HighWind].propertyValue) / 2);

    analysis.propertys[AbsFallout].propertyValue = forecast.properties[Fallout].propertyValue;
    analysis.propertys[FalloutDeviation].propertyValue = FalloutDevAnalisys(month, forecast.properties[Fallout].propertyValue);

    analysis.propertys[AbsImpuls].propertyValue = AbsImpulsAnalisys((forecast.properties[LowImpuls].propertyValue + forecast.properties[HighImpuls].propertyValue)/2);

    analysis.propertys[AbsPressure].propertyValue = AbsPressureAnalisys(forecast.properties[Pressure].propertyValue);

    analysis.forecast = forecast;

    return analysis;
};
#endif //ANALYSIS_H
