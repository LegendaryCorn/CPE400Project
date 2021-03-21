//CPE 400 Project
// Crystal Atoz, Korben DiArcangel, Joshua Insorio

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "node.h"

using namespace std;

void createLink(map<string,node*> nodeList, string nodeA, string nodeB, int weight);

int main(int argc, char *argv[])
{
	//----------------------------
	// Test for Appropriate Input
	//----------------------------
	
	if(argc != 2){
		cout << "Please name the input file in the command line." << endl;
		return 0;
	}	
	
	
	
	//----------------------------
	// Initialization
	//----------------------------
	map<string,node*> nodeList;
	ifstream infile;
	string line;
	string mode = "";
	
	infile.open(argv[1]);
	
	
	//----------------------------
	// MAIN LOOP
	//----------------------------
	
	while(getline(infile, line))
	{
		
		// Ignore the line
		if(line.empty() || line[0] == '%')
		{
			// Do nothing
		}
		
		// Change modes
		else if(line[0] == '.')
		{
			mode = line;
		}
		
		// Standard Case
		else
		{
			
			//----------------------------
			// Initialize Nodes
			//----------------------------
			
			if(mode == ".begin.inodes")
			{
				nodeList[line] = new node(line);
			}
			
			
			//----------------------------
			// Initialize Links
			//----------------------------
			
			else if(mode == ".begin.ilinks")
			{
				size_t firstComma = line.find(",",0);
				size_t secondComma = line.find(",",firstComma+2);

				createLink(nodeList, line.substr(0,firstComma), line.substr(firstComma+1,secondComma-firstComma-1),stoi(line.substr(secondComma+1,line.size())));
			}
			
			
			else
			{
				// Do nothing
			}
		}
	}
	
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
