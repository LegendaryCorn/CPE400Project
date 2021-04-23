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
        int linkfailureChance();
        bool getStatus();

    private:
        string node1;
        string node2;
        bool isActive;
        int weight;
        bool status;
};

link::link(string n1, string n2, int w)
{
    node1 = n1;
    node2 = n2;
    isActive = true;
    weight = w;
}

string link::getOppositeNode(string node)
{
    if(node == node1){return node2;}
    if(node == node2){return node1;}
    return "";
}

bool link::getIsActive()
{
    return isActive;
}

int link::getWeight()
{
    return weight;
}

bool node::getStatus()
{
    return status;
}

int node::getFailureChance()
{
    return failureChance;
}

void link::linkFailureChance()
{
    int randNum = 0;
    randNum = rand() % 101;

    if(failureChance <= randNum){
        status = true;
        cout << "The link: " + name + "is ACTIVE";
    }else{
        status = false;
        cout << "The link: " + name + "has FAILED";
    }

#endif
