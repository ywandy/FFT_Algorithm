//
// Created by yewei on 17-10-21.
//
#include "FFT.h"
using namespace std;
#define FFT_ORDER_COUNT 4096
double Test_Array[FFT_ORDER_COUNT];
int Add_Complex(Complex * Src1,Complex * Src2,Complex * dest){
    dest->imagin = Src1->imagin+Src2->imagin;
    dest->real = Src1->real+Src2->real;
    return 0;
}


int Sub_Complex(Complex * Src1,Complex * Src2,Complex * dest){
    dest->imagin = Src1->imagin-Src2->imagin;
    dest->real = Src1->real-Src2->real;
    return 0;
}

int Mul_Complex(Complex * Src1,Complex * Src2,Complex * dest){
//    dest->real = (((Src1->real)*(Src2->real))-((Src1->imagin)*(Src2->imagin)));
//    dest->imagin = (((Src1->real)*(Src2->imagin))+((Src2->real)*(Src1->imagin))); //不知道为毛这样算不了
    double R1=0.0,R2=0.0;
    double I1=0.0,I2=0.0;
    R1=Src1->real;
    R2=Src2->real;
    I1=Src1->imagin;
    I2=Src2->imagin;
    dest->imagin=R1*I2+R2*I1;
    dest->real=R1*R2-I1*I2;
    return 0;
}


////////////////////////////////////////////////////////////////////
//在FFT中有一个WN的n次方项，在迭代中会不断用到，具体见算法说明
///////////////////////////////////////////////////////////////////
void getWN(double n,double size_n,Complex * dst){
    double x=2.0*M_PI*n/size_n;
    dst->imagin=-sin(x);
    dst->real=cos(x);
}

void Get_W_n(double n,double size_n,Complex * dst){
    double x=2.0*M_PI*n/size_n;
    dst->imagin=-sin(x);
    dst->real=cos(x);
}



int Make_Test_sequence(double inp[],int count){
    for(int loop=0;loop<count;loop++){
        *(inp+loop) = loop;
    }
}


int test_print(){
    Make_Test_sequence(Test_Array,8);
    ShowAll_Array(Test_Array,sizeof(Test_Array)/sizeof(*Test_Array));
    BitReverse_Array(Test_Array,8);
    ShowAll_Array(Test_Array,sizeof(Test_Array)/sizeof(*Test_Array));
    return 0;
}

void ShowAll_Array(double inp[],int count){
    for(int loop=0;loop<count;loop++){
        bitset<3>myset(*(inp+loop));
        cout<<"Array["<<loop<<"]:"<<*(inp+loop)<<" ";
        //cout<<"Array["<<loop<<"]:"<<myset<<" ";
    }
    cout<<endl;
}

void showAll_Array_Complex(Complex inp[],int count){
    for(int loop=0;loop<count;loop++){
        if(inp[loop].imagin>=0.0){
            cout<<inp[loop].real<<"+"<<inp[loop].imagin<<"j"<<endl;
        }else{
            cout<<inp[loop].real<<inp[loop].imagin<<"j"<<endl;
        }
    }
}

int BitReverse_Array(double * inp,int count){
    if(count==1)
        return 0;
    double * temp=(double *)malloc(sizeof(double)*count);
    for(int i=0;i<count;i++)
        if(i%2==0)
            temp[i/2]=inp[i];
        else
            temp[(count+i)/2]=inp[i];
    for(int i=0;i<count;i++)
        inp[i]=temp[i];
    free(temp);
    BitReverse_Array(inp, count/2);
    BitReverse_Array(inp+count/2, count/2);
}



void FFT(double * src,Complex * dst,int size_n,double *passtime_t){
    BitReverse_Array(src, size_n); //数组倒序
    cout<<"Change the Array Order x1(n) is:"<<endl;
    ShowAll_Array(src,size_n);
    cout<<"Make FFT ： F{x(n)} is :"<<endl;
    /***fft core**/
    int k=size_n;
    int z=0;
    clock_t start,end;
    start=clock();
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);
    if(src_com==NULL)
        exit(0);
    for(int i=0;i<size_n;i++){
        src_com[i].real=src[i];
        src_com[i].imagin=0;
    }
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                Get_W_n(z, size_n, &wn);
                Mul_Complex(&src_com[j], &wn,&src_com[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imagin=src_com[neighbour].imagin;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
    }
    for(int i=0;i<size_n;i++){
        dst[i].imagin=src_com[i].imagin;
        dst[i].real=src_com[i].real;
    }
    end=clock();
    printf("DFT use time :%lf for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size_n);
}


int FFT_Run_Test(){ //快速傅里叶测试函数
    double time_pass;
    Complex dest[FFT_ORDER_COUNT];
    Make_Test_sequence(Test_Array,FFT_ORDER_COUNT); //0-16的数组
    cout<<"The Original Array X(n) is:"<<endl;
    ShowAll_Array(Test_Array,FFT_ORDER_COUNT);
    FFT(Test_Array,dest,FFT_ORDER_COUNT,&time_pass);
    //showAll_Array_Complex(dest,FFT_ORDER_COUNT);

    return 0;
}
