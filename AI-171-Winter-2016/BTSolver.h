#ifndef BT_SOLVER
#define BT_SOLVER

#include "stdafx.h"
#include "AI-171-Winter-2016.h"
#include "Variable.h"
#include <stack>

class BTSolver {
private:
	SudokuMatrix* matrix;
	int nodes, backtracks;
	bool foundSolution, timedOut;
	std::vector<Variable> variables;
	std::stack<int> trail; //For backtracking and undoing variable assignments

public:
	BTSolver(SudokuMatrix* matrix);

	int getUnassignedVariableIndex();

	int getNodes();

	int getBacktracks();

	int getNextValue(std::vector<int>& values);

	std::vector<Variable> getVariables();

	int solve(clock_t begin, clock_t limit);
};

#endif
