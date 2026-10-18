#ifndef CALCULADORA_H
#define CALCULADORA_H

int llegirCoficientsIGrau(float *a, float *b, float *c);
float CalcularTallEixY(float a, float b, float c);
float CalcularTallEixX(float a, float b, float c, float *tall_x1, float *tall_x2);
int CalcularVertex(float a, float b, float c, float *vertex_x, float *vertex_y);

#endif
