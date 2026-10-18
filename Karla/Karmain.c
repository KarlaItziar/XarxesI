#include <stdio.h>
#include "KarFuncions.h"

int main(void) {
    float a, b, c;
    float tall_x1, tall_x2;
    float vertex_x, vertex_y;

    int grau = llegirCoficientsIGrau(&a, &b, &c);

    if (grau == 0) {
        printf("No s'ha pogut llegir la funció. Revisa el format.\n");
        return 1;
    }

    printf("Grau de la funció: %d\n", grau);
    printf("Coeficients: a = %g, b = %g, c = %g\n", a, b, c);

    printf("Tall amb l'eix y: (0, %g)\n",
           CalcularTallEixY(a, b, c));

    int nombre_talls = CalcularTallEixX(
        a, b, c, &tall_x1, &tall_x2
    );

    if (a == 0.0f && b == 0.0f) {
        if (c == 0.0f) {
            printf("La funció coincideix amb l'eix x: tots els valors de x són talls.\n");
        } else {
            printf("No hi ha tall amb l'eix x.\n");
        }
    } else if (nombre_talls == 2) {
        printf("Talls amb l'eix x: (%g, 0) i (%g, 0)\n",
               tall_x1, tall_x2);
    } else if (nombre_talls == 1) {
        printf("Tall amb l'eix x: (%g, 0)\n", tall_x1);
    } else {
        printf("No hi ha tall amb l'eix x.\n");
    }

    if (CalcularVertex(a, b, c, &vertex_x, &vertex_y)) {
        printf("Vèrtex: (%g, %g)\n", vertex_x, vertex_y);
    } else {
        printf("La funció de primer grau no té vèrtex.\n");
    }

    return 0;
}