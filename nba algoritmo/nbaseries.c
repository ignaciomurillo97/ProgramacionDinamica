#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main () {
    int i, j, n;
    float p_h, p_r, *mm, **m;
    // Nacho.TODO = Borrar los comentarios >:v
    n = 7; //numero de juegos
    p_h = 0.57; //probabilidad casa
    p_r = 0.49; //probabilidad visita
    float serie[7] = {0.57,0.57,0.49,0.49,0.49,0.57,0.57}; //Ese 7 es n y cada campo es o p_h o p_r

    int wins = (int)ceil((double)n/2);
    int m_size = (wins+1) * (wins+1);
    mm = calloc(m_size, sizeof (float));
    m = malloc((wins+1) * sizeof (float *));
    m[0] = mm;
    
    for (i = 0; i <= wins; i++) {
        m[i] = &mm[i * (wins+1)];
        for (j = 0; j <= wins; j++) {
            if (i == 0) m[i][j] = 1;
        }
    }

    for (i = 1; i <= wins; i++) {
        for (j = 1; j <= wins; j++) {
            int actual_game = (wins - i) + (wins - j) + 1;
            float p = serie[actual_game-1];
            float q = 1-p;
            m[i][j] = m[i-1][j]*p + m[i][j-1]*q;
            printf("%f ",m[i][j]);
        }
        printf("\n");
    }

    float result = m[wins][wins] * 100;
    printf("El equipo *Nombre* tiene una probabilidad del %f%% de ganar el torneo.\n",result);
    free(mm);
    free(m); 
}
 
