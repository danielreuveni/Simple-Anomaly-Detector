
#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
	//vector<correlatedFeatures> cf;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	void create_cf(list<std::pair<std::string, std::vector<float>>>::iterator itr,
	list<std::pair<std::string, std::vector<float>>>::iterator itr2, float correlationValue,
	const int S);
	bool check_correlation_of_circle(float c);
	vector<AnomalyReport> createAnomalyReport(std::vector<correlatedFeatures>::iterator itr, const int S,
 	Point** ps, string s1, string s2, vector<AnomalyReport> r);
	float distance(Point a, Point b);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
