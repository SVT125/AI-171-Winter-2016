#ifndef AI_MAIN
#define AI_MAIN

#include "SudokuMatrix.h"
#include <string>
#include <time.h>

bool isTimedOut(long begin, long end);
SudokuMatrix* parseInput(std::string fileName);
bool fillMatrix(SudokuMatrix* matrix, clock_t begin, int limit);
void outputMatrix(const SudokuMatrix* matrix, std::string fileName);
int main(int argc, char* argv[]);

#endif