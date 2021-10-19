/*
  Daniel Reuveni
 */

#include <map>
#include <vector>



#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries{
const char* file_name;
std::vector<std::pair<std::string, std::vector<float>>> info;

public:

	TimeSeries(const char* CSVfileName);
	std::vector<std::pair<std::string, std::vector<float>>> read_file();
	std::vector<std::pair<std::string, std::vector<float>>> getInfo() const;
	std::pair<std::string, std::vector<float>> getCol(int k) const;
	std::string getNameCol(int k) const;
	void addLine(int arr[], int numLine);
	void print();
	int getSizeRow();




};
#endif /* TIMESERIES_H_ */
