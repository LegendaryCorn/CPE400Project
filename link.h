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

    private:
        string node1;
        string node2;
        bool isActive;
        int weight;
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

#endif
