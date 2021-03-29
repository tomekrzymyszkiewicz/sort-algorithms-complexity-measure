#include<stdio.h>
#include<random>
#include<ctime>
#include<iostream>
#include<string>
#include<fstream>
#include<chrono>
using namespace std;

vector<int> data_vector;
vector<vector<string>> tasks;
vector<string> results;
string data_file_name = "";
int data_amount = 0;
string results_file_name = "";

struct Result{
    string sort_algorithm;
    int size_of_array;
    double time_span;
    int number_of_repetitions;
    Result(string sort_algorithm, int size_of_array, double time_span, int number_of_repetitions){
        this->sort_algorithm = sort_algorithm;
        this->size_of_array = size_of_array;
        this->time_span = time_span;
        this->number_of_repetitions = number_of_repetitions;
    }
    string toString(){
        return(sort_algorithm+","+to_string(size_of_array)+","+to_string(time_span)+","+to_string(number_of_repetitions));
    }
};

void save_results(string results_file_name){
    cout<<"Saving results"<<endl;
    fstream fout;
    fout.open(results_file_name,ios::out);
    fout<<"sort_algorithm,size_of_array,time_of_sort_s,number_of_repetitions"<<endl;
    for(int i = 0; i < results.size(); i++){
        fout<<results[i]<<endl;
    }
    fout.close();
    cout<<"Correctly saved "<<results.size()<<" results"<<endl;

}

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

int* fill_array_with_data(int size){
    int* array = new int[size];
    for(int i = 0; i < size; i++){
        array[i] = data_vector[i];
    }
    return array;
}

void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

bool load_data(string file_name, int amount){
    cout<<"Loading data from "<<file_name<<" file"<<endl;
    ifstream fin;
    fin.open(file_name,ios::in);
    if(!fin.good()){
        cout<<"Data file "<<file_name<<" not exist"<<endl;
        fin.close();
        return false;
    }
    string line;
    int data_loaded = 0;
    for(int i = 0; i < amount; i++){
        fin>>line;
        if(!fin.eof()){
            data_vector.push_back(stoi(line));
        }else{
            cout<<"Not enough elements in data file "<<file_name<<endl;
            return false;
        } 
        data_loaded++;
    }
    cout<<"Loaded correctly "<<data_loaded<<" numbers"<<endl;
    fin.close();
    return true;
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
    fin >> data_file_name >> data_amount;
    fin >> results_file_name;
    while(!fin.eof()){
        string sort_algorithm, min_size, max_size, step, number_of_repeats;
        fin>>sort_algorithm>>min_size>>max_size>>step>>number_of_repeats;
        if(sort_algorithm.size() == 0 || min_size.size() == 0 || max_size.size() == 0 || step.size() == 0 || number_of_repeats.size() == 0){
            break;
        }
        vector<string> task;
        task.push_back(sort_algorithm);
        task.push_back(min_size);
        task.push_back(max_size);
        task.push_back(step);
        task.push_back(number_of_repeats);
        tasks.push_back(task);
    }
    fin.close();
    cout<<"Config loaded correctly"<<endl;
    return true;
}

int main(){
    load_config();
    if(!load_data(data_file_name,data_amount)){
        cout<<"Cannot load "<<data_amount<<" numbers from "<<data_file_name<<" file."<<endl;
    }
    if(tasks.size() == 0){
        cout<<"No tasks found to be performed."<<endl;
    }else{
        for(int i = 0;i < tasks.size(); i++){
            string sort_algorithm = tasks[i][0];
            int min_size = stoi(tasks[i][1]);
            int max_size = stoi(tasks[i][2]);
            int step = stoi(tasks[i][3]);
            int number_of_repeats = stoi(tasks[i][4]);
            cout<<"Sorting "<<sort_algorithm<<" arrays with numbers ranging from "<<min_size<<" to "<<max_size<<", repeated "<<number_of_repeats<<" times"<<endl;
            if(min_size<1){
                cout<<"Cannot execute task. The array must to have at least 1 element.";
            }else if(number_of_repeats<1){
                cout<<"Cannot execute task. The minimum number of repetitions is 1.";
            }else{
                if(sort_algorithm == "quick_sort"){
                    for(int current_size = min_size; current_size <= max_size; current_size+=step){
                        using namespace std::chrono;
                        cout<<"Sorting an array with "<<current_size<<" elements. "<<number_of_repeats<<" repeats of task."<<endl;
                        high_resolution_clock::time_point t_start = high_resolution_clock::time_point();
                        high_resolution_clock::time_point t_end = high_resolution_clock::time_point();
                        duration<double> time_span = duration<double>(0);
                        for(int k = 1; k <= number_of_repeats; k++){
                            int* test_array = fill_array_with_data(current_size);
                            t_start = high_resolution_clock::now();
                            quick_sort(test_array,0,(current_size-1));
                            t_end = high_resolution_clock::now();
                            time_span += duration_cast<duration<double>>(t_end - t_start);
                            delete test_array;
                        }
                        cout<<"Sorting finished."<<endl;
                        Result quick_sort_result = Result("quick_sort",current_size,time_span.count(),number_of_repeats);
                        results.push_back(quick_sort_result.toString());
                    }
                }else if(sort_algorithm == "counting_sort"){
                }else{
                    cout<<"Cannot recognize "<<sort_algorithm<<" algorithm.";
                }
            }
        }
    }
    save_results(results_file_name);
    return 0;
}