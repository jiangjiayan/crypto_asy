#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int coef;
    int exp;
} Polynomial;

void multiplyPolynomials(Polynomial poly1[], int m, Polynomial poly2[], int n, Polynomial result[]) {
    for (int i = 0; i < m + n - 1; i++) {
        result[i].coef = 0;
        result[i].exp = i;
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int new_exp = poly1[i].exp + poly2[j].exp;
            result[new_exp].coef += poly1[i].coef * poly2[j].coef;
        }
    }
}

int main() {
    int m, n,p;
    printf("p de F_p: ");
    scanf("%d", &p);

    printf("Max exp du premier poly: ");
    scanf("%d", &m);
    m++;
    Polynomial *poly1 = (Polynomial *)malloc(m * sizeof(Polynomial));
    printf("entrer les coef du deuxème poly de exp haut à bas:\n");
    for (int i = 0; i < m; i++) {
        printf("coef de x^%d est: ", m - i - 1);
        scanf("%d", &poly1[i].coef);
        poly1[i].exp = m - i - 1;
    }

    printf("Max exp du deuxème poly: ");
    scanf("%d", &n);
    n++;
    Polynomial *poly2 = (Polynomial *)malloc(n * sizeof(Polynomial));
    printf("entrer les coef du deuxème poly de exp haut à bas:\n");
    for (int i = 0; i < n; i++) {
        printf("coef de x^%d est: ", n - i - 1);
        scanf("%d", &poly2[i].coef);
        poly2[i].exp = n - i - 1;
    }

    Polynomial *result = (Polynomial *)malloc((m + n - 1) * sizeof(Polynomial));
    multiplyPolynomials(poly1, m, poly2, n, result);

    printf("le reusltat de multiplication: ");
    for (int i = m + n - 2; i >= 0; i--) {  
        if (result[i].coef != 0) {
            printf("%dx^%d ", (result[i].coef)%p, result[i].exp);
            if (i > 0 && result[i - 1].coef > 0) {
                printf("+ ");
            }
        }
    }

    free(poly1);
    free(poly2);
    free(result);

    return 0;
}




