#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

class link
{
    public:
        link(string n1, string n2, int w);

        string getOppositeNode(string node);

        bool getIsActive();
        int getWeight();
        int getFailureChance();
        void linkFailureChance();
        bool getStatus();
        bool flipStatus();
        string getBothNodes();

    private:
        string node1;
        string node2;
        int weight;
        int failureChance;
        bool status;
};

link::link(string n1, string n2, int w)
{
    node1 = n1;
    node2 = n2;
    status = true;
    weight = w;
    failureChance = 1;
}

string link::getOppositeNode(string node)
{
    if(node == node1){return node2;}
    if(node == node2){return node1;}
    return "";
}

bool link::getIsActive()
{
    return status;
}

int link::getWeight()
{
    return weight;
}

bool link::getStatus()
{
    return status;
}

bool link::flipStatus()
{
    status = !status;
    return status;
}

int link::getFailureChance()
{
    return failureChance;
}

void link::linkFailureChance()
{
    int randNum = 0;
    randNum = rand() % 101;

    if(failureChance <= randNum){

    }else{
        status = !status;
        cout << node1 << " " << node2 << " " << status << endl;
    }
}

string link::getBothNodes()
{
    return node1 + " " + node2;
}

#endif
