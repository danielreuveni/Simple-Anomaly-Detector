/*
Daniel Reuveni
 */

#include "timeseries.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <ostream>

using namespace std;

//members   
const char* file_name;
std::vector<std::pair<std::string, std::vector<float>>> info;
//constructor
TimeSeries::TimeSeries(const char* CSVfileName){
    this->file_name = CSVfileName;
    read_file();
	}


std::vector<std::pair<std::string, std::vector<float>>> TimeSeries::read_file() {
std::ifstream fin;
fin.open(this->file_name);
std::string row, col;
float val;
std::getline(fin, row);
std::stringstream string_stream(row);
while(std::getline(string_stream, col, ',')){
    this->info.push_back({col, std::vector<float>{}});
     }

  while(std::getline(fin, row))
    {
        std::stringstream string_stream1(row);
        int index = 0;
        while(string_stream1 >> val)
        {
            this->info.at(index).second.push_back(val);
            if(string_stream1.peek() == ','){
                string_stream1.ignore();
            }
            index++;
        }
    }
    
 fin.close();

 return this->info;

}

std::vector<std::pair<std::string, std::vector<float>>> TimeSeries::getInfo() const{
    return this->info;
}

std::string TimeSeries::getNameCol(int k) const{
    for (int i = 0; i < info.size(); i++)
        if (i == k)
    return 0;
return "0";
}

std::pair<std::string, std::vector<float>> TimeSeries::getCol(int k) const {
    for (int i = 0; i < info.size(); i++)
        if (i == k)
        return info[i];
return {};
}

void addLine(double arr[], int numLine) {
    int i = 0;
    for (auto itr=info.begin(); itr != info.end(); itr++){
        itr->second.insert(itr->second.begin() + numLine, arr[0]);
        
    }
}

int TimeSeries::getSizeRow(){
    return info.size();
}

void TimeSeries::print() {
cout << "The vector elements are: " << endl; 
for (auto itr = info.begin(); itr != info.end(); ++itr){
    for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        std::cout << *itr2 << " ";
        cout << endl;
}
}

;
