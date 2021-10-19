
#include "HybridAnomalyDetector.h"
#include "minCircle.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::create_cf(list<std::pair<std::string, std::vector<float>>>::iterator itr,
list<std::pair<std::string, std::vector<float>>>::iterator itr2, float correlationValue,
const int S){
		SimpleAnomalyDetector::create_cf(itr, itr2, correlationValue, S);
		if (this->check_correlation_of_circle(correlationValue)) {
		correlatedFeatures cf1;
		float max_dev = 0, dev_temp;
		cf1.feature1 = itr->first;
		cf1.feature2 = itr2->first;
		cf1.corrlation = correlationValue;
		Point* ps[S];
		for(int i = 0 ;i < S; i++)
		ps[i]=new Point(itr->second.data()[i], itr2->second.data()[i]);
		cf1.c_reg = findMinCircle(ps, S);
		max_dev  = cf1.c_reg.radius;
		cf1.threshold = max_dev * 1.1;
		SimpleAnomalyDetector::cf.push_back(cf1);
		}
		return;
}

bool HybridAnomalyDetector::check_correlation_of_circle(float c) {

	 if ((c <= this->threshold && c >= 0.5) || (c >= -this->threshold && c <= -0.5)){
	 	return true;}
	return false;
	
}

vector<AnomalyReport> HybridAnomalyDetector::createAnomalyReport(std::vector<correlatedFeatures>::iterator itr, const int S,
 Point** ps, string s1, string s2, 	vector<AnomalyReport> r) {
	if (check_correlation_of_circle(itr->corrlation)){
        for (int j =0; j < S; j++){
		float dev_temp = distance(*ps[j], itr->c_reg.center);
		if (dev_temp > itr->threshold || -dev_temp < -itr->threshold){
			AnomalyReport a(s1 + "-" + s2, j+1); 
			r.push_back(a);
		}
		}
		}
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

float HybridAnomalyDetector::distance(Point a, Point b){
	float x2=(a.x-b.x)*(a.x-b.x);
	float y2=(a.y-b.y)*(a.y-b.y);
	return sqrt(x2+y2);
}

