//CPE 400 Project
// Crystal Atoz, Korben DiArcangel, Joshua Insorio

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <thread> // Requires C++11!
#include <queue>
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
void fastLoop();

// Command handler
bool handleCommand(string command, string parameters);

// Commands
bool createNode(string parameter);
bool createLink(string parameters);
bool seeNodes();
bool seeLinks();
bool flipNode(string parameter, bool onOff);
bool flipLink(string parameters, bool onOff);
bool findRoute(string parameters);
bool dtest(string parameters);
bool stopLoop();

// Path Handler
string recursiveRoute(string nodeA, string nodeB);

// Dijkstra Algorithm
void optimalPath(map<string, node*> nodeList, string nodeA, string nodeB);
void printSolution(map<string, int> dist, map<string, string> path, string nodeA, string nodeB);
void printPath(map<string, string> path, string j, string nodeA, string nodeB);
string minDistance(map<string, int> dist, map<string, bool> set);

// Temporary Route
void tempRoute();
bool impossiblePath(string startNode, string endNode);

// BFS Algorithm
void printBFSPath(vector<string> path);
void findPaths(map<string, node*> nodeList, string nodeA, string nodeB);
int isNotVisited(string x, vector<string> path);





// Global Variables
mutex mtx; // Mutex lock, YOU SHOULD ONLY MODIFY THE VARIABLES BELOW WHILE IN THIS LOCK.
bool shouldStop = false; //Determines if the loops should stop
map<string,node*> nodeList; //List of nodes
vector<link*> linkList; //List of links
vector<string> nodesToClose; //List of nodes to shut down
vector<string> linksToClose; //List of links to shut down
bool dRun = false; // Determines if dijkstra's is running
bool fRun = false; // Determines if the fast algorithm needs to run
map<string,map<string,string>> forwardingTables; // The forwarding table, only accessed by IterationLoop
vector<string> fastPath; // Vector that stores the fast path
bool create = true; // Determines whether the algorithm should use the create commands

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
    thread fastL (fastLoop);

    iterationL.join();
    inputL.join();
    fastL.join();

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
	double stepSize = 30;
	double step = stepSize;
    srand(time(0));
    bool firstTime = true;


    mtx.lock();
	while(!shouldStop){
        mtx.unlock();

		while(lineIteration <= difftime(time(&current),start) && !atEndofFile)
        {
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

            else
            {
                command = "";
                parameters = "";
            }
		}

        mtx.lock();
        create = false;

		if(firstTime)
        {
		    dRun = true;
            mtx.unlock();
            forwardingTables.clear();
            dtest("");
            mtx.lock();
            dRun = false;
            fRun = false;
            for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++){
                a->second->clearTable();
                for(map<string,node*>::iterator b = nodeList.begin(); b != nodeList.end(); b++){
                    if(forwardingTables.find(a->first) != forwardingTables.end()  && forwardingTables[a->first].find(b->first) != forwardingTables[a->first].end())
                        a->second->editTable(b->first, forwardingTables[a->first][b->first]);
                }
            }
            firstTime = false;
            step = difftime(time(&current), start) + stepSize;
		}

        // Every x seconds, try to shut down nodes and links
        if(difftime(time(&current), start) > step){
            step += stepSize;



            for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++)
            {
                if(a->second->getStatus() && rand() % 20 == 0){
                    nodesToClose.push_back(a->first);
                    cout << "Node " << a->first << " has been shut down." << endl;
                }
                else if(!a->second->getStatus() && rand() % 4 > 0){
                    a->second->flipStatus();
                    cout << "Node " << a->first << " has been repaired." << endl;
                }
            }

            for(int b = 0; b < linkList.size(); b++)
            {
                if(linkList[b]->getStatus() && rand() % 20 == 0){
                   linksToClose.push_back(linkList[b]->getBothNodes());
                   cout << "Link " << linkList[b]->getBothNodes() << " has been shut down." << endl;
                }
                else if(!linkList[b]->getStatus() && rand() % 4 > 0){
                    linkList[b]->flipStatus();
                    cout << "Link " << linkList[b]->getBothNodes() << " has been repaired." << endl;
                }
            }
        }


        // Main run
        dRun = false;
		for(int i = 0; i < nodesToClose.size(); i++)
        {
            nodeList[nodesToClose[i]]->flipStatus();
            dRun = true;
            fRun = true;
        }
        for(int i = 0; i < linksToClose.size(); i++)
        {
            size_t firstSpace = linksToClose[i].find(" ",0);
            size_t secondSpace = linksToClose[i].find(" ",firstSpace+1);

            string nodeA = linksToClose[i].substr(0,firstSpace);
            string nodeB = linksToClose[i].substr(firstSpace+1,secondSpace-firstSpace-1);

            nodeList[nodeA]->flipLinkStatus(nodeB);
            dRun = true;
            fRun = true;
        }

        if(dRun){
            mtx.unlock();
            forwardingTables.clear();
            dtest("");
            mtx.lock();
            dRun = false;
            fRun = false;

            nodesToClose.clear();
            linksToClose.clear();
            step = difftime(time(&current), start) + stepSize;
            for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++)
            {
                a->second->clearTable();
                for(map<string,node*>::iterator b = nodeList.begin(); b != nodeList.end(); b++)
                {
                    if(forwardingTables.find(a->first) != forwardingTables.end()  && forwardingTables[a->first].find(b->first) != forwardingTables[a->first].end())
                        a->second->editTable(b->first, forwardingTables[a->first][b->first]);
                }
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
    srand(time(0));
    mtx.lock();

    while(!shouldStop)
    {
        //----------------------------
        // UI for User Interaction          //Should be fine in the final when user is prompted first
        //----------------------------
        cout << endl <<  "-----------------------Commands-------------------------" << endl;
        cout << "| Options:\t\t\t\t\t\t|\n|" ;
        cout << "\t-seenodes\t\t\t\t\t|\n|";
        cout << "\t-seelinks\t\t\t\t\t|\n|";
        cout << "\t-flipnode <node>\t\t\t\t|\n|";
        cout << "\t-fliplink <nodeA> <nodeB>\t\t\t|\n|";
        cout << "\t-fixnode <node>\t\t\t\t\t|\n|";
        cout << "\t-fixlink <nodeA> <nodeB>\t\t\t|\n|";
        cout << "\t-findroute <nodeA> <nodeB>\t\t\t|\n|";
        cout << "\t-stop\t\t\t\t\t\t|" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "> ";

        mtx.unlock();

        // Get input
        string inp;
        getline(cin, inp);

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
// void fastLoop
//
// This function should calculate the fast routes when Dijkstra's is running.
//
//-----------------------------------------------------------------------------

void fastLoop()
{
    mtx.lock();

    while(!shouldStop)
    {
        mtx.unlock();
        mtx.lock();
        if(fRun == true)
        {
            tempRoute();
            cout << "----Finished Fast Route Calculations----" << endl;
            fRun = false;
        }

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
    if(command == "createnode" && !dRun && create)
    {
        return createNode(parameters);
    }
    if(command == "createlink" && !dRun && create)
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
    if(command == "flipnode" && !dRun)
    {
        return flipNode(parameters, false);
    }
    if(command == "fliplink" && !dRun)
    {
        return flipLink(parameters, false);
    }
    if(command == "fixnode" && !dRun)
    {
        return flipNode(parameters, true);
    }
    if(command == "fixlink" && !dRun)
    {
        return flipLink(parameters, true);
    }
    if(command == "findroute")
    {
        return findRoute(parameters);
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
    // Initialize parameters
    parameter = parameter.substr(0,parameter.find(" ",0));

    // If node does not exist, create
    if(nodeList.find(parameter) == nodeList.end())
    {
        nodeList[parameter] = new node(parameter);
        cout << "Node created: " << parameter << endl;
        return true;
    }

    // Else node exists
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
    // Initialize by parsing parameters
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
    // Iterate nodeList and print all nodes
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
    // Iterate nodeList and print all links
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

bool flipNode(string parameter, bool onOff)
{

    // Initialize by parsing parameter
    parameter = parameter.substr(0,parameter.find(" ",0));

    if(nodeList.find(parameter) == nodeList.end())
    {
        cout << "Node does not exist." << endl;
        return false;
    }
    else
    {
        if(!onOff && nodeList[parameter]->getStatus()){
            nodesToClose.push_back(parameter);
            cout << "Node shut down." << endl;
            return true;
        }
        if(onOff && !nodeList[parameter]->getStatus()){
            nodeList[parameter]->flipStatus();
            cout << "Node repaired." << endl;
            return true;
        }
    }

    cout << "Invalid node." << endl;
    return false;

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

bool flipLink(string parameters, bool onOff)
{
    // Initialize by parsing parameters
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

    //Flip link if found
	if((nodeList[nodeA]->getLinkStatus(nodeB) != -1))
	{
	    if(!onOff && nodeList[nodeA]->getLinkStatus(nodeB) != -2){
            linksToClose.push_back(parameters);
            cout << "Link shut down." << endl;
            return true;
	    }
        if(onOff && nodeList[nodeA]->getLinkStatus(nodeB) == -2){
            nodeList[nodeA]->flipLinkStatus(nodeB);
            cout << "Link repaired." << endl;
            return true;
	    }
	    else{
            cout << "Invalid link." << endl;
            return false;
	    }
	}
    cout << "Link doesn't exist." << endl;
	return false;
}

//-----------------------------------------------------------------------------
//
// bool findRoute
//
// This function should find a route from node A to node B.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------

bool findRoute(string parameters)
{
    // Initialize by parsing parameters
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

    if(!nodeList[nodeA]->getStatus())
    {
		cout << "The first node is currently inactive." << endl;
		return false;
    }
    // Call recursiveRoute to print out strings
    recursiveRoute(nodeA, nodeB);

    return true;
}

//-----------------------------------------------------------------------------
//
// bool dtest
//
// This function should test the algorithm.
//
// outputs:
//    Returns true if the operation was successful.
//
//-----------------------------------------------------------------------------
bool dtest(string parameters)
{
    // Iterate through nodeList
    for(map<string,node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++)
    {
        // "Delay" function to simulate long calculation times
        std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
        std::uniform_int_distribution<> dist{2, 4};
        std::this_thread::sleep_for(std::chrono::seconds{dist(eng)});

        // Call for and calculate Dijkstra's Alg
        if(a->second->getStatus())
        {
            optimalPath(nodeList, a->first, "");
        }
    }
    cout << "----Finished Dijkstra's Calculations----" << endl;

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

//-----------------------------------------------------------------------------
//
// string recursiveRoute
//
// This function should print the strings in the path.
//
// outputs:
//    Returns the next string.
//
//-----------------------------------------------------------------------------
string recursiveRoute(string nodeA, string nodeB)
{
    // If src is dst, return
    if(nodeA == nodeB)
    {
        cout << nodeB << endl;
        return nodeB;
    }

    // Check for invalid route
    if(nodeList[nodeA]->seeTable(nodeB) == "")
    {
        cout << "Invalid route!" << endl;
        return "";
    }

    cout << nodeA << " -> ";
    recursiveRoute(nodeList[nodeA]->seeTable(nodeB), nodeB);

    return nodeA;
}

//-----------------------------------------------------------------------------
//
// void optimalPath
//
// Iterates through the nodes and links of nodeList using Dijkstra's algorithm
//
// inputs:
//    nodeList: A copy of the entire nodeList
//    nodeA: Name of the source node
//    nodeB: Name of the destination node
//
//-----------------------------------------------------------------------------

void optimalPath(map<string, node*> nodeList, string nodeA, string nodeB)
{
    //Initializations
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
//    nodeA: A copy of nodeA
//    nodeB: A copy of nodeB
//
//-----------------------------------------------------------------------------

void printSolution(map<string, int> dist, map<string, string> path, string nodeA, string nodeB)
{
    // Iterate through dist map
    for(map<string, int>::iterator i = dist.begin(); i != dist.end(); i++)
    {
        // Start printPath recursive call to print Dijkstra's calculations
        if(dist[i->first] != INT_MAX && dist[i->first] >= 0)
        {
            printPath(path, i->first, nodeA, i->first);
        }
    }
}


//-----------------------------------------------------------------------------
//
// void printPath
//
// Function to recursively print forwarding table path
//
// inputs:
//    dist: A copy of the entire dist map
//    path: A copy of the entire set map
//    nodeA: A copy of the source node
//    nodeB: A copy of the destination
//
//-----------------------------------------------------------------------------

void printPath(map<string, string> path, string j, string nodeA, string nodeB)
{
    // If path[j] is src, return
    if (path[j] == nodeA)
    {
        forwardingTables[nodeA][nodeB] = j;
        return;
    }

    // Recursively call printPath
    printPath(path, path[j], nodeA, nodeB);
}

//-----------------------------------------------------------------------------
//
// void tempRoute
//
// Function to calculate a temporary route after shutdown
//
//-----------------------------------------------------------------------------

void tempRoute()
{
    vector<string> pathToFix1; // Start node
    vector<string> pathToFix2; // End node
    vector<string> pathToFix3; // Forwarding Table Node

    // Get rid of shut down node paths; we know these don't work

    for(int i = 0; i < nodesToClose.size(); i++)
    {
        for(map<string, node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++)
        {
            a->second->editTable(nodesToClose[i], "");
        }
    }

    // Now to check each path if its possible
    for(map<string, node*>::iterator a = nodeList.begin(); a != nodeList.end(); a++){
        for(map<string, node*>::iterator b = nodeList.begin(); b != nodeList.end(); b++){

            // Skip inactive nodes because we don't care about them
            if(a->second->getStatus() && b->second->getStatus()){
                // If it isn't possible, check for alt route. If it is, just ignore it.
                if(impossiblePath(a->first, b->first)){
                    fastPath.clear();
                    findPaths(nodeList, a->first, b->first);
                    if(!fastPath.empty()){
                        pathToFix1.push_back(a->first);
                        pathToFix2.push_back(b->first);
                        pathToFix3.push_back(fastPath[1]);
                    }
                    else{
                        pathToFix1.push_back(a->first);
                        pathToFix2.push_back(b->first);
                        pathToFix3.push_back("");
                    }
                }
            }
        }
    }

    // Now that this is over, fix the paths

    for(int x = 0; x < pathToFix1.size(); x++)
    {
        nodeList[pathToFix1[x]]->editTable(pathToFix2[x], pathToFix3[x]);
    }

}

//-----------------------------------------------------------------------------
//
// bool impossiblePathA
//
// Function to recursively determine if the path is impossible
//
// inputs:
//    startNode: A copy of the start node
//    endNode: A copy of the end node
//    brokenNode: A copy of the broke node
//
//-----------------------------------------------------------------------------

bool impossiblePath(string startNode, string endNode)
{
    if(!nodeList[startNode]->getStatus()){return true;}
    if(startNode == endNode){return false;}
    if(nodeList[startNode]->getLinkStatus(nodeList[startNode]->seeTable(endNode)) < 0){return true;}
    return impossiblePath(nodeList[startNode]->seeTable(endNode), endNode);
}

//-----------------------------------------------------------------------------
//
// void printBFSPath
//
// Function to print the BFS path
//
// inputs:
//    path: A copy BFS path vector
//
//-----------------------------------------------------------------------------

void printBFSPath(vector<string> path)
{
    // Initialize fastPath vector with BFS path
    int size = path.size();
    for (int i = 0; i < size; i++)
    {
        fastPath.push_back(path[i]);
    }
}

//-----------------------------------------------------------------------------
//
// void findPaths
//
// Iterates through the nodes and links of nodeList using BFS algorithm
//
// inputs:
//    nodeList: A copy of the entire nodeList
//    nodeA: Name of the source node
//    nodeB: Name of the destination node
//
//-----------------------------------------------------------------------------

void findPaths(map<string, node*> nodeList, string nodeA, string nodeB)
{
    // create a queue which stores
    // the paths
    queue<vector<string> > q;
    vector<string> path;

    // path vector to store the current path
    if(nodeList[nodeA]->getStatus())
    {
        path.push_back(nodeA);
        q.push(path);
    }

    while (!q.empty())
    {
        path = q.front();
        q.pop();
        string last = path[path.size() - 1];

        // if last vertex is the desired destination
        // then print the path
        if (last == nodeB)
        {
            printBFSPath(path);
            while(!q.empty())
            {
                q.pop();
            }
        }

        else
        {
            // traverse to all the nodes connected to
            // current vertex and push new path to queue
            for (map<string, node*>::iterator i = nodeList.begin(); i != nodeList.end(); i++)
            {
                //Check if link exists
                if(i->second->getStatus() && nodeList[last]->getLinkStatus(i->first) >= 0)
                {
                    if(isNotVisited(i->first, path))
                    {
                        vector<string> newpath(path);
                        newpath.push_back(i->first);
                        q.push(newpath);
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
//
// void isNotVisited
//
// Function to keep track of nodes that have been visited
//
// inputs:
//    x: A copy of the string to be searched for in the path vector
//    path: A copy of the path vector
//
//-----------------------------------------------------------------------------

int isNotVisited(string x, vector<string> path)
{
    // Iterate through path to find and determine if x(node) exists in the path vector
    for(int i = 0; i < path.size(); i++)
    {
        if(std::find(path.begin(), path.end(), x) != path.end())
        {
            return 0;
        }
    }

    return 1;
}
