#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class node
{
    public:
        node();
        node(int ndata, string nname);

        void setLink();
        bool getStatus();
        int getWeight();
        string getName();

    private:
        int data;
        bool status;
        string name;
        vector<string> neighbors;
        vector<int> neighborsWeights;
};

node::node()
{
    data = '\n';
    name = "";
    status = true;
}

node::node(int ndata, string nname)
{
    data = ndata;
    name = nname;
    status = true;
}

bool node::getStatus()
{
    return status;
}

string node::getName()
{
    return name;
}




#endif