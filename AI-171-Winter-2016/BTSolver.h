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
	std::map<std::pair<int, int>, Variable> variables;
	std::stack<std::pair<int, int>> trail; //For backtracking and undoing variable assignments

public:
	BTSolver(SudokuMatrix* matrix);

	std::vector<Variable> BTSolver::getEmptyVariableSet(std::map<std::pair<int, int>, Variable> m);

	std::vector<Variable> executeMRV(std::vector<Variable> vars);

	std::vector<Variable> executeDH(std::vector<Variable> vars);

	int executeLCV(int row, int col, int value);

	Variable getUnassignedVariable(bool doMRV, bool doDH);

	int getNodes();

	int getBacktracks();

	int getNextValue(int row, int col, std::vector<int>& values, bool doLCV);

	std::stack<std::pair<int, int>> getTrail();

	std::map<std::pair<int, int>, Variable> getVariables();

	std::vector<Variable> getVariableVector();

	int solve(clock_t begin, clock_t limit, bool doFC, bool doMRV, bool doDH, bool doLCV);

	void applyForwardChecking(int row, int col, int val);

	void undoForwardChecking(int row, int col);
};

#endif