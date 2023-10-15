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

void divideInFq(Polynomial dividend, Polynomial divisor, int p, Polynomial* quotient, Polynomial* remainder) {
    *quotient = createPolynomial(dividend.degree - divisor.degree);
    *remainder = createPolynomial(dividend.degree);
    for(int i = 0; i <= dividend.degree; i++) {
        remainder->coef[i] = dividend.coef[i];
    }


    while(remainder->degree >= divisor.degree) {
        int coef = remainder->coef[remainder->degree] * divisor.coef[divisor.degree] % p;
        int degree_diff = remainder->degree - divisor.degree;

        quotient->coef[degree_diff] = coef;

        for(int i = 0; i <= divisor.degree; i++) {
            remainder->coef[i + degree_diff] -= coef * divisor.coef[i];
            remainder->coef[i + degree_diff] %= p;
            if (remainder->coef[i + degree_diff] < 0) {
                remainder->coef[i + degree_diff] += p;
            }
        }

        
        while (remainder->degree >= 0 && remainder->coef[remainder->degree] == 0) {
            remainder->degree--;
        }
    }
}


Polynomial multiplyInFq(Polynomial a, Polynomial b, int p, Polynomial f) {
    Polynomial result = createPolynomial(a.degree + b.degree);
    for(int i = 0; i <= a.degree; i++) {
        for(int j = 0; j <= b.degree; j++) {
            result.coef[i + j] += a.coef[i] * b.coef[j];
            result.coef[i + j] %= p;
        }
    }
    Polynomial prodTemp = result; // 创建一个临时多项式来保存原始的乘积结果
    Polynomial quotient, remainder;

    divideInFq(prodTemp, f, p, &quotient, &remainder);
    
    

return remainder;


    return result;
}

Polynomial subtractInFq(Polynomial a, Polynomial b, int p) {
    int max_degree = a.degree > b.degree ? a.degree : b.degree;
    Polynomial result = createPolynomial(max_degree);

    for (int i = 0; i <= max_degree; i++) {
        if (i <= a.degree) {
            result.coef[i] += a.coef[i];
        }
        if (i <= b.degree) {
            result.coef[i] -= b.coef[i];
            if (result.coef[i] < 0) {
                result.coef[i] += p;
            }
        }
        result.coef[i] %= p;
    }

    // Update the degree of the resulting polynomial
    while (result.degree >= 0 && result.coef[result.degree] == 0) {
        result.degree--;
    }

    return result;
}


Polynomial extendedEuclidean(Polynomial a, Polynomial f, int p, Polynomial* u, Polynomial* v) {
    if (a.degree == -1) {
        *u = createPolynomial(-1);  // 0
        *v = createPolynomial(0);   // 1
        v->coef[0] = 1;
        return f;
    }

    Polynomial quotient, remainder;
    divideInFq(f, a, p, &quotient, &remainder);

    Polynomial u1, v1;
    Polynomial g = extendedEuclidean(remainder, a, p, &u1, &v1);

    *u = subtractInFq(v1, multiplyInFq(quotient, u1, p, f), p);
    *v = u1;


    return g;
}

Polynomial inverseInFq(Polynomial a, int p, Polynomial f) {
    Polynomial u, v;
    Polynomial g = extendedEuclidean(a, f, p, &u, &v);

    // Si g est différent de 1, alors a n'a pas d'inverse mod f.
    if (g.degree != 0 || g.coef[0] != 1) {
        printf("Pas d'inverse modulaire!\n");
        exit(1);
    }
    

    freePolynomial(v);
    freePolynomial(g);

    return u;
}


int main() {
    int p, d;
    printf("Entrez le nombre premier p: ");
    scanf("%d", &p);
    printf("Entrez le degré d: ");
    scanf("%d", &d);
    
    Polynomial f = createPolynomial(d);
    printf("Entrez les coefficients du polynôme irréductible f(X) de degré %d:\n", d);
    for(int i = d; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &f.coef[i]);
    }

    Polynomial a = createPolynomial(d - 1); 
    printf("Entrez les coefficients du polynôme pour lequel vous voulez trouver l'inverse:\n");
    for(int i = d - 1; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &a.coef[i]);
    }

    Polynomial inverse = inverseInFq(a, p, f);
    printf("L'inverse du polynôme dans F_q est:\n");
    for(int i = inverse.degree; i >= 0; i--) {
        if(inverse.coef[i] != 0) {
            printf("%dx^%d ", inverse.coef[i], i);
            if (i > 0) {
                printf("+ ");
            }
        }
    }
    printf("\n");

    freePolynomial(a);
    freePolynomial(f);
    freePolynomial(inverse);

    return 0;
}
