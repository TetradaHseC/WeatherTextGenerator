#ifndef GENERATION_H
#define GENERATION_H

#include "../models/db_models.h"
#include "../models/property.h"

char *GenerateTextForProperties(int propc, Property *propv, Forecast forecast);

#endif // GENERATION_H
