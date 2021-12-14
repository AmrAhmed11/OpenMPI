// Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdio.h"
#include "mpi.h"
#include "time.h"
#include "omp.h"

int signCalc(int count){
    int sign;
    if(count%2==0){
        sign=1;
    }
    else{
        sign=-1;
    }
    return sign;
}

long double numCalc(long double count,long double x){
    long double num = 1;
    for(int i =0;i<count;i++){
        num *=x*x;
    }
    return num;
}

long double denCalc(long double j){
    long double den=1;
    for(int i=2;i<=(2*j);i++){
        den*=i;
    }
    return den;
}

long double sequentialSum(long double i, long double x) {
    int sign;
    long double numerator;
    long double denominator;
    
    long double value =0;
    for(int j=0;j<=i;j++){
        sign = signCalc(j);
        numerator = numCalc(j,x);
        denominator = denCalc(j);
        value += ((sign * numerator)/denominator);
    }
    return value;


    // long double sign = -1;
    // long double numerator =  x * x;
    // long double denominator = 2;
    // long double value = 1;
    // for (long double j = 0+rank*5; j <= i; j/size) {
    //     value += (sign * numerator) / denominator;
    //     numerator *=  x * x;
    //     denominator *= (2.0 * j + 1) * (2.0 * j + 2);
    //     sign *= -1;
    // }
    // return value;
}

long double parallelSum(int i,int x,int rank,int size){
    int sign;
    long double numerator;
    long double denominator;
    long double value =0;
    for(int j=rank;j<=i;j=j+size){
        sign = signCalc(j);
        numerator = numCalc(j,x);
        denominator = denCalc(j);
        value += ((sign * numerator)/denominator);
    }
    return value;




    // double sign = -1;
    // double numerator =  x * x;
    // double denominator = 2;
    // double value = 1;
    // for (int j = 1; j <= i; j++) {
    //     value += (sign * numerator) / denominator;
    //     numerator *=  x * x;
    //     denominator *= (2.0 * j + 1) * (2.0 * j + 2);
    //     sign *= -1;
    // }
    // return value;
}

int main(int argc , char **argv)
{
    long double x,i;
    int size,rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    setbuf(stdout, NULL);

    if(rank==0){
    // printf("Size = %d \n ", size);
    // printf("Rank = %d \n ", rank);

    printf("Enter i Value: ");
    scanf("%Lf",&i);
    printf("\n");

    printf("Enter x Value: ");
    scanf("%Lf",&x);
    printf("\n");


    }
    MPI_Bcast(&i,1,MPI_LONG_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(&x,1,MPI_LONG_DOUBLE,0,MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    long double beginParSeq = clock();
    long double ans1 = parallelSum(i, x,rank,size);
    long double endParSeq = clock();
    long double finalAns;
    long double duration = (endParSeq - beginParSeq)/CLOCKS_PER_SEC;
    long double longestDuration;
    MPI_Reduce(&duration, &longestDuration, 1, MPI_LONG_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&ans1, &finalAns, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank==0){
        long double beginSeq = clock();
        long double ans2 = sequentialSum(i, x);
        long double endSeq = clock();
        long double time_spent = (endSeq - beginSeq) / CLOCKS_PER_SEC;
        printf("Sequential Sum Value = %Lf \n", ans2);
        printf("Sequential Time spent = %Lf sec \n", time_spent);
        printf("Parallel Sum Value = %Lf \n", finalAns);
        printf("Parallel Time spent = %Lf sec \n", longestDuration);
    }
    //return value;
    MPI_Finalize();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
