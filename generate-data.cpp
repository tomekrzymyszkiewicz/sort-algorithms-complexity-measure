#include<stdio.h>
#include<random>
#include<ctime>
#include<iostream>
using namespace std;


int* array_with_random_numbers(int size, int min,int max){
    int* array = new int[size];
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        array[i] = (rand() % (2*max+1)) + min;
    }
    return array;
}

int main(){
    
    return 0;
}