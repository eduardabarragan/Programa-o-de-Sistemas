#include <stdio.h>
#include <stdlib.h>

#define Tdouble 0
#define Tfloat 1

typedef union {
    float f;
    struct {
        unsigned int frac : 23;
        unsigned int exp  : 8;
        unsigned int sign : 1;
    } bits;
} float_repr;

typedef union {
    double d;
    struct {
        unsigned long long frac : 52;
        unsigned long long exp  : 11;
        unsigned long long sign : 1;
    } bits;
} double_repr;

typedef struct {
    int type;
    void *data;
} real;

real novo_real(double x, int t) {
    real r;
    r.type = t;
    if (t == Tdouble) {
        double *pt_data = (double *)malloc(sizeof(double));
        *pt_data = x;
        r.data = pt_data;
    } else if (t == Tfloat) {
        float *pt_data = (float *)malloc(sizeof(float));
        *pt_data = (float)x;
        r.data = pt_data;
    }
    return r;
}

void exclui_real(real *x) {
    if (x->data != NULL) {
        free(x->data);
        x->data = NULL;
    }
}

real sqrtNR(real A) {
    if (A.type == Tfloat) {
        float A_val = *((float *)A.data);
        if (A_val == 0.0f || A_val == 1.0f)
            return novo_real((float)A_val, Tfloat);

        float_repr x0_ieee;
        x0_ieee.f = A_val;

        int impar = !(x0_ieee.bits.exp & 1);
        x0_ieee.bits.exp = ((x0_ieee.bits.exp >> 1) + 0x3F);
        x0_ieee.bits.frac >>= 1;

        float x0 = x0_ieee.f;
        if (impar)
            x0 *= 1.4142135623730950488016887f;

        float x1 = x0;
        do {
            x0 = x1;
            x1 = 0.5f * (x0 + A_val / x0);
        } while (x0 != x1);

        return novo_real(x1, Tfloat);
    } else if (A.type == Tdouble) {
        double A_val = *((double *)A.data);
        if (A_val == 0.0 || A_val == 1.0)
            return novo_real(A_val, Tdouble);

        double_repr x0_ieee;
        x0_ieee.d = A_val;

        int impar = !(x0_ieee.bits.exp & 1);
        x0_ieee.bits.exp = ((x0_ieee.bits.exp >> 1) + 0x1FF);
        x0_ieee.bits.frac >>= 1;

        double x0 = x0_ieee.d;
        if (impar)
            x0 *= 1.4142135623730950488016887;

        double x1 = x0;
        do {
            x0 = x1;
            x1 = 0.5 * (x0 + A_val / x0);
        } while (x0 != x1);

        return novo_real(x1, Tdouble);
    }

    return A;
}

void change_to_Tdouble(real *A) {
    if (A->type != Tfloat) return;
    float valor_atual = *((float *)A->data);
    free(A->data);
    double *novo_dado = (double *)malloc(sizeof(double));
    *novo_dado = (double)valor_atual;
    A->data = novo_dado;
    A->type = Tdouble;
}

void change_to_Tfloat(real *A) {
    if (A->type != Tdouble) return;
    double valor_atual = *((double *)A->data);
    free(A->data);
    float *novo_dado = (float *)malloc(sizeof(float));
    *novo_dado = (float)valor_atual;
    A->data = novo_dado;
    A->type = Tfloat;
}

double real_to_double(real A) {
    if (A.type == Tfloat)
        change_to_Tdouble(&A);
    return *((double *)A.data);
}

float real_to_float(real A) {
    if (A.type == Tdouble)
        change_to_Tfloat(&A);
    return *((float *)A.data);
}

int main() {
    double x = 3.141592;

    real r1 = novo_real(x, Tfloat);
    real r2 = novo_real(x, Tdouble);

    real sqrtR1 = sqrtNR(r1);
    real sqrtR2 = sqrtNR(r2);

    float sqrt_xF = real_to_float(sqrtR1);
    double sqrt_xD = real_to_double(sqrtR2);

    printf("float sqrt(%lf): %.10f\n", x, sqrt_xF);
    printf("double sqrt(%lf): %.15lf\n", x, sqrt_xD);

    float_repr fbits;
    fbits.f = sqrt_xF;
    printf("FLOAT (32 bits): sinal:%x mantissa:%x expoente:%x\n",
           fbits.bits.sign, fbits.bits.frac, fbits.bits.exp);

    double_repr dbits;
    dbits.d = sqrt_xD;
    printf("DOUBLE (64 bits): sinal:%llx mantissa:%llx expoente:%llx\n",
           dbits.bits.sign, dbits.bits.frac, dbits.bits.exp);

    exclui_real(&r1);
    exclui_real(&r2);
    exclui_real(&sqrtR1);
    exclui_real(&sqrtR2);

    return 0;
}
