#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <array>
#include "xxhash.hpp"


const int MAX_DIMENSION = 256;

// Structure to represent the game board
class Board {
public:
    int width, height;
    std::vector<std::vector<int>> pieces;

    Board(int w, int h) : width(w), height(h), pieces(h, std::vector<int>(w)) {}
};

// Function to generate a random board
Board generateRandomBoard(int w, int h) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 3);

    Board board(w, h);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            board.pieces[i][j] = distrib(gen);
        }
    }
    return board;
}


// Direct comparison function
bool areBoardsEqual_direct(const Board& board1, const Board& board2) {
    if (board1.width != board2.width || board1.height != board2.height) {
        return false;
    }
    for (int i = 0; i < board1.height; ++i) {
        for (int j = 0; j < board1.width; ++j) {
            if (board1.pieces[i][j] != board2.pieces[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Hash comparison function (using C++ xxHash)
bool areBoardsEqual_hash(const Board& board1, const Board& board2) {
    xxh::hash_t<64> hash1 = xxh::xxhash<64>(board1.pieces);
    xxh::hash_t<64> hash2 = xxh::xxhash<64>(board2.pieces);
    return hash1 == hash2;
}


int main() {
    Board board1 = generateRandomBoard(MAX_DIMENSION, MAX_DIMENSION);
    Board board2 = board1; // Initially identical
    Board board3 = generateRandomBoard(MAX_DIMENSION, MAX_DIMENSION); // Different board


    auto start = std::chrono::high_resolution_clock::now();
    bool equal_direct = areBoardsEqual_direct(board1, board2);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_direct = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    bool equal_hash = areBoardsEqual_hash(board1, board2);
    end = std::chrono::high_resolution_clock::now();
    auto duration_hash = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Direct comparison (identical): " << duration_direct.count() << " microseconds\n";
    std::cout << "Hash comparison (identical): " << duration_hash.count() << " microseconds\n";

        // Time direct comparison (different boards)
    start = std::chrono::high_resolution_clock::now();
    equal_direct = areBoardsEqual_direct(board1, board3);
    end = std::chrono::high_resolution_clock::now();
    duration_direct = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Time hash comparison (different boards)
    start = std::chrono::high_resolution_clock::now();
    equal_hash = areBoardsEqual_hash(board1, board3);
    end = std::chrono::high_resolution_clock::now();
    duration_hash = std::chrono::duration_cast<std::chrono::microseconds>(end - start);


    std::cout << "Direct comparison (different): " << duration_direct.count() << " microseconds\n";
    std::cout << "Hash comparison (different): " << duration_hash.count() << " microseconds\n";

    return 0;
}