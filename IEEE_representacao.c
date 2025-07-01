#include <stdio.h>
#include <stdlib.h>

#define Tdouble 0
#define Tfloat 1

typedef struct{
    unsigned long long int f:52;
    unsigned long long int E:11;
    unsigned long long int s:1;
} dIEEE;

typedef struct {
    unsigned long int f:23;
    unsigned long int E:8;
    unsigned long int s:1;
}fIEEE;

typedef struct{
    int type;
    void *data;
}real;

real novo_real(double x, int t){
    real r;
    r.type = t;
    if(t==Tdouble){
        double *pt_data;
        pt_data = (double *) malloc(sizeof(double));
        *pt_data = x;
        r.data = (void *) pt_data;
    }else if(t==Tfloat){
        float *pt_data;
        pt_data = (float *) malloc(sizeof(float));
        *pt_data = (float)x;
        r.data = (void *) pt_data;
    }
    return r;
};

void exclui_real(real *x){
    if(x->data!=NULL){
        free(x->data);
        x->data=NULL;
    }
}


real sqrtNR(real A){
/*
A função sqrtNR recebe um valor real e retorna o valor real da raiz quadrada de A calculado 
com o método de Newton Raphson. 
A função deve analisar se o valor A é representado como Tfloat ou Tdouble.
    - Caso o tipo de A seja Tfloat, o retorno deverá ser do tipo Tfloat
    - Caso o tipo de A seja Tdouble, o retorno deverá ser do tipo Tdouble
Se o tipo não for Tdouble nem Tfloat, o comportamento é indefinido (ou seja, não se sabe o que será retornado) 
*/
    return A; // Cuidado com esse retorno, pois A.data é uma região de memória que não faz parte da struct
}

void change_to_Tdouble(real *A){
/*
A função change_to_Tdouble deve converter o tipo de dado do valor real para Tdouble;
Caso o o tipo de A não seja Tfloat, nada deve ser feito.
*/
}

void change_to_Tfloat(real *A){
/*
A funlção change_to_Tfloat deve converter o tipo de dado do valor real para Tfloat;
Caso o tipo de A não seja Tdouble, nada deve ser feito.
*/
}

double real_to_double(real A){
/*
A função real_to_double retorna um valor double a partir de um real;
*/
    if(A.type == Tfloat){
        change_to_Tdouble(&A);
    }
    return * ((double*) A.data);
}


float real_to_float(real A){
/*
A função real_to_float retorna um valor float a partir de um real;
*/
    if(A.type == Tdouble){
        change_to_Tfloat(&A);
    }
    return * ((float*) A.data);
}


int main()
{
    double x = 13.125;

    real r1, r2;
    r1 = novo_real(x, Tfloat);
    r2 = novo_real(x, Tdouble);
    
    fIEEE *as_float;
    dIEEE *as_double;
    
    real sqrtR1 = sqrtNR(r1); // essa chamada da função deve retornar um valor real com type == Tfloat, pois r1.type == Tfloat 
    real sqrtR2 = sqrtNR(r2); // essa chamada da função deve retornar um valor real com type == Tdouble, pois r2.type == Tdouble
    
    float sqrt_xF = real_to_float(sqrtR1);
    double sqrt_xD = real_to_double(sqrtR2);

    printf("float sqrt(%lf):%f\n",x,sqrt_xF);
    printf("double sqrt(%lf):%lf\n",x,sqrt_xD);

    as_float = (fIEEE*)r1.data;
    as_double = (dIEEE*)r2.data;
    
    printf("sinal:%x mantissa:%x expoente:%x\n",as_float->s, as_float->f, as_float->E);
    printf("sinal:%llx mantissa:%llx expoente:%llx\n",as_double->s, as_double->f, as_double->E);
    
    exclui_real(&r1);
    exclui_real(&r2);

    return 0;
}
