/*
Daniel Reuveni
 */

#include "CLI.h"
#include "commands.h"
#include <iostream>
using namespace std;


CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    }

void CLI::start(){
    anomalyReportTemp art;
    string input= " ";
    while(input != "6"){
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    uploadCommand* c1 = new uploadCommand(this->dio);
    this->vectorCommands.push_back(c1);
    algorithmCommand* c2 = new algorithmCommand(this->dio);
    this->vectorCommands.push_back(c2);
    detectCommand* c3 = new detectCommand(this->dio);
    this->vectorCommands.push_back(c3);
    displayCommand* c4 = new displayCommand(this->dio);
    this->vectorCommands.push_back(c4);
    uploadAndAnalizeCommand* c5 = new uploadAndAnalizeCommand(this->dio);
    this->vectorCommands.push_back(c5);
    exitCommand* c6 = new exitCommand(this->dio);
    this->vectorCommands.push_back(c6);

    for (int i = 0; i < 6; i++)
        this->dio->write(this->vectorCommands.at(i)->getDescription());

    int n;
    input = this->dio->read();
    if (input == "1") 
        c1->execute(&art);
    if (input == "2")
        c2->execute(&art);
    if (input == "3")
        c3->execute(&art);
    if (input == "4")
        c4->execute(&art);
    if (input == "5")
        c5->execute(&art);
    if (n == 6)
       c6->execute();
    }
}
    


CLI::~CLI() {
}
