//CPE 400 Project
// Crystal Atoz, Korben DiArcangel, Joshua Insorio

#include <iostream>
#include <fstream>
#include <string>
#include <thread> // Requires C++11!
#include <mutex>
#include <map>
#include <time.h>
#include <stdlib.h>
#include "node.h"

using namespace std;

// Thread loops
void iterationLoop(char* fileName);
void inputLoop();

// Command handler
bool handleCommand(string command, string parameters);

// Commands
bool createNode(string parameter);
bool createLink(string parameters);
bool seeNodes();
bool seeLinks();
bool stopLoop();


// Global Variables
mutex mtx; // Mutex lock, YOU SHOULD ONLY MODIFY THE VARIABLES BELOW WHILE IN THIS LOCK.
bool shouldStop = false; //Determines if the loops should stop
map<string,node*> nodeList; //List of nodes

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
	// Time Loop
	//----------------------------

    thread iterationL (iterationLoop, argv[1]);
    thread inputL (inputLoop);

    iterationL.join();
    inputL.join();

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
// void iterationLoop
//
// The main loop that the program runs on.
//
// inputs:
//    fileName: The name of the file in the command line input.
//    shouldStop: The pointer to the shouldStop variable.
//
//-----------------------------------------------------------------------------

void iterationLoop(char* fileName)
{
    //Initialization
    int lineIteration = 0;

    ifstream infile;
	string line = "";
	infile.open(fileName);
	bool atEndofFile = false;

	string command = "";
	string parameters = "";

	time_t start = time(NULL); //set start to current time
	time_t current;

    mtx.lock();
	while(!shouldStop){
        mtx.unlock();

		while(!atEndofFile && lineIteration <= difftime(time(&current),start))
        {
            //cout << line << endl;
            mtx.lock();
            handleCommand(command, parameters);
            mtx.unlock();

            atEndofFile = !getline(infile, line);
            if(line[line.size()-1] == '\r') {line.erase(line.size()-1);}
            if(line[0] == '$'){
				size_t firstSpace = line.find(" ",0);
				size_t secondSpace = line.find(" ",firstSpace+1);
				lineIteration = atoi(line.substr(1,firstSpace).c_str());
				command = line.substr(firstSpace+1,secondSpace-firstSpace-1);
				parameters = line.substr(secondSpace+1,line.size());
            }
            else{
                command = "";
                parameters = "";
            }
		}

        mtx.lock();
	}
	mtx.unlock();
}

//-----------------------------------------------------------------------------
//
// void inputLoop
//
// This function should allow the user to send inputs while the program is running.
//
// inputs:
//    shouldStop: The pointer to the shouldStop variable.
//
//-----------------------------------------------------------------------------

void inputLoop()
{
    mtx.lock();

    while(!shouldStop)
    {
        mtx.unlock();

        // Get input
        string inp;
        getline(cin, inp);
        //cin.getline(i,256); //line
        //cout << i;
        //cout << inp;
        // Parse input string
        string command = inp.substr(0,inp.find(" ",0));
        string parameters = inp.substr(inp.find(" ",0)+1,inp.size());

        // Handle Command
        mtx.lock();
        handleCommand(command, parameters);
    }

    mtx.unlock();
}

//-----------------------------------------------------------------------------
//
// void handleCommand
//
// This function should handle the inputs given to them. This function should be under mutex.
//
// inputs:
//    nodeList: The map containing the pointers to the nodes.
//
// outputs:
//    Returns true if the command was successful.
//
//-----------------------------------------------------------------------------

bool handleCommand(string command, string parameters) //If someone has a better way to do this lmk
{
    if(command == "createnode")
    {
        return createNode(parameters);
    }
    if(command == "createlink")
    {
        return createLink(parameters);
    }
    if(command == "seenodes")
    {
        return seeNodes();
    }
    if(command == "seelinks")
    {
        return seeLinks();
    }
    if(command == "stop")
    {
        return stopLoop();
    }
    return false;
}

//-----------------------------------------------------------------------------
//
// bool createNode
//
// This function should create a node.
//
// inputs:
//    parameter: The node to create.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------
bool createNode(string parameter)
{
    parameter = parameter.substr(0,parameter.find(" ",0));

    if(nodeList.find(parameter) == nodeList.end())
    {
        nodeList[parameter] = new node(parameter);
        cout << "Node created: " << parameter << endl;
        return true;
    }
    else
    {
        cout << "Node already exists." << endl;
        return false;
    }
}

//-----------------------------------------------------------------------------
//
// bool createLink
//
// This function should create a link between A and B.
//
// inputs:
//    nodeA: The name of the first node.
//    nodeB: The name of the second node.
//    weight: the weight of the link.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool createLink(string parameters)
{
    // Parsing parameters
    size_t firstSpace = parameters.find(" ",0);
    size_t secondSpace = parameters.find(" ",firstSpace+1);

    string nodeA = parameters.substr(0,firstSpace);
    string nodeB = parameters.substr(firstSpace+1,secondSpace-firstSpace-1);
    int weight = stoi(parameters.substr(secondSpace+1,parameters.size()));

	// Checking if the link is possible
	if((nodeList.find(nodeA) == nodeList.end()) || (nodeList.find(nodeB) == nodeList.end()))
	{
		cout << "Error setting connection " << nodeA << " - " << nodeB << ": One or more of the nodes does not exist." << endl;
		return false;
	}

	// Forming the link
	nodeList[nodeA]->setLink(nodeB,nodeList[nodeB],weight);
	nodeList[nodeB]->setLink(nodeA,nodeList[nodeA],weight);
	cout << "Link created: " << nodeA << " - " << nodeB << " with weight " << weight << endl;
	return true;
}

//-----------------------------------------------------------------------------
//
// bool seeNodes
//
// This function should list all nodes.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool seeNodes()
{
    for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++){
        cout << a->first << endl;
	}
    return true;
}

//-----------------------------------------------------------------------------
//
// bool seeLinks
//
// This function should list all links.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool seeLinks()
{
	for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++){
		for(map<string,node*>::iterator b = a; b != nodeList.end(); b++){
			if(a->second->getLinkStatus(b->first) >= 0)
			{
				cout << a->first << " - " << b->first << " " << a->second->getLinkStatus(b->first) << endl;
			}
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
// bool stopLoop
//
// This function should stop the loop in iterationLoop.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool stopLoop()
{
    shouldStop = true;
    return shouldStop;
}
