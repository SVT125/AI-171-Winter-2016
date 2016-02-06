#ifndef AI_MAIN
#define AI_MAIN

#include "SudokuMatrix.h"
#include "Variable.h"
#include <string>
#include <time.h>

bool isTimedOut(long begin, long end);
SudokuMatrix* parseInput(std::string fileName);
bool fillMatrix(SudokuMatrix* matrix, clock_t begin, int limit);
void outputMatrix(const SudokuMatrix* matrix, std::string fileName);
SudokuMatrix* readInput(std::string fileName);
void outputLog(SudokuMatrix* matrix, std::string fileName, int flag, clock_t start, clock_t s_start, clock_t s_end, std::vector<Variable> vars, int nodes, int bts);
bool findFlag(int argc, char* argv[], std::string flag);
int main(int argc, char* argv[]);

#endif