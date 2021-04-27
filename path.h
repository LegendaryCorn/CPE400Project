#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include "node.h"

using namespace std;

class path{
public:
    path();
    path(vector<string> newPath, int newPathLength);
    bool checkValidPath(map<string,node*> nodeList);
    void setNewPath(vector<string> newPath, int newPathLength);
    bool hasPath();

    void printPath();

    string getStartNode();
    string getEndNode();
    int getPathLength();
private:
    vector<string> nodePath;
    string startNode;
    string endNode;
    int pathLength;
};

path::path()
{
    startNode = "";
    endNode = "";
}

path::path(vector<string> newPath, int newPathLength)
{
    nodePath = newPath;
    startNode = nodePath[0];
    endNode = nodePath[nodePath.size()-1];
    pathLength = newPathLength;
}

bool path::checkValidPath(map<string,node*> nodeList)
{
    if(nodePath.empty() || !nodeList[nodePath[0]]->getStatus())
    {
        return false;
    }

    for(int i = 0; i < nodePath.size() - 1; i++)
    {
        if(nodeList[nodePath[i]]->getLinkStatus(nodePath[i+1]) < 0)
        {
            return false;
        }
    }
    return true;
}

void path::setNewPath(vector<string> newPath, int newPathLength)
{
    nodePath = newPath;
    pathLength = newPathLength;
}

bool path::hasPath()
{
    return nodePath.empty();
}

void path::printPath()
{
    cout << startNode << " to " << endNode << endl;;
    if(nodePath.empty())
    {
        cout << "Path is currently impossible." << endl;
    }
    else{
        for(int i = 0; i < nodePath.size(); i++){
            if(i != 0){
                cout << " -> ";
            }
            cout << nodePath[i];
        }
        cout << endl;
        cout << "Path distance: " << pathLength << endl;
    }
    cout << endl;
}

string path::getStartNode(){return startNode;}
string path::getEndNode(){return endNode;}
int path::getPathLength(){return pathLength;}

#endif
