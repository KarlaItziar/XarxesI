#include <stdio.h>   // fgets() i printf()
#include <stdlib.h>  // strtof()
#include <string.h>  // strchr()
#include <math.h>

int llegirCoficientsIGrau(float *a, float *b, float *c) {
       
    char segon_grau[100];

    int grau; // si existeix a, retornara 2. en cas contrari retornara 1. si no funcina 0.
    float termeA, termeB, termeC; // Variables per guardar els termes de la funció

    *a = 0.0f;
    *b = 0.0f;
    *c = 0.0f;

    printf("Introdueix una funció en la forma explicita (ax^2+bx+c, sense espais i amb termes a, b i c visibles): ");
    fgets(segon_grau, sizeof(segon_grau), stdin);
    segon_grau[strcspn(segon_grau, "\n")] = '\0'; //Neteja 

    char *posicio_x2 = strstr(segon_grau, "x^2"); // Busca fins x^2

    if (posicio_x2 == NULL) {
              
        char *posicio_x = strchr(segon_grau, 'x'); // Busca la x en el segon grau

        if (posicio_x == NULL) {
            grau = 0; // No s'ha trobat la x
        } 

        else {
            grau = 1; // S'ha trobat la x, és de primer grau

            termeB = strtof(segon_grau, NULL); // Converteix el que hi ha abans de la x a float
            termeC = strtof(posicio_x + 1, NULL); // Converteix el que hi ha després de la x a float
            
            *a = 0;
            *b = termeB;
            *c = termeC;
        }
    }

    else {
        char *posicio_x = strchr(posicio_x2 + 3, 'x');

        if (posicio_x == NULL) {
            grau = 0;
        } else {
            grau = (termeA == 0.0f) ? 1 : 2; // Si a és 0, és de primer grau; si no, és de segon grau

            termeA = strtof(segon_grau, NULL); // Converteix el que hi ha abans de x^2 a float
            termeB = strtof(posicio_x2 + 3, NULL); // Converteix el que hi ha després de x^2 a float
            termeC = strtof(posicio_x + 1, NULL); // Converteix el que hi ha després de la segona x a float

            *a = termeA;
            *b = termeB;
            *c = termeC;
        }
    }

    return grau;
}

float CalcularTallEixY(float a, float b, float c) {
    return c; // Retorna el tall amb l'eix y (quan x = 0)
}

float CalcularTallEixX(float a, float b, float c, float *tall_x1, float *tall_x2) {
    if (a == 0.0f) {
        if (b == 0.0f) {
            return 0;
        }

        *tall_x1 = -c / b;
        return 1;
    }

    float arrel = b*b - 4*a*c;

    if (arrel > 0) {
        *tall_x1 = (-b + sqrt(arrel)) / (2*a);
        *tall_x2 = (-b - sqrt(arrel)) / (2*a);
        return 2;
    } 
    else if (arrel == 0) {
        *tall_x1 = -b / (2*a);
        return 1;
    } 
    else {
        return 0;
    }
}

int CalcularVertex(float a, float b, float c, float *vertex_x, float *vertex_y) {
    
    if (a == 0.0f) {
        return -1; // Una funció de primer grau no té vèrtex
    }

    *vertex_x = -b / (2 * a);
    *vertex_y = a * (*vertex_x) * (*vertex_x)
              + b * (*vertex_x)
              + c;

    return 0; // Càlcul correcte
}

