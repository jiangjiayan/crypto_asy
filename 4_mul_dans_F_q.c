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
    
    freePolynomial(quotient);

return remainder;


    return result;
}

int main() {
    int p, d;
    printf("Entrez le nombre premier p: ");
    scanf("%d", &p);
    printf("Entrez le degré d: ");
    scanf("%d", &d);

    // Le polynôme irréductible f(X) doit être fourni.
    // Pour cet exemple, supposons qu'il est de degré d.
    Polynomial f = createPolynomial(d);
    printf("Entrez les coefficients du polynôme irréductible f(X) de degré %d:\n", d);
    for(int i = d; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &f.coef[i]);
    }

    // Multipliez deux polynômes dans F_q
    Polynomial a = createPolynomial(d - 1);  // Exemple pour polynômes de degré d-1
    Polynomial b = createPolynomial(d - 1);
    printf("Entrez les coefficients du premier polynôme:\n");
    for(int i = d - 1; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &a.coef[i]);
    }
    printf("Entrez les coefficients du second polynôme:\n");
    for(int i = d - 1; i >= 0; i--) {
        printf("Coefficient de x^%d: ", i);
        scanf("%d", &b.coef[i]);
    }

    Polynomial prod = multiplyInFq(a, b, p, f);
    
    printf("Le produit des polynômes dans F_q est:\n");
    for(int i = prod.degree; i >= 0; i--) {
        if(prod.coef[i] != 0) {
            printf("%dx^%d", prod.coef[i], i);
            int hasMoreTerms = 0;
            for(int j = i-1; j >= 0; j--) {
                if(prod.coef[j] != 0) {
                    hasMoreTerms = 1;
                    break;
                }
            }
            if(hasMoreTerms) {
                printf(" + ");
            }
        }
    }
    printf("\n");

    


    freePolynomial(a);
    freePolynomial(b);
    freePolynomial(f);
    freePolynomial(prod);

    return 0;
}
