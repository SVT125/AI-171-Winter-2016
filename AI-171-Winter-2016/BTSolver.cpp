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
				variables.push_back(var);
			}
}

int BTSolver::getUnassignedVariableIndex() {
	for (int i = 0; i < variables.size(); i++)
		if (variables[i].getValue() == 0)
			return i;
	return -1;
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

vector<Variable> BTSolver::getVariables() {
	return this->variables;
}

int BTSolver::solve(clock_t begin, clock_t limit, bool doFC) {
	if (foundSolution)
		return 1;

	int nextVariableIndex, value;

	if ((nextVariableIndex = getUnassignedVariableIndex()) == -1) {
		foundSolution = true;
		return 1;
	}

	Variable var = variables[nextVariableIndex];
	vector<int> values = var.getPossibleValues();

	if (values.size() > 0)
		nodes++;

	while ((value = getNextValue(values)) != -1) {
		if (isTimedOut(clock() - begin, limit))
			return 0;

		matrix->setMatrixCell(var.getRow(), var.getCol(), value);
		if (SudokuMatrix::checkValidCell(matrix, var.getRow(), var.getCol())) {
			variables[nextVariableIndex].setValue(value);
			trail.push(nextVariableIndex);

			if (doFC)
				applyForwardChecking(var.getRow(), var.getCol(), value);

			//If we were able to find further solutions from this assignment, return success.
			if (solve(begin, limit, doFC) == 1)
				return 1;

			//Backtrack, this value didn't lead to any further possible assignments.
			if (doFC)
				undoForwardChecking(var.getRow(), var.getCol());

			//Backtrack, this value didn't lead to any further possible assignments.
			variables[trail.top()].setValue(0);
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
		for (int j = 0; j < variables.size(); j++)
			if (variables[j].getRow() == row)
				variables[j].removeValue(row, col, val);
	
	//Check down the column.
	for (int i = 0; i < matrix->getN(); i++)
		for (int j = 0; j < variables.size(); j++)
			if (variables[j].getCol() == col)
				variables[j].removeValue(row, col, val);

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix->getP(); i++)
		for (int j = 0; j < matrix->getQ(); j++)
			for (int k = 0; k < variables.size(); k++)
				if (variables[k].getRow() == firstBlockCell.first + i && variables[k].getCol() == firstBlockCell.second + j)
					variables[k].removeValue(row, col, val);
}

void BTSolver::undoForwardChecking(int row, int col) {
	//Check down the row.
	for (int i = 0; i < matrix->getN(); i++) {
		for (int j = 0; j < variables.size(); j++) {
			if (variables[j].getRow() == row)
				variables[j].undoChange(row, col);
		}
	}

	
	//Check down the column.
	for (int i = 0; i < matrix->getN(); i++) {
		for (int j = 0; j < variables.size(); j++) {
			if (variables[j].getCol() == col)
				variables[j].undoChange(row, col);
		}
	}

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix->getP(); i++) {
		for (int j = 0; j < matrix->getQ(); j++) {
			for (int k = 0; k < variables.size(); k++) {
				if (variables[k].getRow() == firstBlockCell.first + i && variables[k].getCol() == firstBlockCell.second + j)
					variables[k].undoChange(row, col);
			}
		}
	}
}
