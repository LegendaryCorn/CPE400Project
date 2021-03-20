#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class node
{
    public:
        node();
        node(string nname);
        node(int ndata, string nname);

        void setLink(string nname, node* npointer, int nweight);
        
        int getLinkStatus(string nname);
        bool getStatus();
        int getWeight();
        string getName();

    private:
        int data;
        bool status;
        string name;
        map<string,node*> neighbors;
        map<string,int> neighborsWeights;
};

//-----------------------------------------------------------------------------
//
// CONSTRUCTORS
//
//-----------------------------------------------------------------------------

node::node()
{
    data = '\n';
    name = "";
    status = true;
}

node::node(string nname)
{
	data = '\n';
	name = nname;
	status = true;
}

node::node(int ndata, string nname)
{
    data = ndata;
    name = nname;
    status = true;
}

//-----------------------------------------------------------------------------
//
// void setLink
//
// This function establishes a link to another node.
//
// inputs:
//    nname: The name of the node to connect to.
//    npointer: The pointer to the node.
//    nweight: The weight of the link.
//
//-----------------------------------------------------------------------------

void node::setLink(string nname, node* npointer, int nweight)
{
	neighbors[nname] = npointer;
	neighborsWeights[nname] = nweight;
}

//-----------------------------------------------------------------------------
//
// int getLinkStatus
//
// Gets the status of the link to a node.
//
// inputs:
//    nname: The name of the node to check the connection with.
//
// outputs:
//    int: A number that determines the status of the link.
//       0 or above: The link exists and has this weight.
//       -1: The link doesn't exist.
//       -2: The link exists, but the node is currently down.
//
//-----------------------------------------------------------------------------
int node::getLinkStatus(string nname)
{
	if(neighbors.find(nname) == neighbors.end())
	{
		return -1;
	}
	if(!neighbors[nname]->getStatus())
	{
		return -2;
	}
	
	return neighborsWeights[nname];
}

//-----------------------------------------------------------------------------
//
// bool getStatus
//
// Checks the node's status.
//
// outputs:
//    status: The current status of the node.
//
//-----------------------------------------------------------------------------

bool node::getStatus()
{
    return status;
}

//-----------------------------------------------------------------------------
//
// string getName
//
// This function get's the node's name.
//
// outputs:
//    name: The name of the node.
//
//-----------------------------------------------------------------------------

string node::getName()
{
    return name;
}




#endif
