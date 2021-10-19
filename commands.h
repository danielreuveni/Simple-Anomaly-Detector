
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

};
struct fixdReport{
	int start;
	int end;
	string description;
	bool tp;
};
struct anomalyReportTemp{
	float threshold = 0.9;
	vector<AnomalyReport> ar;
	vector<fixdReport> fixdRports;
	int testFileSize;
};


class Command{
	DefaultIO* dio;
	string description;
public:
	virtual void printCommand()=0;
	virtual ~Command(){}
	virtual string getDescription()=0;
};

class uploadCommand:public Command{
	DefaultIO* dio;
	string description ="1.upload a time series csv file\n" ;
public:
	uploadCommand(DefaultIO* dio):dio(dio){}
  	virtual ~uploadCommand(){};
	virtual void execute(anomalyReportTemp* art);
	void printCommand(){
			cout << this->description << endl;

	};
	string getDescription(){
		return this->description;
	}
};

class algorithmCommand:public Command{
	DefaultIO* dio;
	string description ="2.algorithm settings\n" ;
public:
	algorithmCommand(DefaultIO* dio):dio(dio){}
  	virtual ~algorithmCommand(){};
	virtual void execute(){};
	virtual void execute(anomalyReportTemp* art);
	void printCommand(){
			cout << this->description << endl;

	};
	string getDescription(){
		return this->description;
	}
};

class detectCommand:public Command{
	DefaultIO* dio;
	string description ="3.detect anomalies\n" ;
public:
	detectCommand(DefaultIO* dio):dio(dio){}
  	virtual ~detectCommand(){};
	virtual void execute(){};
	virtual void execute(anomalyReportTemp* art);
	void printCommand(){
		cout << this->description << endl;
	};
	string getDescription(){
		return this->description;
	}
};

class displayCommand:public Command{
	DefaultIO* dio;
	string description ="4.display results\n" ;
public:
	displayCommand(DefaultIO* dio):dio(dio){}
  	virtual ~displayCommand(){};
	virtual void execute(){};
	virtual void execute(anomalyReportTemp* art);
	void printCommand(){
			cout << this->description << endl;

	};
	string getDescription(){
		return this->description;
	}
};

class uploadAndAnalizeCommand:public Command{
	DefaultIO* dio;
	string description ="5.upload anomalies and analyze results\n" ;
public:
	uploadAndAnalizeCommand(DefaultIO* dio):dio(dio){}
  	virtual ~uploadAndAnalizeCommand(){};
	virtual void execute(){};
	void printCommand(){
			cout << this->description << endl;

	};
	string getDescription(){
		return this->description;
	}
	bool crossSection(int as,int ae,int bs, int be){
		return (ae>=bs && be>=as);
	}

	bool isTP(int start, int end,anomalyReportTemp* art){
		for(size_t i=0;i<art->fixdRports.size();i++){
			fixdReport fr=art->fixdRports[i];
			if(crossSection(start,end,fr.start,fr.end)){
				art->fixdRports[i].tp=true;
				return true;
			}
		}
		return false;
	}

	virtual void execute(anomalyReportTemp* art){

		for(size_t i=0;i<art->fixdRports.size();i++){
			art->fixdRports[i].tp=false;
		}

		dio->write("Please upload your local anomalies file.\n");
		string s="";
		float TP=0,sum=0,P=0;
		while((s=dio->read())!="done"){
			size_t t=0;
			for(;s[t]!=',';t++);
			string st=s.substr(0,t);
			string en=s.substr(t+1,s.length());
			int start = stoi(st);
			int end = stoi(en);
			if(isTP(start,end,art))
				TP++;
			sum+=end+1-start;
			P++;
		}
		dio->write("Upload complete.\n");
		float FP=0;
		for(size_t i=0;i<art->fixdRports.size();i++)
			if(!art->fixdRports[i].tp)
				FP++;
		float N=art->testFileSize - sum;
		float tpr=((int)(1000.0*TP/P))/1000.0f;
		float fpr=((int)(1000.0*FP/N))/1000.0f;
		dio->write("True Positive Rate: ");
		dio->write(tpr);
		dio->write("\nFalse Positive Rate: ");
		dio->write(fpr);
		dio->write("\n");

	}
};

class exitCommand:public Command{
	DefaultIO* dio;
	string description ="6.exit\n" ;
public:
	exitCommand(DefaultIO* dio):dio(dio){}
  	virtual ~exitCommand(){};
	virtual void execute();
	void printCommand(){
			cout << this->description << endl;

	};
	string getDescription(){
		return this->description;
	}

};

inline void uploadCommand::execute(anomalyReportTemp* art) {
	this->dio->write("Please upload your local train CSV file.\n");
	string input;
	std::ofstream myFile("trainFile.csv");
	while (!(input == "done")){
    input = this->dio->read();
	if (!(input == "done"))
	myFile << input;
	myFile << endl;
	}
	this->dio->write("Upload complete.\n");
	this->dio->write("Please upload your local test CSV file.\n");
	string input2;
	std::ofstream myFile2("testFile.csv");
	while (!(input2 == "done")){
    input2 = this->dio->read();
	if (!(input2 == "done"))
	myFile2 << input2;
	myFile2 << input2;
	myFile2 << endl;
	}
	this->dio->write("Upload complete.\n");

}

inline void algorithmCommand::execute(anomalyReportTemp* art) {
	float threshold;
	this->dio->write("The current correlation threshold is 0.9\nType a new threshold\n");
    threshold = std::stof(this->dio->read());
	HybridAnomalyDetector had;
	had.setThreshold(threshold);
	while (threshold <= 0 || threshold >= 1){
	this->dio->write("please choose a value between 0 and 1.\nType a new threshold\n");
    threshold = std::stof(this->dio->read());
	}
}

inline void detectCommand::execute(anomalyReportTemp* art) {
		TimeSeries train("trainFile.csv");
		TimeSeries ts2("testFile.csv");
		art->testFileSize = ts2.getSizeRow();
		HybridAnomalyDetector ad;
		ad.setThreshold(art->threshold);
		ad.learnNormal(train);

		fixdReport fr;
		fr.start=0;
		fr.end=0;
		fr.description="";
		fr.tp=false;
		for_each(art->ar.begin(),art->ar.end(),[&fr,art](AnomalyReport& ar){
			if(ar.timeStep==fr.end+1 && ar.description==fr.description)
				fr.end++;
			else{
				art->fixdRports.push_back(fr);
				fr.start=ar.timeStep;
				fr.end=fr.start;
				fr.description=ar.description;
			}
		});
		art->fixdRports.push_back(fr);
		art->fixdRports.erase(art->fixdRports.begin());
	this->dio->write("anomaly detection complete.\n");


}
inline void displayCommand::execute(anomalyReportTemp* art) {
for_each(art->ar.begin(),art->ar.end(),[this](AnomalyReport& ar2){
			dio->write(ar2.timeStep);
			dio->write("\t "+ar2.description+"\n");
		});
		dio->write("Done.\n");
}

inline void exitCommand::execute() {
	exit(0);
}






#endif /* COMMANDS_H_ */

