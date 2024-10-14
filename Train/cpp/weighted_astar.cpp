#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include "xxhash.hpp"
#include "environment.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/un.h>


// void error(const char *msg) {
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

// void printArray(const std::vector<uint8_t>& arr) {
//     for (const auto& val : arr) {
//         std::cout << static_cast<int>(val) << " ";
//     }
//     std::cout << std::endl;
// }

std::string stateToString(const std::vector<uint8_t>& state) {
    std::ostringstream vts;
    for (size_t i = 0; i < state.size(); ++i) {
        vts << static_cast<int>(state[i]);
        if (i < state.size() - 1) vts << ", ";
    }
    return vts.str();
}

double getTimeElapsed(const std::chrono::high_resolution_clock::time_point& start, 
                      const std::chrono::high_resolution_clock::time_point& end) {
    return std::chrono::duration<double>(end - start).count();
}

struct Node {
    const Environment* env;
    int depth;
    int parentMove;
    float cost;
    float heuristic;
    Node* parent;

    bool operator==(const Node& other) const {
        return env->getState() == other.env->getState();
    }

    ~Node() {
        delete env;
    }
};

struct NodePointerEq {
    bool operator()(Node const* lhs, Node const* rhs) const {
        return lhs->env->getState() == rhs->env->getState();
    }
};

struct Hash {
    size_t operator()(const Node* node) const {
        std::vector<std::vector<int>> state = node->env->getState();
        return xxh::xxhash<64>(state);
    }
};

class CompareNodeCost {
public:
    bool operator()(const Node* node1, const Node* node2) {
        return node1->cost > node2->cost;
    }
};

void writeFile(int sockfd, const std::vector<Node*>& children) {
    std::vector<std::vector<int>> states;
    for (const auto& child : children) {
        const auto& state = child->env->getState();
        states.insert(states.end(), state.begin(), state.end());
    }

    unsigned long long dataSendSize = sizeof(uint8_t) * states.size();
    // write(sockfd, &dataSendSize, sizeof(dataSendSize));
    // write(sockfd, states.data(), dataSendSize);
}

void parallelWeightedAStar(const Environment *env, float depthPenalty, int numParallel, std::string socketName) {
	/* Initialize Heuristics */
	printf("INITIALIZING QUEUES\n");
	std::priority_queue<Node*,std::vector<Node*>,compareNodeCost> open;
	std::unordered_set<Node*,Hash,NodePointerEq> closed;

	int sockfd, servlen;
	struct sockaddr_un serv_addr;

    // Initalize socket
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, socketName.c_str());
    servlen = (int) strlen(serv_addr.sun_path) + (int) sizeof(serv_addr.sun_family);

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
        error("Creating socket");
    if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
        error("Connecting");

	std::chrono::high_resolution_clock::time_point searchStartTime = std::chrono::high_resolution_clock::now();

	open.push(new Node{env,0,-1,0,0,NULL}); //Push root node to open
	printf("GET START\n");
	closed.insert(new Node{env,0,-1,0,0,NULL}); //Add root node to seen
	

	int searchItr = 1;
	long numNodesGenerated = 1;
	bool isSolved = false;
	Node *solvedNode = NULL;
	while (isSolved == false) {
		std::chrono::high_resolution_clock::time_point startTime, t1;
		double itrTime, remOpenTime, expandingTime, dataWriteTime, checkClosedTime, heuristicTime, costTime, addToQueueTime;
		int maxDepth = 0, minDepth = 0;
		float maxValue = 0, minValue = 0, minCost = 0, maxCost = 0;

		startTime = std::chrono::high_resolution_clock::now();

		// Remove from open
		int openSize = (int) open.size();
		int numPop = std::min(openSize,numParallel);
		std::vector<Node*> popped;

		//printf("REMOVING FROM OPEN\n");
		t1 = std::chrono::high_resolution_clock::now();
		bool goal_node_found_prev = solvedNode != NULL;
		for (int i=0; i<numPop; i++) {
			Node *node = open.top();
			popped.push_back(node);
			open.pop();

			bool isSolved_itr = node->env->isSolved();
			if (isSolved_itr) {
			    if (numParallel == 1) {
				    solvedNode = node;
			        isSolved = true;
			    } else {
                    if (solvedNode == NULL) {
                        solvedNode = node;
                    } else if (solvedNode->cost > node->cost) {
                        solvedNode = node;
                    }
			    }
			    break;
			}
		}
		if (goal_node_found_prev && (popped[0]->cost >= solvedNode->cost)) {
		    // printf("%f, %f, %f\n", popped[0]->cost, solvedNode->cost, popped[popped.size()-1]->cost);
		    isSolved = true;
		}
		remOpenTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());

		// Expand
		//printf("EXPANDING\n");
		t1 = std::chrono::high_resolution_clock::now();
		std::vector<int> depths(popped.size());
		std::vector<Node*> children(popped.size()*env->getNumActions());

		#pragma omp parallel for
		for (unsigned int i=0; i<popped.size(); i++) {
			std::vector<Environment*> children_env = popped[i]->env->getNextStates();
			int depth = popped[i]->depth + 1;
			depths[i] = depth;

			for (unsigned int j=0; j<children_env.size(); j++) {
			    float heuristic_lb = std::max(popped[i]->heuristic - 1, (float) 0.0);  //TODO replace with transition cost
			    float cost = heuristic_lb*(!children_env[j]->isSolved()) + depthPenalty*((float) depth);
				Node *node = new Node{children_env[j],depth,(int) j,cost,heuristic_lb,popped[i]};

				children[i*env->getNumActions() + j] = node;
			}
		}
		minDepth = *std::min_element(depths.begin(),depths.end());
		maxDepth = *std::max_element(depths.begin(),depths.end());
		expandingTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());

		// Write children to file
		t1 = std::chrono::high_resolution_clock::now();
		writeFile(sockfd,children);

		//std::thread writeThread (writeFile,sockfd,children);
		//writeThread.join();
		dataWriteTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());

		//Check if in closed
		t1 = std::chrono::high_resolution_clock::now();
		std::vector<Node*> nodesToAdd;
		std::vector<int> nodesToAdd_idx;
		for (unsigned int i=0; i<children.size(); i++) {
			Node *node = children[i];
			std::unordered_set<Node*,Hash,NodePointerEq>::const_iterator found = closed.find(node);

            if (found == closed.end()) {
                closed.insert(node);
                nodesToAdd.push_back(node);
                nodesToAdd_idx.push_back(i);
            } else if ((*found)->depth > node->depth) {
                (*found)->depth = node->depth;
                (*found)->parentMove = node->parentMove;
                (*found)->parent = node->parent;

                nodesToAdd.push_back(node);
                nodesToAdd_idx.push_back(i);
            } else {
                delete node;
            }
		}
		numNodesGenerated += children.size();
		checkClosedTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());

		//Get value
		//printf("GETTING HEURISTIC\n");
		t1 = std::chrono::high_resolution_clock::now();
		std::vector<float> values(nodesToAdd_idx.size());
		std::vector<float> values_temp;

        float f;
        for (unsigned int i=0; i<children.size(); i++) {
            read(sockfd,reinterpret_cast<char*>(&f),4);
            values_temp.push_back(f);
        }
        for (unsigned int i=0; i<nodesToAdd_idx.size(); i++) {
            values[i] = values_temp[nodesToAdd_idx[i]];
        }

		if (nodesToAdd.size() > 0) {
			minValue = *std::min_element(values.begin(),values.end());
			maxValue = *std::max_element(values.begin(),values.end());
		}

		heuristicTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());

		//Compute cost
		t1 = std::chrono::high_resolution_clock::now();
		std::vector<float> costs(nodesToAdd.size());

		#pragma omp parallel for
		for (unsigned int i=0; i<nodesToAdd.size(); i++) {
			//float heuristic = std::max(nodesToAdd[i]->heuristic, values[i]);
			float cost = values[i]*(!nodesToAdd[i]->env->isSolved()) + depthPenalty*((float) nodesToAdd[i]->depth);
			costs[i] = cost;
		}

		if (nodesToAdd.size() > 0) {
			minCost = *std::min_element(costs.begin(),costs.end());
			maxCost = *std::max_element(costs.begin(),costs.end());
		}
		costTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());


		//Add to open
		t1 = std::chrono::high_resolution_clock::now();
		//printf("ADDING TO OPEN\n");
		for (unsigned int i=0; i<nodesToAdd.size(); i++) {
			Node *nodeToAdd = nodesToAdd[i];

			nodeToAdd->cost = costs[i];
			nodeToAdd->heuristic = values[i];

			open.push(nodeToAdd);
		}

		addToQueueTime = getTimeElapsed(t1,std::chrono::high_resolution_clock::now());

		printf("Times - remOpen: %f, exp: %f, write: %f, check: %f, heur: %f, cost: %f, add: %f, goal_p: %i\n",remOpenTime,expandingTime,dataWriteTime,checkClosedTime,heuristicTime,costTime,addToQueueTime,goal_node_found_prev);

		itrTime = getTimeElapsed(startTime,std::chrono::high_resolution_clock::now());

		printf("Iteration: %i, Min/Max - Depth: %i/%i, Heur: %.2f/%.2f, Cost: %.2f/%.2f, OpenSize: %li, ClosedSize: %li, Time: %f, Num Added: %li\n\n",searchItr,minDepth,maxDepth,minValue,maxValue,minCost,maxCost,open.size(),closed.size(),itrTime,nodesToAdd.size());

		searchItr++;
	}

	printf("SOLVED!\n");

	printf("Move nums:\n");

	Node *currNode = solvedNode;
	while (currNode->depth > 0) {
		printf("%i ",currNode->parentMove);
		currNode = currNode->parent;
	}
	printf("\n");
	printf("Nodes Generated:\n%li\n",numNodesGenerated);

	double totalTime = getTimeElapsed(searchStartTime,std::chrono::high_resolution_clock::now());
	printf("Total time:\n%f\n",totalTime);
}





int main(int argc, const char *argv[]) {

	printf("The argument supplied is %s\n", argv[1]);
	
	/* Get input from file*/
	std::string input = argv[1];
    std::string goal_input = argv[2];
    int n=atoi(argv[3]);
    int m=atoi(argv[4]);
	float depthPenalty = (float) atof(argv[5]);
	int numParallel = atoi(argv[6]);
	std::string socketName = argv[8];
	std::string envName =  argv[9];

	std::string str;

	/* Parse State */
	std::vector<int> one_init;
    std::vector<int> one_goal_init;
	std::stringstream ssin(input);
	while (ssin.good()){
		int val;
		ssin >> val;
		one_init.push_back((uint8_t) val);
	}
    std::stringstream ssi(goal_input);
    int vala;
    while (ssi >> vala) {
        one_goal_init.push_back(static_cast<int>(vala));
    }
    std::vector<std::vector<int>> goal_init(256, std::vector<int>(256,0));
    std::vector<std::vector<int>> init(256, std::vector<int>(256,0));
    int cnt= 0 ; 
    for ( int i = 0 ; i< n ; i++ ){
        for ( int j = 0 ; j< m ; j++ ){
            init[i][j]=one_init[cnt] ;
            cnt++ ;  
        }
    }
    cnt= 0 ;
    for ( int i = 0 ; i< n ; i++ ){
        for ( int j = 0 ; j< m ; j++ ){
            goal_init[i][j]=one_goal_init[cnt]; 
        }
    }


	/* Search */
	// printf("State:\n");
	// printArr(init);
	// printf("\n");

	Environment *env = NULL;
	if (envName == "game") {
		env = new GameState(init , goal_init , n , m);
	} 

	parallelWeightedAStar(env, depthPenalty, numParallel, socketName);

	delete env;

	return 0;
}






// int main() {
//     std::vector<uint8_t> one_init;
//     std::string input;
//     std::cout << "Enter the initial board state values (space-separated integers): ";
//     std::getline(std::cin, input);
//     std::stringstream ssin(input);

//     int val;
//     while (ssin >> val) {
//         one_init.push_back(static_cast<int>(val));
//     }

//     std::vector<uint8_t> one_goal_init;
//     std::string goal_input;
//     std::cout << "Enter the initial board state values (space-separated integers): ";

//     std::getline(std::cin, goal_input);

//     std::stringstream ssi(goal_input);


//     int vala;
//     while (ssi >> vala) {
//         one_goal_init.push_back(static_cast<int>(vala));
//     }
//     std::cout << "size:" ; 
//     int n , m ; 
//     std::cin >> n >> m ; 

//     float depthPenalty;
//     int numParallel;
//     std::cout << "Enter the depth penalty (float): ";
//     std::cin >> depthPenalty;
//     std::cout << "Enter the number of parallel nodes (integer): ";
//     std::cin >> numParallel;
//     std::cout << "Initial State:" << std::endl;
//     Environment* env = nullptr;
//     std::vector<std::vector<int>> goal_init(256, std::vector<int>(256,0));
//     std::vector<std::vector<int>> init(256, std::vector<int>(256,0));
//     int cnt= 0 ; 
//     for ( int i = 0 ; i< n ; i++ ){
//         for ( int j = 0 ; j< m ; j++ ){
//             init[i][j]=one_init[cnt] ;
//             cnt++ ;  
//         }
//     }
//     cnt= 0 ;
//     for ( int i = 0 ; i< n ; i++ ){
//         for ( int j = 0 ; j< m ; j++ ){
//             goal_init[i][j]=one_goal_init[cnt]; 
//         }
//     }


//     env = new GameState(init , goal_init , n , m);
//     parallelWeightedAStar(env, depthPenalty, numParallel);
//     return 0;
// }













// int main() {
   
//     std::vector<std::vector<int>> goal_init(256, std::vector<int>(256,0));
//     std::vector<std::vector<int>> init(256, std::vector<int>(256,0));
//     int n , m ; 
//     std::cin >>  n >> m  ; 
//     for ( int i = 0 ; i< n ; i++ ){
//         for ( int j = 0 ; j< m ; j++ ){
//             std::cin >> init[i][j] ; 
//         }
//     }
//     for ( int i = 0 ; i< n ; i++ ){
//         for ( int j = 0 ; j< m ; j++ ){
//             std::cin >> goal_init[i][j] ; 
//         }
//     }
    
//     float depthPenalty;
//     int numParallel;

//     std::cout << "Enter the depth penalty (float): ";
//     std::cin >> depthPenalty;

//     std::cout << "Enter the number of parallel nodes (integer): ";
//     std::cin >> numParallel;



//     Environment* env = nullptr;
//     env = new GameState(init , goal_init , n , m);

//     parallelWeightedAStar(env, depthPenalty, numParallel);

//     return 0;
// }