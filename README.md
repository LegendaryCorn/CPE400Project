# CPE400Project
Project for UNR's networking class CPE 400.

Team Members: Crystal Atoz , Korben DiArchangel, Joshua Insorio

The team implemented a dynamic routing mechanism design in a faulty network. 

==========================================================================
                        Installation instructions
==========================================================================

You do NOT need to install any extra files or libraries
other than standard g++ dependencies. Please ensure that
g++ is installed and updated before continuing. 

Before running the simulation, you must compile the source
code. After unzipping the folder, you may compile in terminal by typing:

        ‘make’

You may now run the program by following the instructions
in the "Program running instructions" section of this 
README.

==========================================================================
                        Program Background
==========================================================================

This program demonstrates Project Number 6, which is 
Dynamic routing mechanism design in faulty network.

(1)When running this program a file will be read in to create
the world the packets will be sent through.

(2)From there, each node will perform an optimal path using Dijkstra’s 
Algorithm search and will store the optimal path as well as Breadth First 
Search Algorithm(BFS) to provide suboptimal paths for the forwarding 
table. 

(3)Each node created will have a 1/20 chance of the node/link failing for 
DEMONSTRATION purposes on how the system will react to nodes 
going on/off. There is also a 3/4 chance for the repair of the node or 
link that is already shut down. Dijkstra’s calculations have also been 
“delayed” to simulate a slower calculation speed because our sample 
size is small.

(4)Based on if an neighboring node/link is on or off, the current node
with the packet will analyze and send the information accordingly
to the BFS algorithm and will provide a quick path instead of 
recalculating using Dijkstra’s Algorithm.

==========================================================================
                    Program running instructions
==========================================================================
After compiling, you may execute in terminal by typing:

        ‘./main paths.txt’

where:

	paths.txt - The structure of the world that the packets will be sent 
                through. Containing the commands to create nodes/links.

==========================================================================
                        Program commands
==========================================================================
Inside of the program, the user is prompted with eight different options.
Here the user can chose to interact with the code by choosing any of the
options:

    - ‘seenodes’
    - ‘seelinks’
    - ‘flipnode <node>’
    - ‘fliplink <nodeA> <nodeB>’
    - ‘fixnode <node>’
    - ‘fixlink <ndoeA> <nodeB>’
    - ‘findroute <nodeA> <nodeB>’
    - ‘stop’

where:
    
    seenodes    -   Lists all the nodes created.
    seelinks    -   Lists all the links and the weight between them.
    flipnode    -   Manually shutdown a node from the node list.
    fliplink    -   Manually shutdown a link between two nodes from the
                    node list.
    fixnode     -   Manually repair a node that has been shutdown and to 
                    be added back to the node list.
    fixlink     -   Manually repair a link between two nodes and to be 
                    added back to the node list. 
    findroute   -   Find the optimal route between two nodes calculated 
                    by Dijkstra or a temporary route calculated by BFS.
    stop        -   Terminate the program.



===========================
    paths.txt OUTLINE
===========================
Each line in the file will have data that will be used to create the world
that the packets will be sent through. There will be two commands, 
“create node <node>” or “createlink <nodeA> <nodeB> <weight>”. 
An example line can look like this:

	$0 createnode California

	$0 - Command to evoke line is a command instead of a comment
	createnode - Command to specify creation of a node
	California - The string name for the node

Or

	$0 createlink California Nevada 5

    $0 - Command to evoke line is a command instead of a comment
	createlink - Command to specify creation of a link between two nodes
	California - The string name for the nodeA
	Nevada - The string name for the nodeB
	5 - The integer weight for the link


