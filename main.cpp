//CPE 400 Project
// Crystal Atoz, Korben DiArcangel, Joshua Insorio

#include <iostream>
#include <string>
#include <map>
#include "node.h"

using namespace std;

void createLink(map<string,node*> nodeList, string nodeA, string nodeB, int weight);

int main()
{
	map<string,node*> nodeList;
	
	
	//----------------------------
	// Initialize Nodes
	//----------------------------
    nodeList["California"] = new node("California");
    nodeList["Nevada"] = new node("Nevada");
    nodeList["Colorado"] = new node("Colorado");
    nodeList["Arizona"] = new node("Arizona");
    nodeList["New Mexico"] = new node("New Mexico");
    nodeList["Utah"] = new node("Utah");
    nodeList["Idaho"] = new node("Idaho");
    nodeList["Oregon"] = new node("Oregon");
    
    //----------------------------
	// Initialize Links
	//----------------------------
    createLink(nodeList, "California","Nevada",5);
    createLink(nodeList, "Utah","Nevada",10);
    createLink(nodeList, "New Mexico","Idaho",4);
    createLink(nodeList, "Arizona","Colorado",7);
    createLink(nodeList, "Oregon","California",6);
    createLink(nodeList, "Nevada","Idaho",2);
    createLink(nodeList, "Arizona","New Mexico",8);
    createLink(nodeList, "Oregon","Utah",12);
    createLink(nodeList, "Colorado","California",2);
    createLink(nodeList, "Nevada","New Mexico",6);
	
	//----------------------------
	// Testing the Nodes (Should be removed later)
	//----------------------------
	
	for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++){
		for(map<string,node*>::iterator b = nodeList.begin(); b != nodeList.end(); b++){
			if(a->second->getLinkStatus(b->first) >= 0)
			{
				cout << a->first << " - " << b->first << " " << a->second->getLinkStatus(b->first) << endl;
			}
		}
	}
	
	//----------------------------
	// Deleting the Nodes
	//----------------------------
	for(map<string,node*>::iterator i = nodeList.begin(); i != nodeList.end(); i++){
		delete i->second;
	}
	
    return 0;
}

//-----------------------------------------------------------------------------
//
// void createLink
//
// This function should create a link between A and B.
//
// inputs:
//    nodeList: The map containing the pointers to the nodes.
//    nodeA: The name of the first node.
//    nodeB: The name of the second node.
//    weight: the weight of the link.
//
//-----------------------------------------------------------------------------

void createLink(map<string,node*> nodeList, string nodeA, string nodeB, int weight)
{
	//----------------------------
	// Checking if the link is possible
	//----------------------------
	if((nodeList.find(nodeA) == nodeList.end()) || (nodeList.find(nodeB) == nodeList.end()))
	{
		cout << "Error setting connection " << nodeA << " - " << nodeB << ": One or more of the nodes does not exist." << endl;
		return;
	}
	
	//----------------------------
	// Forming the link
	//----------------------------
	nodeList[nodeA]->setLink(nodeB,nodeList[nodeB],weight);
	nodeList[nodeB]->setLink(nodeA,nodeList[nodeA],weight);
}
