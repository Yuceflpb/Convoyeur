#ifndef FONCTIONS_MATHS_H
#define FONCTIONS_MATHS_H
#include <stdint.h>

#define DIX			10


float StepsToCm(int32_t nbSteps);
float MmToCm(uint16_t ValeurMm);
int16_t CmToSteps(float ValeurCm);
float Multi(float val1, uint16_t val2);
int16_t Distance_to_temps(float objectif, float acceleration, float delta_t);



#endif /* FONCTIONS_MATHS_H */