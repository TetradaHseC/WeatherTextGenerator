#include "ui/ui.h"
#include "stepan/forecast.h"
#include "stepan/analysis.h"
#include "models/db_models.h"
#include "generation/generation.h"

Property ForecastPropertyToDBPropertyAdapter(int i, struct StructProperty structProperty);

int main() {
    srand(NULL);

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
        for (int row = 1; row < count; ++row) {
            Analysis fproperties = GetAnalysis(ParseForecast(inputTable[row]));
            Property properties[NOPA] = { 0 };

            for (int propertyI = 0; propertyI < NOPA; ++propertyI) {
                properties[propertyI] = ForecastPropertyToDBPropertyAdapter(
                        propertyI, fproperties.propertys[propertyI]
                );
            }

            GenerateTextForProperties(NOPA, properties, fproperties.forecast);
        }
        for (int i = 0; i < count; ++i)
            free(inputTable[i]);
        free(inputTable);
    }
#pragma clang diagnostic pop

    return 0;
}

Property ForecastPropertyToDBPropertyAdapter(int i, struct StructProperty structProperty) {
    Property property = { i, structProperty.propertyValue };

    return property;
}
