#include "ui/ui.h"
#include "stepan/forecast.h"
#include "stepan/analysis.h"
#include "models/db_models.h"
#include "generation/generation.h"

Property ForecastPropertyToDBPropertyAdapter(int i, struct StructProperty structProperty);

int main() {
    srand(0);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    // This will end when exit(0) is called in Start()
    while (1) {
        Start();

        // after all we starting algorithm
        FILE *input = fopen(inputFile, "r");

        size_t count = 0;
        char **inputTable = ReadFile(input, &count);
        fclose(input);

        FILE *output = fopen(outputFile, "w");
        for (int row = 1; row < count; ++row) {
            Forecast tempForecast = ParseForecast(inputTable[row]);
            Analysis fproperties = GetAnalysis(tempForecast);
            Property properties[NOPA] = { 0 };

            for (int propertyI = 0; propertyI < NOPA; ++propertyI) {
                properties[propertyI] = ForecastPropertyToDBPropertyAdapter(
                        propertyI, fproperties.propertys[propertyI]
                );
            }

            char *temprow = GenerateTextForProperties(NOPA, properties, fproperties.forecast);
            fprintf(output,
                    "%d.%d.%d: %s\n",
                    fproperties.forecast.properties[Day].propertyValue,
                    fproperties.forecast.properties[Month].propertyValue,
                    fproperties.forecast.properties[Year].propertyValue,
                    temprow);
            free(temprow);
        }
        for (int i = 0; i < count; ++i)
            free(inputTable[i]);
        free(inputTable);

        fclose(output);
    }
#pragma clang diagnostic pop

    return 0;
}

Property ForecastPropertyToDBPropertyAdapter(int i, struct StructProperty structProperty) {
    Property property = { i, structProperty.propertyValue };

    return property;
}
