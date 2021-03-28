#include<stdio.h>
#include<random>
#include<ctime>
#include<iostream>
#include<fstream>
using namespace std;
struct config
{
        string file_name;
        int size;
        int min;
        int max;
};

int* array_with_random_numbers(int size, int min,int max){
    int* array = new int[size];
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        array[i] = (rand() % (2*max+1)) + min;
    }
    return array;
}

config load_config(){
    cout<<"Loading data-config.ini"<<endl;
    struct config loaded_config = {"",0,0,0};
    ifstream fin;
    fin.open("data-config.ini",ios::in);
    if(!fin.good()){
        cout<<"Config.ini not found"<<endl;
        fin.close();
        return loaded_config;
    }
    fin >> loaded_config.file_name >> loaded_config.size >> loaded_config.min >> loaded_config.max;
    fin.close();
    cout<<"Config loaded correctly"<<endl;
    return loaded_config;
}

bool generate_data_to_file(string file_name, int amount, int min, int max){
    cout<<"Generating "<<amount<<" numbers to data file "<<file_name<<endl;
    fstream fout;
    fout.open(file_name,ios::out);
    int* array = array_with_random_numbers(amount,min,max);
    for(int i = 0; i < amount; i++){
        fout<<array[i]<<"\n";
    }
    fout.close();
    cout<<"Data generated correctly"<<endl;
}

int main(){
    struct config loaded_config = load_config();
    generate_data_to_file(loaded_config.file_name, loaded_config.size, loaded_config.min, loaded_config.max);
    return 0;
}