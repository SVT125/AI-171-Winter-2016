#ifndef LOCAL_SOLVER
#define LOCAL_SOLVER

#include "stdafx.h"
#include "AI-171-Winter-2016.h"
#include "Variable.h"
#include <map>

class LocalSolver {
private:
	SudokuMatrix* matrix;
	bool foundSolution, timedOut;
	std::vector<Variable> variables;
	int MAX_STEPS;
public:
	LocalSolver(SudokuMatrix* matrix);
	int applyLocalSearch(clock_t begin, clock_t limit);
	bool applyMinConflicts();
	int countConflicts(int row, int col, int val);
	bool isMatrixValid();
	std::vector<Variable> getVariableVector();
	void resetMatrix();
	void fillMatrix();
};


#endif