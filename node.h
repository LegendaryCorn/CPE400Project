#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include "link.h"

using namespace std;

class node
{
    public:
        node();
        node(string nname);
        node(int ndata, string nname);

        void setLink(link* l);

        int getLinkStatus(string nname);
        bool getStatus();
        int getWeight();
        string getName();
        int getFailureChance();

        void nodeFailureChance();
        bool isNodeActive();
        bool flipStatus();
        void flipLinkStatus(string nname);

        void editTable(string endNode, string neighborNode);
        string seeTable(string endNode);
        void clearTable();

    private:
        int data;
        bool status;
        string name;
        int failureChance;
        bool nodeIsActive;
        map<string,link*> neighborLinks;
        map<string,string> forwardingTable;
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
    failureChance = 1;
}

node::node(string nname)
{
	data = '\n';
	name = nname;
	status = true;
	failureChance = 1;
}

node::node(int ndata, string nname)
{
    data = ndata;
    name = nname;
    status = true;
    failureChance = 1;
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

void node::setLink(link* l)
{
	neighborLinks[l->getOppositeNode(name)] = l;
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
	if(neighborLinks.find(nname) == neighborLinks.end())
	{
		return -1;
	}
	if(!neighborLinks[nname]->getIsActive())
	{
		return -2;
	}

	if(!neighborLinks[neighborLinks[nname]->getOppositeNode(name)]->getStatus())
    {
        return -3;
    }

	return neighborLinks[nname]->getWeight();
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

//-----------------------------------------------------------------------------
//
// Gets chance of failure
//
// Returns chance of failure
//
//-----------------------------------------------------------------------------

int node::getFailureChance()
{
    return failureChance;
}

//-----------------------------------------------------------------------------
//
// Returns if node is active or not
//
//-----------------------------------------------------------------------------

bool node::isNodeActive()
{
    return status;
}

//-----------------------------------------------------------------------------
//
// Flips a node's status
//
//-----------------------------------------------------------------------------

bool node::flipStatus()
{
    status = !status;
    return status;
}

//-----------------------------------------------------------------------------
//
// Flips a link's status
//
//-----------------------------------------------------------------------------

void node::flipLinkStatus(string nname)
{
    neighborLinks[nname]->flipStatus();
}

//-----------------------------------------------------------------------------
//
// Checks if node/link failed based on chance of failure
//
//-----------------------------------------------------------------------------

void node::nodeFailureChance()
{
    int randNum = 0;
    randNum = rand() % 101;

    if(failureChance <= randNum){

    }else{
        status = !status;
        cout << name << " " << status << endl;
    }
}

//-----------------------------------------------------------------------------
//
// Edits the forwarding table
//
//-----------------------------------------------------------------------------

void node::editTable(string endNode, string neighborNode){
    forwardingTable[endNode] = neighborNode;
}

//-----------------------------------------------------------------------------
//
// Returns the neighbor to forward to
//
//-----------------------------------------------------------------------------

string node::seeTable(string endNode)
{
    if(forwardingTable.find(endNode) == forwardingTable.end() || forwardingTable[endNode] == "")
    {
        return "";
    }
    return forwardingTable[endNode];
}

//-----------------------------------------------------------------------------
//
// Clears the table
//
//-----------------------------------------------------------------------------

void node::clearTable()
{
    forwardingTable.clear();
}


#endif
