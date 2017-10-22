//
// Created by yewei on 17-10-21.
//
#include "FFT.h"
using namespace std;
#define FFT_ORDER_COUNT 16 //修改测试点数
double Test_Array[FFT_ORDER_COUNT]; //测试傅里叶算法的数组

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



void FFT(double * src,Complex * dest,int size_n){
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
    } //k的奇数偶数判断
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    for(int i=0;i<size_n;i++){
        dest[i].real=src[i];
        dest[i].imagin=0;
    }
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                //计算Wn
                Get_W_n(z, size_n, &wn);
                //Wn和x相乘
                Mul_Complex(&dest[j], &wn,&dest[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                temp.real=dest[neighbour].real;
                temp.imagin=dest[neighbour].imagin;
                //蝶形运算
                Add_Complex(&temp, &dest[j], &dest[neighbour]);
                Sub_Complex(&temp, &dest[j], &dest[j]);
            }
            else
                z=0;
        }
    }
    end=clock();
    printf("DFT use time :%lf for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size_n);
}


int FFT_Run_Test(){ //快速傅里叶测试函数
    double time_pass;
    Complex dest[FFT_ORDER_COUNT];
    Make_Test_sequence(Test_Array,FFT_ORDER_COUNT); //0-x的数组
    cout<<"The Original Array X(n) is:"<<endl;
    ShowAll_Array(Test_Array,FFT_ORDER_COUNT);
    FFT(Test_Array,dest,FFT_ORDER_COUNT); //进行傅里叶变化
    showAll_Array_Complex(dest,FFT_ORDER_COUNT); //解开注释就可以显示傅里叶变换对

    return 0;
}
