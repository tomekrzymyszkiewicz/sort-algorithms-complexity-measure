#include<stdio.h>
#include<random>
#include<ctime>
#include<iostream>
#include<string>
#include<fstream>
#include<chrono>
#include <cstring>
#include <algorithm>
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
    double time_memory;
    int number_of_repetitions;
    string type_of_result;
    Result(string sort_algorithm, int size_of_array, double time_memory, int number_of_repetitions, string type_of_result){
        this->sort_algorithm = sort_algorithm;
        this->size_of_array = size_of_array;
        this->time_memory = time_memory;
        this->number_of_repetitions = number_of_repetitions;
        this->type_of_result = type_of_result;
    }
    string toString(){
        return(sort_algorithm+","+to_string(size_of_array)+","+to_string(time_memory)+","+to_string(number_of_repetitions)+","+type_of_result);
    }
};

void save_results(string results_file_name){
    cout<<"Saving results"<<endl;
    fstream fout;
    fout.open(results_file_name,ios::out);
    fout<<"sort_algorithm,size_of_array,time_of_sort_s/memory_usage,number_of_repetitions,type_of_test"<<endl;
    for(int i = 0; i < results.size(); i++){
        fout<<results[i]<<endl;
    }
    fout.close();
    cout<<"Correctly saved "<<results.size()<<" results"<<endl;

}

void swap_m(int* a, int* b, int* used_memory)
{
    int temp = *a;
    used_memory += sizeof(temp);
    *a = *b;
    *b = temp;
}

int partition_m(int arr[], int low, int high, int* used_memory)
{
    int pivot = arr[high];
    *used_memory += sizeof(pivot);
    int i = (low - 1);
    *used_memory += sizeof(i);
    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap_m(&arr[i], &arr[j], used_memory);
        }
    }
    swap_m(&arr[i + 1], &arr[high], used_memory);
    return (i + 1);
}
 
void quick_sort_m(int arr[], int low, int high, int* used_memory)
{
    if (low < high)
    {
        int pivot = partition_m(arr, low, high, used_memory);
        *used_memory += sizeof(pivot);
        quick_sort_m(arr, low, pivot - 1, used_memory);
        quick_sort_m(arr, pivot + 1, high, used_memory);
    }
}

void swap_t(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition_t(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
 
    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            swap_t(&arr[i], &arr[j]);
        }
    }
    swap_t(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
void quick_sort_t(int arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = partition_t(arr, low, high);
        quick_sort_t(arr, low, pivot - 1);
        quick_sort_t(arr, pivot + 1, high);
    }
}

void counting_sort_t(int array[],int size_of_array,int max_value){
    int* output = new int[size_of_array];
    int* count = new int[max_value + 1];
    int i;
    memset(count, 0, (max_value+1)*sizeof(int));
    for(i = 0; i < size_of_array; i++)
        count[array[i]]++;
 
    for(i = 1; i <= max_value; i++)
        count[i] += count[i - 1];
 
    for(i = size_of_array-1; i>=0; i--){
        count[array[i]]--;
        output[count[array[i]]] = array[i];
    }
    memcpy(array,output,size_of_array*sizeof(int));
    delete output;
    delete count;
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
        string sort_algorithm, test_type, min_size, max_size, step, number_of_repeats;
        fin>>sort_algorithm>>test_type>>min_size>>max_size>>step>>number_of_repeats;
        if(sort_algorithm.size() == 0 || min_size.size() == 0 || max_size.size() == 0 || step.size() == 0 || number_of_repeats.size() == 0){
            break;
        }
        vector<string> task;
        task.push_back(sort_algorithm);
        task.push_back(test_type);
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
            string test_type = tasks[i][1];
            int min_size = stoi(tasks[i][2]);
            int max_size = stoi(tasks[i][3]);
            int step = stoi(tasks[i][4]);
            int number_of_repeats = stoi(tasks[i][5]);
            cout<<"Sorting "<<sort_algorithm<<" arrays with numbers ranging from "<<min_size<<" to "<<max_size<<", repeated "<<number_of_repeats<<" times"<<endl;
            if(min_size<1){
                cout<<"Cannot execute task. The array must to have at least 1 element.";
            }else if(number_of_repeats<1){
                cout<<"Cannot execute task. The minimum number of repetitions is 1.";
            }else{
                if(sort_algorithm == "quick_sort"){
                    if(test_type == "m"){
                        for(int current_size = min_size; current_size <= max_size; current_size+=step){
                            cout<<"Memory test. Sorting an array with "<<current_size<<" elements. "<<number_of_repeats<<" repeats of task."<<endl;
                            int* used_memory = new int;
                            for(int k = 1; k <= number_of_repeats; k++){
                                int* test_array = fill_array_with_data(current_size);
                                *used_memory += sizeof(test_array);
                                quick_sort_m(test_array,0,(current_size-1),used_memory);
                                delete test_array;
                            }
                            cout<<"Sorting finished."<<endl;
                            Result quick_sort_memory_result = Result("quick_sort",current_size,(double)*used_memory,number_of_repeats,"memory");
                            results.push_back(quick_sort_memory_result.toString());
                            delete used_memory;
                        }
                    }else if(test_type == "t"){
                        for(int current_size = min_size; current_size <= max_size; current_size+=step){
                            using namespace std::chrono;
                            cout<<"Time test. Sorting an array with "<<current_size<<" elements. "<<number_of_repeats<<" repeats of task."<<endl;
                            high_resolution_clock::time_point t_start = high_resolution_clock::time_point();
                            high_resolution_clock::time_point t_end = high_resolution_clock::time_point();
                            duration<double> time_span = duration<double>(0);
                            for(int k = 1; k <= number_of_repeats; k++){
                                int* test_array = fill_array_with_data(current_size);
                                t_start = high_resolution_clock::now();
                                quick_sort_t(test_array,0,(current_size-1));
                                t_end = high_resolution_clock::now();
                                time_span += duration_cast<duration<double>>(t_end - t_start);
                                delete test_array;
                            }
                            cout<<"Sorting finished."<<endl;
                            Result quick_sort_time_result = Result("quick_sort",current_size,time_span.count(),number_of_repeats,"time");
                            results.push_back(quick_sort_time_result.toString());
                        }
                    }else{
                        cout<<"Wrong test type."<<endl;
                    }
                }else if(sort_algorithm == "counting_sort"){
                    if(test_type == "m"){
                        // for(int current_size = min_size; current_size <= max_size; current_size+=step){
                        //     cout<<"Memory test. Sorting an array with "<<current_size<<" elements. "<<number_of_repeats<<" repeats of task."<<endl;
                        //     int* used_memory = new int;
                        //     for(int k = 1; k <= number_of_repeats; k++){
                        //         int* test_array = fill_array_with_data(current_size);
                        //         *used_memory += sizeof(test_array);
                        //         quick_sort_m(test_array,0,(current_size-1),used_memory);
                        //         delete test_array;
                        //     }
                        //     cout<<"Sorting finished."<<endl;
                        //     Result quick_sort_memory_result = Result("quick_sort",current_size,(double)*used_memory,number_of_repeats,"memory");
                        //     results.push_back(quick_sort_memory_result.toString());
                        //     delete used_memory;
                        // }
                    }else if(test_type == "t"){
                        for(int current_size = min_size; current_size <= max_size; current_size+=step){
                            using namespace std::chrono;
                            cout<<"Time test. Sorting an array with "<<current_size<<" elements. "<<number_of_repeats<<" repeats of task."<<endl;
                            high_resolution_clock::time_point t_start = high_resolution_clock::time_point();
                            high_resolution_clock::time_point t_end = high_resolution_clock::time_point();
                            duration<double> time_span = duration<double>(0);
                            for(int k = 1; k <= number_of_repeats; k++){
                                int* test_array = fill_array_with_data(current_size);
                                int max_value = *std::max_element(test_array,test_array+current_size);
                                printArray(test_array,current_size);
                                t_start = high_resolution_clock::now();
                                counting_sort_t(test_array,current_size,max_value);
                                t_end = high_resolution_clock::now();
                                printArray(test_array,current_size);
                                time_span += duration_cast<duration<double>>(t_end - t_start);
                                delete test_array;
                            }
                            cout<<"Sorting finished."<<endl;
                            Result quick_sort_time_result = Result("quick_sort",current_size,time_span.count(),number_of_repeats,"time");
                            results.push_back(quick_sort_time_result.toString());
                        }
                    }else{
                        cout<<"Wrong test type."<<endl;
                    }
                }else{
                    cout<<"Cannot recognize "<<sort_algorithm<<" algorithm.";
                }
            }
        }
    }
    save_results(results_file_name);
    return 0;
}