#include "stdafx.h"
#include "BTSolver.h"
#include <iostream>

using namespace std;

BTSolver::BTSolver(SudokuMatrix* matrix) {
	this->matrix = matrix;
    this->foundSolution = false;
    this->timedOut = false;
    this->nodes = 0;
    this->backtracks = 0;

	//Fill the variables vector according to the empty slots of the matrix.
	for (int i = 0; i < matrix->getN(); i++)
		for (int j = 0; j < matrix->getN(); j++)
			if (matrix->getMatrixCell(i, j) == 0) {
				Variable var(i, j, matrix->getN());
				var.setValue(0);
				variables.emplace(make_pair(i, j), var);
			}
}

Variable BTSolver::getUnassignedVariable() {
	for (auto element : variables) {
		if (element.second.getValue() == 0)
			return element.second;
	}
	return Variable(-1,-1,-1);
}

int BTSolver::getBacktracks() {
	return this->backtracks;
}

int BTSolver::getNodes() {
	return this->nodes;
}

int BTSolver::getNextValue(std::vector<int>& values) {
	if (values.size() == 0)
		return -1;

	int value = values[0];
	values.erase(values.begin());
	return value;
}

map<pair<int,int>,Variable> BTSolver::getVariables() {
	return this->variables;
}

vector<Variable> BTSolver::getVariableVector() {
	vector<Variable> vars;
	for (auto element : variables)
		vars.push_back(element.second);
}

stack<pair<int, int>> BTSolver::getTrail() {
	return this->trail;
}

int BTSolver::solve(clock_t begin, clock_t limit, bool doFC) {
	nodes++;
    if (foundSolution)
		return 1;

	int value;
	std::vector<int> values;
	for (int i = 1; i <= matrix->getN(); i++)
		values.push_back(i);

	Variable var = getUnassignedVariable();
	if (var.getValue() == -1) {
		foundSolution = true;
		return 1;
	}

	while ((value = getNextValue(values)) != -1) {
		if (isTimedOut(clock()-begin, limit))
			return 0;

		matrix->setMatrixCell(var.getRow(), var.getCol(), value);
		if (SudokuMatrix::checkValidCell(matrix, var.getRow(), var.getCol())) {
			var.setValue(value);
			trail.push(make_pair(var.getRow(),var.getCol()));

			if (doFC)
				applyForwardChecking(var.getRow(), var.getCol(),value);

			//If we were able to find further solutions from this assignment, return success.
			if (solve(begin,limit,doFC) == 1)
				return 1;

			//Backtrack, this value didn't lead to any further possible assignments.
			if (doFC)
				undoForwardChecking(var.getRow(), var.getCol());

			variables[make_pair(var.getRow(),var.getCol())].setValue(0);
			matrix->setMatrixCell(var.getRow(), var.getCol(), 0);
			trail.pop();
			backtracks++;
		}
		matrix->setMatrixCell(var.getRow(), var.getCol(), 0);
	}

	return -1;
}

void BTSolver::applyForwardChecking(int row, int col, int val) {
	//Check down the row.
	for (int i = 0; i < matrix->getN(); i++)
		variables[make_pair(i, col)].removeValue(row,col,val);

	//Check down the column.
	for (int j = 0; j < matrix->getN(); j++)
		variables[make_pair(row, j)].removeValue(row, col, val);

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix,row,col);
	for (int i = 0; i < matrix->getP(); i++)
		for (int j = 0; j < matrix->getQ(); j++)
			variables[make_pair(firstBlockCell.first+i,firstBlockCell.second+j)].removeValue(row, col, val);
}

void BTSolver::undoForwardChecking(int row, int col) {
	//Check down the row.
	for (int i = 0; i < matrix->getN(); i++)
		variables[make_pair(i, col)].undoChange(row, col);

	//Check down the column.
	for (int j = 0; j < matrix->getN(); j++)
		variables[make_pair(row, j)].undoChange(row, col);

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix->getP(); i++)
		for (int j = 0; j < matrix->getQ(); j++)
			variables[make_pair(firstBlockCell.first + i, firstBlockCell.second + j)].undoChange(row, col);
}
