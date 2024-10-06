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

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void printArray(const std::vector<uint8_t>& arr) {
    for (const auto& val : arr) {
        std::cout << static_cast<int>(val) << " ";
    }
    std::cout << std::endl;
}

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
        std::vector<int> state = node->env->getState();
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
    std::vector<uint8_t> states;
    for (const auto& child : children) {
        const auto& state = child->env->getState();
        states.insert(states.end(), state.begin(), state.end());
    }

    unsigned long long dataSendSize = sizeof(uint8_t) * states.size();
    // write(sockfd, &dataSendSize, sizeof(dataSendSize));
    // write(sockfd, states.data(), dataSendSize);
}

void parallelWeightedAStar(const Environment* env, float depthPenalty, int numParallel) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodeCost> open;
    std::unordered_set<Node*, Hash, NodePointerEq> closed;

    auto searchStartTime = std::chrono::high_resolution_clock::now();

    open.push(new Node{ env, 0, -1, 0, 0, nullptr });
    closed.insert(new Node{ env, 0, -1, 0, 0, nullptr });

    bool isSolved = false;
    Node* solvedNode = nullptr;

    int searchItr = 1;
    long numNodesGenerated = 1;

    while (!isSolved) {
        auto startTime = std::chrono::high_resolution_clock::now();

        // Remove from open
        int openSize = open.size();
        int numPop = std::min(openSize, numParallel);
        std::vector<Node*> popped;

        for (int i = 0; i < numPop; ++i) {
            Node* node = open.top();
            popped.push_back(node);
            open.pop();

            if (node->env->is_solved()) {
                if (solvedNode == nullptr || solvedNode->cost > node->cost) {
                    solvedNode = node;
                }
                isSolved = true;
                break;
            }
        }

        // If the current best node's cost is greater or equal to the solved node's cost, we can stop searching
        if (solvedNode != nullptr && !popped.empty() && popped[0]->cost >= solvedNode->cost) {
            isSolved = true;
        }

        // Expand nodes
        std::vector<Node*> children(popped.size() * env->getNumActions());
        #pragma omp parallel for
        for (size_t i = 0; i < popped.size(); ++i) {
            auto children_env = popped[i]->env->getNextStates();
            int depth = popped[i]->depth + 1;

            for (size_t j = 0; j < children_env.size(); ++j) {
                float heuristic_lb = std::max(popped[i]->heuristic - 1, 0.0f);
                float cost = heuristic_lb * (!children_env[j]->is_solved()) + depthPenalty * static_cast<float>(depth);
                Node* node = new Node{ children_env[j], depth, static_cast<int>(j), cost, heuristic_lb, popped[i] };
                children[i * env->getNumActions() + j] = node;
            }
        }

        // Check if in closed set
        std::vector<Node*> nodesToAdd;
        for (auto& node : children) {
            if (closed.find(node) == closed.end()) {
                closed.insert(node);
                nodesToAdd.push_back(node);
            } else {
                delete node;  // Node already exists, delete to avoid memory leak
            }
        }

        numNodesGenerated += children.size();

        // Compute cost
        std::vector<float> costs(nodesToAdd.size());
        #pragma omp parallel for
        for (size_t i = 0; i < nodesToAdd.size(); ++i) {
            float cost = nodesToAdd[i]->heuristic + depthPenalty * static_cast<float>(nodesToAdd[i]->depth);
            costs[i] = cost;
        }

        // Add to open set
        for (size_t i = 0; i < nodesToAdd.size(); ++i) {
            nodesToAdd[i]->cost = costs[i];
            open.push(nodesToAdd[i]);
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
    while (currNode && currNode->depth > 0) {
        std::cout << currNode->parentMove << " ";
        currNode = currNode->parent;
    }
    std::cout << std::endl;
    std::cout << "Nodes Generated: " << numNodesGenerated << std::endl;
    double totalTime = getTimeElapsed(searchStartTime, std::chrono::high_resolution_clock::now());
    std::cout << "Total time: " << totalTime << std::endl;
}

int main() {
    std::vector<uint8_t> init;
    std::string input;

    std::cout << "Enter the initial board state values (space-separated integers): ";
    std::getline(std::cin, input);

    std::stringstream ssin(input);
    int val;
    while (ssin >> val) {
        init.push_back(static_cast<int>(val));
    }

    float depthPenalty;
    int numParallel;

    std::cout << "Enter the depth penalty (float): ";
    std::cin >> depthPenalty;

    std::cout << "Enter the number of parallel nodes (integer): ";
    std::cin >> numParallel;

    std::cout << "Initial State:" << std::endl;
    printArray(init);

    Environment* env = nullptr;
    env = new Gamestate(init, 32, 32);

    parallelWeightedAStar(env, depthPenalty, numParallel);

    return 0;
}
