#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* coef;
    int degree;
} Polynomial;

Polynomial createPolynomial(int degree) {
    Polynomial poly;
    poly.degree = degree;
    poly.coef = (int*)malloc((degree + 1) * sizeof(int));
    for(int i = 0; i <= degree; i++) {
        poly.coef[i] = 0;
    }
    return poly;
}

void freePolynomial(Polynomial poly) {
    free(poly.coef);
}

Polynomial multiply(Polynomial a, Polynomial b, int p) {
    Polynomial result = createPolynomial(a.degree + b.degree);
    for(int i = 0; i <= a.degree; i++) {
        for(int j = 0; j <= b.degree; j++) {
            result.coef[i + j] += a.coef[i] * b.coef[j];
            result.coef[i + j] %= p;
        }
    }
    return result;
}

Polynomial deepCopy(Polynomial source) {
    Polynomial dest = createPolynomial(source.degree);
    for (int i = 0; i <= source.degree; i++) {
        dest.coef[i] = source.coef[i];
    }
    return dest;
}


void divide(Polynomial dividende, Polynomial diviseur, int p, Polynomial* quotient, Polynomial* res) {
    *quotient = createPolynomial(dividende.degree - diviseur.degree);
    *res = deepCopy(dividende);


    while(res->degree >= diviseur.degree) {
        int coef = res->coef[res->degree] * diviseur.coef[diviseur.degree] % p;
        int degree_diff = res->degree - diviseur.degree;

        quotient->coef[degree_diff] = coef;

        for(int i = 0; i <= diviseur.degree; i++) {
            res->coef[i + degree_diff] -= coef * diviseur.coef[i];
            res->coef[i + degree_diff] %= p;
            if (res->coef[i + degree_diff] < 0) {
                res->coef[i + degree_diff] += p;
            }
        }

        
        while (res->degree >= 0 && res->coef[res->degree] == 0) {
            res->degree--;
        }
    }
}
Polynomial inputPolynomial() {
    int degree;
    printf("entrer le max dégree du polynome: ");
    scanf("%d", &degree);
    
    Polynomial poly = createPolynomial(degree);
    printf("Entrez les coefs du polynôme par ordre décroissant:\n");
    for (int i = 0; i <= degree; i++) {
        printf("coef x^%d est: ", degree - i);
        scanf("%d", &poly.coef[degree - i]);
    }

    return poly;
}

int main() {
    int p;
    printf("p de F_p est : ");
    scanf("%d", &p);

    printf("polynome de dividende:\n");
    Polynomial dividende = inputPolynomial();

    printf("polynome de diviseur:\n");
    Polynomial diviseur = inputPolynomial();

    Polynomial quotient = createPolynomial(dividende.degree - diviseur.degree);
    Polynomial res = createPolynomial(dividende.degree);

    divide(dividende, diviseur, p, &quotient, &res);

    printf("quotient:\n");
    for(int i = quotient.degree; i >= 0; i--) {
        if (quotient.coef[i] != 0) {
            printf("%dx^%d ", quotient.coef[i], i);
            if (i > 0) {
                printf("+ ");
            }
        }
    }
    printf("\n");

    printf("res:\n");
    for(int i = res.degree; i >= 0; i--) {
        if (res.coef[i] != 0) {
            printf("%dx^%d ", res.coef[i], i);
            if (i > 0) {
                printf("+ ");
            }
        }
    }
    printf("\n");

    freePolynomial(dividende);
    freePolynomial(diviseur);
    freePolynomial(quotient);
    freePolynomial(res);

    return 0;
}


