#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <set>

void print_board(const int board[9]) {
    char symbols[] = {' ', 'X', 'O'};  // 0: free, 1: AI, 2: player
    std::cout << "\nBoard:\n";
    for (int i = 0; i < 9; i += 3) {
        std::cout << " " << symbols[board[i]] << " | " << symbols[board[i+1]] << " | " << symbols[board[i+2]] << "\n";
        if (i < 6) {
            std::cout << "---|---|---\n";
        }
    }
    std::cout << std::endl;
}

bool check_win(const int board[9], int player) {
    for (int i = 0; i < 3; ++i) {
        if (board[3 * i] == player && board[3 * i + 1] == player && board[3 * i + 2] == player) return true;
        if (board[i] == player && board[i + 3] == player && board[i + 6] == player) return true;
    }
    if (board[0] == player && board[4] == player && board[8] == player) return true;
    if (board[2] == player && board[4] == player && board[6] == player) return true;

    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Error\n";
        return 1;
    }

    std::string strategy = argv[1];
    if (strategy.length() != 9) {
        std::cout << "Error\n";
        return 1;
    }

    std::set<char> received_numbers;
    for (char c : strategy) {
        if (c < '1' || c > '9' || !received_numbers.insert(c).second) {
            std::cout << "Error: each number from 1 to 9 must appear exactly once\n";
            return 1;
        }
    }

    int board[9] = {0};  // every place is free

    for (int turn = 0; turn < 9; ++turn) {
        bool moveMade = false;
        for (char c : strategy) {
            int move = c - '1';
            if (board[move] == 0) {  // check if free
                board[move] = 1;    // the AI playing
                print_board(board);
                if (check_win(board, 1)) {
                    std::cout << "I win\n";
                    return 0;
                }
                moveMade = true;
                break;
            }
        }

        if (!moveMade) {  // if we don't have free
            std::cout << "DRAW\n";
            return 0;
        }

        // the player need to play
        int player_move;
        while (true) {
            std::cout << "Your move (1-9): ";
            std::cin >> player_move;
            if (std::cin && player_move >= 1 && player_move <= 9 && board[player_move - 1] == 0) {
                board[player_move - 1] = 2;  // player
                print_board(board);
                if (check_win(board, 2)) {
                    std::cout << "I lost\n";
                    return 0;
                }
                break;
            } else {
                std::cout << "Invalid move or position occupied. Try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    std::cout << "DRAW\n";
    return 0;
}