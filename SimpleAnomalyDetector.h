/*
  Daniel Reuveni
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <list>


struct correlatedFeatures{
	string feature1, feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg; 
	Circle c_reg;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	protected: vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	float max(float c1, float c2, float c3);
	virtual bool check_correlation(float c);
	bool check_lower_correlation(float c);
	virtual void create_cf(list<std::pair<std::string, std::vector<float>>>::iterator itr,
	list<std::pair<std::string, std::vector<float>>>::iterator itr2, float correlationValue,
	const int S);
	virtual vector<AnomalyReport> createAnomalyReport(std::vector<correlatedFeatures>::iterator itr, const int S,
	 Point** ps, string s1, string s2, vector<AnomalyReport> r);
	float getThreshold();
	void setThreshold(float value);


	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
