//
// Created by yewei on 17-10-21.
//

#ifndef FFT_ALGORITHM_FFT_H
#define FFT_ALGORITHM_FFT_H
#include <iostream>
#include "cmath"
#include <bitset>
#include <time.h>
#define SIZE 1024*16
struct Complex_{
    double real;
    double imagin;
};
typedef Complex_ Complex;

int test_print();
void ShowAll_Array(double inp[],int count);
int BitReverse_Array(double * inp,int count);
void FFT(double * src,Complex * dest,int size_n);
int FFT_Run_Test();
#endif //FFT_ALGORITHM_FFT_H

