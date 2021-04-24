//CPE 400 Project
// Crystal Atoz, Korben DiArcangel, Joshua Insorio

#include <iostream>
#include <fstream>
#include <string>
#include <thread> // Requires C++11!
#include <mutex>
#include <algorithm>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <climits>
#include "node.h"
#include "link.h"

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
bool flipNode(string parameter);
bool flipLink(string parameters);
bool stopLoop();

// Pathing Algorithm
void optimalPath(map<string, node*> nodeList, string nodeA, string nodeB);
void printSolution(map<string, int> dist, map<string, string> path, string nodeA, string nodeB);
void printPath(map<string, string> path, string j, string nodeA);

string minDistance(map<string, int> dist, map<string, bool> set);




// Global Variables
mutex mtx; // Mutex lock, YOU SHOULD ONLY MODIFY THE VARIABLES BELOW WHILE IN THIS LOCK.
bool shouldStop = false; //Determines if the loops should stop
map<string,node*> nodeList; //List of nodes
vector<link*> linkList; //List of links

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

    optimalPath(nodeList, "California", "Idaho");

	//----------------------------
	// Deleting the Nodes
	//----------------------------
	for(map<string,node*>::iterator i = nodeList.begin(); i != nodeList.end(); i++){
        delete i->second;
	}

    //----------------------------
	// Deleting the Links
	//----------------------------
	for(int i = 0; i < linkList.size(); i++){
        delete linkList[i];
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

    double step = 0;
	double stepSize = 1;

    mtx.lock();
	while(!shouldStop){
        mtx.unlock();

		while(lineIteration <= difftime(time(&current),start) && !atEndofFile)
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
		if(difftime(current, start) > step){
            step += stepSize;
            for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++)
            {
                a->second->nodeFailureChance();
            }
            for(int b = 0; b < linkList.size(); b++)
            {
                linkList[b]->linkFailureChance();
            }
		}
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
        //----------------------------
        // UI for User Interaction          //Should be fine in the final when user is prompted first
        //----------------------------
        cout << endl <<  "------------Commands-------------" << endl;
        cout << "| Options:\t\t\t|\n| \t-seenodes\t\t|\n| \t-seelinks\t\t|\n| \t-stop\t\t\t|" << endl;
        cout << "---------------------------------" << endl;
        cout << "> ";

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
    if(command == "flipnode")
    {
        return flipNode(parameters);
    }
    if(command == "fliplink")
    {
        return flipLink(parameters);
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
    linkList.push_back(new link(nodeA, nodeB, weight));

	nodeList[nodeA]->setLink(linkList[linkList.size()-1]);
	nodeList[nodeB]->setLink(linkList[linkList.size()-1]);
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
    cout << endl << "Nodes: " << endl;
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
    cout << endl << "Links: " << endl;
	for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++){
		for(map<string,node*>::iterator b = nodeList.begin(); b != nodeList.end(); b++){
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
// bool flipNode
//
// This function should disable a node if its enabled and vice versa.
//
// inputs:
//    parameter: The node to create.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool flipNode(string parameter)
{
    parameter = parameter.substr(0,parameter.find(" ",0));

    if(nodeList.find(parameter) == nodeList.end())
    {
        cout << "Node does not exist." << endl;
        return false;
    }
    else
    {
        nodeList[parameter]->flipStatus();
        cout << "Node flipped." << endl;
        return true;
    }
}

//-----------------------------------------------------------------------------
//
// bool flipLink
//
// This function should disable a link if its enabled and vice versa.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool flipLink(string parameters)
{
    // Parsing parameters
    size_t firstSpace = parameters.find(" ",0);
    size_t secondSpace = parameters.find(" ",firstSpace+1);

    string nodeA = parameters.substr(0,firstSpace);
    string nodeB = parameters.substr(firstSpace+1,secondSpace-firstSpace-1);

	// Checking if the link is possible
	if((nodeList.find(nodeA) == nodeList.end()) || (nodeList.find(nodeB) == nodeList.end()))
	{
		cout << "One of those nodes doesn't exist." << endl;
		return false;
	}
	if((nodeList[nodeA]->getLinkStatus(nodeB) != -1))
	{
        nodeList[nodeA]->flipLinkStatus(nodeB);
		return true;
	}
    cout << "Link doesn't exist." << endl;
	return false;
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

//-----------------------------------------------------------------------------
//
// bool stopLoop
//
// Iterates through the links of nodeList using Dijkstra's algorithm
//
// inputs:
//    nodeList: A copy of the entire nodeList
//    nodeA: Name of the source node
//    nodeB: Name of the destination node
//
//-----------------------------------------------------------------------------

void optimalPath(map<string, node*> nodeList, string nodeA, string nodeB)
{
    map<string, string> path;
    map<string, int> dist;
    map<string, bool> set;

    //Initialize dist map with active nodes only
    for(map<string,node*>::iterator i = nodeList.begin(); i != nodeList.end(); i++){
        if(i->second->isNodeActive())
        {
            path[i->first] = nodeA;
            dist[i->first] = INT_MAX;
            set[i->first] = false;
        }
    }

    //Initialize nodeA as the start of the path
    //path.push_back(nodeA);

    //Distance from Source node
    dist[nodeA] = 0;

    for(map<string, int>::iterator i = dist.begin(); i != dist.end(); i++)
    {
        // Pick the minimum distance vertex from the set of vertices not yet
        //  processed. u is always equal to src in first iteration.
        string u = minDistance(dist, set);

        // Mark the picked vertex as processed
        set[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (map<string, int>::iterator v = dist.begin(); v != dist.end(); v++)

            // Update dist[v] only if is not in sptSet, there is an edge
            //  from u to v, and  total weight of path from src to v
            //  through u is smaller than current value of dist[v]
            if (!set[v->first] && (nodeList[u]->getLinkStatus(v->first) >= 0) && dist[u] + (nodeList[u]->getLinkStatus(v->first)) < dist[v->first])
            {
                path[v->first] = u;
                dist[v->first] = dist[u] + (nodeList[u]->getLinkStatus(v->first));
            }
    }

    printSolution(dist, path, nodeA, nodeB);
}


//-----------------------------------------------------------------------------
//
// int minDistance
//
// A utility function to find the vertex with minimum distance value,
//  from the set of vertices not yet included in shortest path tree
//
// inputs:
//    dist: A copy of the entire dist map
//    set: A copy of the entire set map
//
//-----------------------------------------------------------------------------

string minDistance(map<string, int> dist, map<string, bool> set)
{
    // Initialize min value
    int min = INT_MAX;
    string min_index = "";

    for (map<string, int>::iterator i = dist.begin(); i != dist.end(); i++){
        if (set[i->first] == false && dist[i->first] <= min)
        {
            min = dist[i->first];
            min_index = i->first;
        }
    }

    return min_index;
}

//-----------------------------------------------------------------------------
//
// void printSolution
//
// Function to print shortest path from source to j using parent array
//
// inputs:
//    dist: A copy of the entire dist map
//    path: A copy of the entire set map
//
//-----------------------------------------------------------------------------

void printSolution(map<string, int> dist, map<string, string> path, string nodeA, string nodeB)
{
    printf("Vertex\t\t\t Distance\tPath\n");
    printf("-------------------------------------");
    for(map<string, int>::iterator i = dist.begin(); i != dist.end(); i++)
    {
        if(i->first == nodeB)
        {
            cout << endl << nodeA << " -> " << i->first << "\t\t" << dist[i->first] << " ";
            printPath(path, i->first, nodeA);
        }
    }
}

void printPath(map<string, string> path, string j, string nodeA)
{
    if (path[j] == j)
    {
        cout << j;
        return;
    }

    printPath(path, path[j], nodeA);
    cout << "->" << j;
}
