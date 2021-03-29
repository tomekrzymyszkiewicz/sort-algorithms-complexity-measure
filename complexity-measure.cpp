#include<stdio.h>
#include<random>
#include<ctime>
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

vector<vector<string>> tasks;

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
 
    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
void quick_sort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quick_sort(arr, low, pivot - 1);
        quick_sort(arr, pivot + 1, high);
    }
}

int* array_with_random_numbers(int size, int min,int max){
    int* array = new int[size];
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        array[i] = (rand() % (2*max+1)) + min;
    }
    return array;
}

bool load_config(){
    cout<<"Loading config.ini"<<endl;
    ifstream fin;
    fin.open("config.ini",ios::in);
    if(!fin.good()){
        cout<<"Config.ini not found"<<endl;
        fin.close();
        return false;
    }
    vector<string> row;
    string line;
    while(!fin.eof()){
        string sort_algorithm, size_of_structure, min_value, max_value, number_of_repeats;
        fin>>sort_algorithm>>size_of_structure>>min_value>>max_value>>number_of_repeats;
        if(sort_algorithm.size() == 0 || size_of_structure.size() == 0 || min_value.size() == 0 || max_value.size() == 0 || number_of_repeats.size() == 0){
            break;
        }
        vector<string> task;
        task.push_back(sort_algorithm);
        task.push_back(size_of_structure);
        task.push_back(min_value);
        task.push_back(max_value);
        task.push_back(number_of_repeats);
        tasks.push_back(task);
    }
    fin.close();
    cout<<"Config loaded correctly"<<endl;
    return true;
}

int main(){
    load_config();
    if(tasks.size() == 0){
        cout<<"No tasks found to be performed."<<endl;
    }else{
        for(int i = 0;i < tasks.size(); i++){
            string sort_algorithm = tasks[i][0];
            int size_of_structure = stoi(tasks[i][1]);
            int min_value = stoi(tasks[i][2]);
            int max_value = stoi(tasks[i][3]);
            int number_of_repeats = stoi(tasks[i][4]);
            cout<<"Sorting "<<sort_algorithm<<" array of "<<size_of_structure<<" elements in range from "<<min_value<<" to "<<max_value<<","<<number_of_repeats<<" times"<<endl;
            if(size_of_structure<1){
                cout<<"Cannot execute task. The array must to have at least 1 element.";
            }else if(min_value<INT_MIN){
                cout<<"Cannot execute task. The minimum value is outside the range of an integer.";
            }else if(max_value>INT_MAX){
                cout<<"Cannot execute task. The maximum value is outside the range of an integer.";
            }else if(number_of_repeats<1){
                cout<<"Cannot execute task. The minimum number of repetitions is 1.";
            }else{
                if(sort_algorithm == "quick_sort"){
                    for(int j = 1; i <= number_of_repeats; i++){
                        cout<<"Generating an array with "<<size_of_structure<<" elements."<<endl;
                        int* test_array = array_with_random_numbers(size_of_structure,min_value,max_value);
                        cout<<"Array created. Sorting started."<<endl;
                        quick_sort(test_array,0,(size_of_structure-1));
                        cout<<"Sorting finished."<<endl;
                    }
                }else if(sort_algorithm == "counting_sort"){

                }else{
                    cout<<"Cannot recognize "<<sort_algorithm<<" algorithm.";
                }
            }
        }
    }
    
    return 0;
}