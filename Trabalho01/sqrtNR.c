#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef union floatIEEE{
    float x; /// Argument for SQRT (sqrt(A))
    struct {
 // a ordem de definição dos campos é importante}
        unsigned int f : 23; // 23 bits para a fração da mantissa (0.xxx...)
        unsigned int E : 8; // 8 bits para o expoente enviezado por B = 127
        unsigned int s : 1; // 1 bit para o sinal da mantissa
    }bits;
}floatIEEE;

////   Newton-Rapson
float floatNRsqrt(float A){
    ///
    if (A==0.0 || A==1.0) return A; ////

    const int B = 127; //valor do bias para float
    const double sqrt2 =  1.4142135623730950488016887;
    int k = 0;

    floatIEEE x0 , x1;

    x0.x = A;
    int impar = !(x0.bits.E & 1); /// expoente verdadeiro é ímpar???
    x0.bits.E = ((x0.bits.E>>1) + 0x3F ); /// cuidado com o double
    x0.bits.f >>= 1; /// metade da fração da mantissa
    if (impar) {
        x0.x *= sqrt2;
    }

    printf("\n  E  = %d ", x0.bits.E);
    printf("\n  f  = %d ", x0.bits.f);
    printf("\n  f  = %f ", (float)x0.bits.f/(1<<23));
    printf("\n  s  = %d ", x0.bits.s);


    printf("\n  Estimativa Inicial x0.x  = %f  ", x0.x );

     x1.x = x0.x;

    do { /// newton-rapson   /// dessa maneira, se X.x0 == 0.0
                             /// temos problemas em usar while-do
            x0.x = x1.x; /// estimativa atual
            x1.x = (x0.x + A/x0.x); /// próxima estimativa
/// resolver a multiplicação por (1/2)
            x1.bits.E--; /// x1.bits.E = x1.bits.E – 1;
            k++;  /// contador de iterações
    } while (x0.x != x1.x);
     printf("\n  Total de iteracoes  = %d ", k );
    return x1.x;
}

void main(void){
    float A = 10000.0;
    float r = floatNRsqrt(A);
    printf("\n  floatNRsqrt(%f)  = %f ", A, r);

}
