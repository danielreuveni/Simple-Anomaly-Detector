/*
Daniel Reuveni
*/


#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "minCircle.h"
#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	const int S = ts.getInfo()[0].second.size();
	int N = ts.getInfo().size();
	list<string> stringNames;
	list<std::pair<std::string, std::vector<float>>> columns;

	for (int i =0; i < N; i++){
		stringNames.push_back(ts.getInfo()[0].first);
	}

	for (int i = 0; i < N; i++){
		std::pair<std::string, std::vector<float>> a = ts.getCol(i);
		columns.push_back(a);
	}

	float correlation_arr[N][N];
	list<std::pair<std::string, std::vector<float>>>::iterator itr = columns.begin();
	list<std::pair<std::string, std::vector<float>>>::iterator itr2 = columns.begin();
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
		if (j > i) {
		correlation_arr[i][j] = pearson(itr->second.data(), itr2->second.data(), S);
	}	
	std::advance(itr2, 1);
	if (j == N-1)
	{
		itr2 = columns.begin();
	}
	}
	std::advance(itr, 1);
	}

	itr = columns.begin();
	itr2 = columns.begin();
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++) {	
			if (j > i) {				
		create_cf(itr, itr2, correlation_arr[i][j], S);
	
	}
	std::advance(itr2, 1);
	if (j == N-1){
		itr2 = columns.begin();
	}
	}
	std::advance(itr, 1);
}

}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	// TODO Auto-generated destructor stub
	vector<AnomalyReport> r;
	const int S = ts.getInfo()[0].second.size();
	Point* ps[S];
	std::vector<float> vec1;
	std::vector<float> vec2;	
	string s1, s2;
	

    for (auto itr=this->cf.begin(); itr != this->cf.end(); itr++){

			int i = 0;
		for (auto itr2 = ts.getInfo().begin(); itr2 !=  ts.getInfo().end(); itr2++){
			vec1.clear();
			vec2.clear();
			if (ts.getInfo()[i].first.compare(itr->feature1) == 0) {
				for (int k =0; k < S; k++){
					vec1.push_back(ts.getInfo()[i].second[k]);
				}
			s1 = ts.getInfo()[i].first;
			}

			if (ts.getInfo()[i].first.compare(itr->feature2) == 0) {
			for (int k =0; k < S; k++)
					vec2.push_back(ts.getInfo()[i].second[k]); 
			s2 = ts.getInfo()[i].first; 
			}
			i++; 
    }
	
		
		for (int i = 0; i < S; i++){
		ps[i]=new Point(vec1.data()[i], vec2.data()[i]);
		}
		r = createAnomalyReport(itr, S, ps, s1 ,s2, r);
    }
	
	return r;
	
}

float SimpleAnomalyDetector::max(float c1, float c2, float c3) {
	 if ( c1 > c2 && c1 > c3 )
        return c1;
    else if ( c2 > c1 && c2 > c3 )
        return c2;
    else if ( c3 > c1 && c3 > c2 )
        return c3;

	return 0;
}

bool SimpleAnomalyDetector::check_correlation(float c) {
	 if (c > this->threshold || c < -this->threshold)
	 	return true;
	return false;
	
}

bool SimpleAnomalyDetector::check_lower_correlation(float c) {
	 if (c > this->threshold || c < -this->threshold)
	 	return true;
	return false;
	
}

void SimpleAnomalyDetector::create_cf(list<std::pair<std::string, std::vector<float>>>::iterator itr,
list<std::pair<std::string, std::vector<float>>>::iterator itr2, float correlationValue,
const int S){
	if (this->check_correlation(correlationValue)) {
		correlatedFeatures cf1;
		float max_dev = 0, dev_temp;
		cf1.feature1 = itr->first;
		cf1.feature2 = itr2->first;
		cf1.corrlation = correlationValue;
		Point* ps[S];
		for(int i = 0 ;i < S; i++)
		ps[i]=new Point(itr->second.data()[i], itr2->second.data()[i]);
		cf1.lin_reg = linear_reg(ps, S);
		for(int i = 0 ; i < S; i++){
			dev_temp = dev(*ps[i], cf1.lin_reg);
			if (dev_temp > max_dev)
				max_dev  = dev_temp;
		}
		cf1.threshold = max_dev * 1.1;
		this->cf.push_back(cf1);
	}
		return;
}


vector<AnomalyReport> SimpleAnomalyDetector::createAnomalyReport(std::vector<correlatedFeatures>::iterator itr, const int S,
 Point** ps, string s1, string s2, 	vector<AnomalyReport> r) {
	if (check_correlation(itr->corrlation)){
        for (int j =0; j < S; j++){
		float dev_temp = dev(*ps[j], itr->lin_reg);
		if (dev_temp > itr->threshold || -dev_temp < -itr->threshold){
			AnomalyReport a(s1 + "-" + s2, j+1); 
			r.push_back(a);
		}
		}
		}
		return r;
}

float SimpleAnomalyDetector::getThreshold(){
	return this->threshold;
}

void SimpleAnomalyDetector::setThreshold(float value) {
	this->threshold = value;
}
