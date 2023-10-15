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

Polynomial multiplyInFp(Polynomial a, Polynomial b, int p) {
    int max_degree = a.degree + b.degree;
    Polynomial result = createPolynomial(max_degree);
    for(int i = 0; i <= a.degree; i++) {
        for(int j = 0; j <= b.degree; j++) {
            result.coef[i + j] += a.coef[i] * b.coef[j];
            result.coef[i + j] %= p;
        }
    }
    return result;
}
int isGenerator(Polynomial g, Polynomial f, int p, int q) {
    int q_minus_1 = q - 1;
    // g^(q-1) doit être égal à 1
    Polynomial result = g;
    for(int i = 1; i < q_minus_1; i++) {
        result = multiplyInFp(result, g, p);
    }
    if(result.degree != 0 || result.coef[0] != 1) {
        return 0;  // g n'est pas un générateur
    }

    // g^(q-1)/t ne doit pas être égal à 1 pour tout diviseur premier t de q-1
    for(int t = 2; t <= q_minus_1/2; t++) {
        if(q_minus_1 % t == 0) {
            Polynomial tempResult = g;
            for(int i = 1; i < q_minus_1/t; i++) {
                tempResult = multiplyInFp(tempResult, g, p);
            }
            if(tempResult.degree == 0 && tempResult.coef[0] == 1) {
                return 0;  // g n'est pas un générateur
            }
        }
    }
    return 1;  // g est un générateur
}

int main() {
    int p, d, q;

    // Saisie de p et d
    printf("Entrez le nombre premier p: ");
    scanf("%d", &p);
    printf("Entrez le degré d: ");
    scanf("%d", &d);
    
    q = p;
    for(int i = 1; i < d; i++) {
        q *= p;
    }

    // Saisie du polynôme irréductible f(X)
    Polynomial f = createPolynomial(d);
    printf("Entrez les coefficients du polynôme irréductible f(X) de degré %d:\n", d);
    for(int i = d; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &f.coef[i]);
    }

    // Saisie de l'élément à tester
    Polynomial element = createPolynomial(d - 1); 
    printf("Entrez les coefficients de l'élément à tester:\n");
    for(int i = d - 1; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &element.coef[i]);
    }

    // Test pour savoir si l'élément est un générateur
    if(isGenerator(element, f, p, q)) {
        printf("Oui, l’élément ");
        for(int i = element.degree; i >= 0; i--) {
            printf("%dx^%d ", element.coef[i], i);
            if(i > 0) {
                printf("+ ");
            }
        }
        printf("est générateur.\n");
    } else {
        printf("L’élément ");
        for(int i = element.degree; i >= 0; i--) {
            printf("%dx^%d ", element.coef[i], i);
            if(i > 0) {
                printf("+ ");
            }
        }
        printf("n'est pas générateur.\n");
    }

    // Libération de la mémoire
    freePolynomial(element);
    freePolynomial(f);

    return 0;
}
