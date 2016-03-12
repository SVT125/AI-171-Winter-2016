#include "stdafx.h"
#include "LocalSolver.h"
#include <random>
#include <algorithm>
#include <iostream>

using namespace std;

LocalSolver::LocalSolver(SudokuMatrix* matrix) {
	this->matrix = matrix;
	this->foundSolution = false;
	this->MAX_STEPS = matrix->getN() * matrix->getN() * 10;
	fillMatrix();
}

int LocalSolver::applyLocalSearch(clock_t begin, clock_t limit) {
	while (!applyMinConflicts()) {
		if (isTimedOut(clock() - begin, limit))
			return 0;
		resetMatrix();
		fillMatrix();
	}
	foundSolution = true;
	return 1;
}

bool LocalSolver::applyMinConflicts() {
	for (int i = 0; i < this->MAX_STEPS; i++) {
		if (isMatrixValid())
			return true;

		//Apply min-conflicts.
		default_random_engine generator(random_device{}());
		shuffle(this->variables.begin(), this->variables.end(), generator);
		int index = 0;
		Variable var = this->variables[index++];
		while (SudokuMatrix::checkValidCell(this->matrix,var.getRow(), var.getCol())) {
			var = this->variables[index++];
		}
		int bestValue = -1, lowestConflicts = INT_MAX;
		for (int i = 0; i < var.getPossibleValues().size(); i++) {
			int conflicts = countConflicts(var.getRow(), var.getCol(), var.getPossibleValues()[i]);
			if (conflicts < lowestConflicts) {
				bestValue = var.getPossibleValues()[i];
				lowestConflicts = conflicts;
			}
		}

		//Assign
		matrix->setMatrixCell(this->variables[index - 1].getRow(),this->variables[index-1].getCol(),bestValue);
		this->variables[index - 1].setValue(bestValue);
	}

	return false;
}

//Note that this includes the variable itself - doesn't matter unless we really wanted the actual number of conflicts.
int LocalSolver::countConflicts(int row, int col, int val) {
	int conflicts = 0;

	//Check down the row.
	for (int i = 0; i < matrix->getN(); i++)
		if (matrix->getMatrixCell(row, i) == val)
			conflicts++;

	//Check down the column.
	for (int j = 0; j < matrix->getN(); j++)
		if (matrix->getMatrixCell(j, col) == val)
			conflicts++;

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
	for (int j = 0; j < matrix->getP(); j++)
		for (int k = 0; k < matrix->getQ(); k++)
			if (matrix->getMatrixCell(firstBlockCell.first + j, firstBlockCell.second + k) == val)
				conflicts++;
	
	return conflicts;
}

bool LocalSolver::isMatrixValid() {
	//Check down the row.
	int row = 0;
	while(row < matrix->getN()) {
		vector<int> seenValues;
		for (int j = 0; j < matrix->getN(); j++) {
			if (find(seenValues.begin(), seenValues.end(), matrix->getMatrixCell(row, j)) != seenValues.end())
				return false;
			seenValues.push_back(matrix->getMatrixCell(row, j));
		}
		row++;
	}


	//Check down the column.
	int col = 0;
	while (col < matrix->getN()) {
		vector<int> seenValues;
		for (int j = 0; j < matrix->getN(); j++) {
			if (find(seenValues.begin(), seenValues.end(), matrix->getMatrixCell(j, col)) != seenValues.end())
				return false;
			seenValues.push_back(matrix->getMatrixCell(j, col));
		}
		col++;
	}


	//Check down the block.
	for (int row = 0; row < matrix->getN(); row += matrix->getP()) {
		for (int col = 0; col < matrix->getN(); col += matrix->getQ()) {
			pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
			vector<int> seenValues;
			for (int j = 0; j < matrix->getP(); j++)
				for (int k = 0; k < matrix->getQ(); k++) {
					if (find(seenValues.begin(), seenValues.end(), matrix->getMatrixCell(firstBlockCell.first + j, firstBlockCell.second + k)) != seenValues.end())
							return false;
					seenValues.push_back(matrix->getMatrixCell(firstBlockCell.first + j, firstBlockCell.second + k));
				}

		}
	}
	
	return true;

}

bool variableSort(Variable v, Variable v2) {
	if (v.getRow() == v2.getRow())
		return v.getCol() < v2.getCol();

	return v.getRow() < v2.getRow();
}

vector<Variable> LocalSolver::getVariableVector() {
	vector<Variable> variables = this->variables;
	sort(variables.begin(), variables.end(), variableSort);
	return variables;
}

void LocalSolver::resetMatrix() {
	for (auto& var : variables) {
		this->matrix->setMatrixCell(var.getRow(),var.getCol(),0);
		var.setValue(0);
	}
}

void LocalSolver::fillMatrix() {
	default_random_engine generator(random_device{}());
	uniform_int_distribution<int> distribution(1, matrix->getN());
	for (int i = 0; i < matrix->getN(); i++)
		for (int j = 0; j < matrix->getN(); j++)
			if (this->matrix->getMatrixCell(i, j) == 0) {
				int randomValue = distribution(generator);
				this->matrix->setMatrixCell(i, j, randomValue);

				Variable var(i, j, matrix->getN());
				var.setValue(randomValue);
				this->variables.push_back(var);
			}
}