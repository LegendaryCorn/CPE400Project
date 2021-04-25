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
    path(vector<string> newPath);
    bool checkValidPath(map<string,node*> nodeList);
private:
    vector<string> nodePath;
    string startNode;
    string endNode;
};

path::path()
{
    startNode = "";
    endNode = "";
}

path::path(vector<string> newPath)
{
    nodePath = newPath;
    startNode = nodePath[0];
    endNode = nodePath[nodePath.size()-1];
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


#endif
