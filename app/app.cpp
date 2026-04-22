#include "SudokuIO.h"

int main() {
    using Board = std::array<std::array<int, 9>, 9>;
    Board board;

    if (!SudokuIO::readBoardFromFile("easy.txt", board)) {
        return 1;
    }
    SudokuIO::printBoard(board);
    std::cout << "----------------------" << std::endl;
    SudokuSolver<9> solver(board);
    auto result = solver.solve();

    if (result) {
        SudokuIO::printBoard(result.value());
    }
    else {
        std::cout << "No solution or invalid puzzle\n";
    }

    return 0;
}