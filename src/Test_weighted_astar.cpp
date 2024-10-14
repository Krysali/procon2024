
#define NOMINMAX
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

void parallelWeightedAStar(const Environment* env, float depthPenalty, int numParallel) {
    /* Initialize Heuristics */
	printf("INITIALIZING QUEUES\n");
    std::priority_queue<Node*, std::vector<Node*>, CompareNodeCost> open;
    std::unordered_set<Node*, Hash, NodePointerEq> closed;

    open.push(new Node{ env, 0, -1, 0, 0, nullptr });//Push root node to open
	printf("GET START\n");
    closed.insert(new Node{ env, 0, -1, 0, 0, nullptr });//Add root node toseen


    auto searchStartTime = std::chrono::high_resolution_clock::now();

    int searchItr = 1;
    long numNodesGenerated = 1;
    bool isSolved = false;
    Node* solvedNode = nullptr;

    while (!isSolved) {
        
          double itrTime, remOpenTime, expandingTime, dataWriteTime, 
      checkClosedTime, heuristicTime, costTime, addToQueueTime;
		int maxDepth = 0, minDepth = 0;
		float maxValue = 0, minValue = 0, minCost = 0, maxCost = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        // Remove from open
        int openSize = open.size();
        int numPop = std::min(openSize, numParallel);
        std::vector<Node*> popped;
        bool goal_node_found_prev = solvedNode != NULL;
        for (int i = 0; i < numPop; ++i) {
            Node* node = open.top();
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

        // If the current best node's cost is greater or equal to the solved node's cost, we can stop searching
        if (goal_node_found_prev && (popped[0]->cost >= solvedNode->cost)) {
		    // printf("%f, %f, %f\n", popped[0]->cost, solvedNode->cost, popped[popped.size()-1]->cost);
		    isSolved = true;
		}

        // Expand nodes
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
        // Check if in closed set
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
       

        //Get value
		//printf("GETTING HEURISTIC\n");
		//t1 = std::chrono::high_resolution_clock::now();
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





        // Compute cost
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

        // Add to open set
        for (unsigned int i=0; i<nodesToAdd.size(); i++) {
			Node *nodeToAdd = nodesToAdd[i];

			nodeToAdd->cost = costs[i];
			nodeToAdd->heuristic = values[i];

			open.push(nodeToAdd);
		}

        // Print stats
        auto endTime = std::chrono::high_resolution_clock::now();
        double itrTime = getTimeElapsed(startTime, endTime);
        std::cout << "Iteration: " << searchItr << ", Open Size: " << open.size() << ", Closed Size: " << closed.size() << ", Time: " << itrTime << std::endl;

        ++searchItr;
    }

    std::cout << "SOLVED!" << std::endl;

    // Backtrack to get the moves
    Node* currNode = solvedNode;
    std::cout << "Moves: ";
    while ( currNode->depth > 0) {
        std::cout << currNode->parentMove << " ";
        currNode = currNode->parent;
    }
    std::cout << std::endl;
    std::cout << "Nodes Generated: " << numNodesGenerated << std::endl;
    double totalTime = getTimeElapsed(searchStartTime, std::chrono::high_resolution_clock::now());
    std::cout << "Total time: " << totalTime << std::endl;
}

// int main(int argc, const char *argv[]) {

// 	printf("The argument supplied is %s\n", argv[1]);
	
// 	/* Get input from file*/
// 	std::string input = argv[1];
// 	float depthPenalty = (float) atof(argv[2]);
// 	int numParallel = atoi(argv[3]);
// 	std::string socketName = argv[4];
// 	std::string envName =  argv[5];

// 	std::string str;

// 	/* Parse State */
// 	std::vector<uint8_t> init;

// 	std::stringstream ssin(input);
// 	while (ssin.good()){
// 		int val;
// 		ssin >> val;
// 		init.push_back((uint8_t) val);
// 	}

// 	/* Search */
// 	printf("State:\n");
// 	printArr(init);
// 	printf("\n");

// 	Environment *env = NULL;
// 	if (envName == "puzzle15") {
// 		env = new PuzzleN(init,4);
// 	} else if (envName == "puzzle24") {
// 		env = new PuzzleN(init,5);
// 	} else if (envName == "puzzle35") {
// 		env = new PuzzleN(init,6);
// 	} else if (envName == "puzzle48") {
// 		env = new PuzzleN(init,7);
// 	} else if (envName == "cube3") {
// 		env = new Cube3(init);
// 	} else if (envName == "cube4") {
// 		env = new Cube4(init);
// 	} else if (envName == "lightsout7") {
// 		env = new LightsOut(init,7);
// 	}

// 	parallelWeightedAStar(env, depthPenalty, numParallel, socketName);

// 	delete env;

// 	return 0;
// }

int main() {
   
    std::vector<std::vector<int>> goal_init(256, std::vector<int>(256,0));
    std::vector<std::vector<int>> init(256, std::vector<int>(256,0));
    int n , m ; 
    std::cin >>  n >> m  ; 
    for ( int i = 0 ; i< n ; i++ ){
        for ( int j = 0 ; j< m ; j++ ){
            std::cin >> init[i][j] ; 
        }
    }
    for ( int i = 0 ; i< n ; i++ ){
        for ( int j = 0 ; j< m ; j++ ){
            std::cin >> goal_init[i][j] ; 
        }
    }
    
    float depthPenalty;
    int numParallel;

    std::cout << "Enter the depth penalty (float): ";
    std::cin >> depthPenalty;

    std::cout << "Enter the number of parallel nodes (integer): ";
    std::cin >> numParallel;



    Environment* env = nullptr;
    env = new GameState(init , goal_init , n , m);

    parallelWeightedAStar(env, depthPenalty, numParallel);

    return 0;
}