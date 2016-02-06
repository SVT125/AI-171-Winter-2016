#ifndef BT_SOLVER
#define BT_SOLVER

#include "stdafx.h"
#include "AI-171-Winter-2016.h"
#include "Variable.h"
#include <stack>
#include <map>

class BTSolver {
private:
	SudokuMatrix* matrix;
	int nodes, backtracks;
	bool foundSolution, timedOut;
	std::map<std::pair<int,int>,Variable> variables;
	std::stack<std::pair<int,int>> trail; //For backtracking and undoing variable assignments

public:
	BTSolver(SudokuMatrix* matrix);

	Variable getUnassignedVariable();

	int getNodes();

	int getBacktracks();

	int getNextValue(std::vector<int>& values);

	stack<pair<int, int>> getTrail();

	std::map<std::pair<int,int>,Variable> getVariables();

	std::vector<Variable> getVariableVector();

	int solve(clock_t begin, clock_t limit, bool doFC);

	void applyForwardChecking(int row, int col, int val);

	void undoForwardChecking(int row, int col);
};

#endif
