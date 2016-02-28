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

vector<Variable> BTSolver::getEmptyVariableSet(map<pair<int, int>, Variable> m) {
	vector<Variable> values;
	for (map<pair<int, int>, Variable>::iterator iter = m.begin(); iter != m.end(); iter++)
		if (iter->second.getValue() == 0)
			values.push_back(iter->second);
	return values;
}

vector<Variable> BTSolver::executeMRV(vector<Variable> vars) {
	vector<Variable> lowestVariables;
	int lowestHeuristic = INT_MAX;
	for (int i = 0; i < vars.size(); i++) {
		int possibleSize = vars[i].getPossibleValues().size();
		if (possibleSize < lowestHeuristic) {
			lowestVariables.clear();
			lowestVariables.push_back(vars[i]);
			lowestHeuristic = possibleSize;
		} else if (possibleSize == lowestHeuristic)
			lowestVariables.push_back(vars[i]);
	}
	return lowestVariables;
}

vector<Variable> BTSolver::executeDH(vector<Variable> vars) {
	vector<Variable> highestVariables;
	int highestHeuristic = INT_MIN;

	for (int i = 0; i < vars.size(); i++) {
		int degreeHeuristic = 0, row = vars[i].getRow(), col = vars[i].getCol();

		//Check down the row.
		for (int j = 0; j < matrix->getN(); j++)
			if (variables.count(make_pair(row, j)) && variables[make_pair(row, j)].getPossibleValues().size() == 0)
				degreeHeuristic++;
				

		//Check down the column.
		for (int j = 0; j < matrix->getN(); j++)
			if (variables.count(make_pair(j, col)) && variables[make_pair(j, col)].getPossibleValues().size() == 0)
				degreeHeuristic++;
				

		//Check down the block.
		pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
		for (int j = 0; j < matrix->getP(); j++)
			for (int k = 0; k < matrix->getQ(); k++)
				if (variables.count(make_pair(firstBlockCell.first + j, firstBlockCell.second + k)) 
					&& variables[make_pair(firstBlockCell.first + j, firstBlockCell.second + k)].getPossibleValues().size() == 0)
					degreeHeuristic++;

		if (degreeHeuristic > highestHeuristic) {
			highestVariables.clear();
			highestVariables.push_back(vars[i]);
			highestHeuristic= degreeHeuristic;
		}
		else if (degreeHeuristic == highestHeuristic)
			highestVariables.push_back(vars[i]);
					
	}

	return highestVariables;
}

//Apply the heuristics as was given in program execution - final tiebreak by returning first empty variable.
Variable BTSolver::getUnassignedVariable(bool doMRV, bool doDH) {
	//If there are no more empty variables, just return a null sentinel to show we're done.
	vector<Variable> emptyVariables = getEmptyVariableSet(this->variables);
	if (emptyVariables.size() == 0)
		return Variable();

	if (doMRV && !doDH) {
		//only MRV...
		vector<Variable> resultMRV = this->executeMRV(emptyVariables);
		return resultMRV[0];
	} else if (doMRV && doDH) {
		//MRV and tiebreak via DH
		vector<Variable> resultMRV = this->executeMRV(emptyVariables);
		return resultMRV.size() == 1 ? resultMRV[0] : executeDH(resultMRV)[0];
	} else if (!doMRV && doDH) {
		//only DH...
		vector<Variable> resultDH = this->executeDH(emptyVariables);
		return resultDH[0];
	} else {
		//No arguments, just return the first open variable.
		return emptyVariables[0];
	}
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

map<pair<int, int>, Variable> BTSolver::getVariables() {
	return this->variables;
}

vector<Variable> BTSolver::getVariableVector() {
	vector<Variable> vars;
	for (auto element : variables) {
		vars.push_back(element.second);
	}
	return vars;
}

stack<pair<int, int>> BTSolver::getTrail() {
	return this->trail;
}

int BTSolver::solve(clock_t begin, clock_t limit, bool doFC, bool doMRV, bool doDH) {
	if (foundSolution)
		return 1;

	Variable var = getUnassignedVariable(doMRV, doDH);
	int value;

	if (var.getValue() == -1) {
		foundSolution = true;
		return 1;
	}

	vector<int> values = var.getPossibleValues();
	if (values.size() > 0)
		nodes++;

	while ((value = getNextValue(values)) != -1) {
		if (isTimedOut(clock() - begin, limit))
			return 0;

		matrix->setMatrixCell(var.getRow(), var.getCol(), value);
		if (SudokuMatrix::checkValidCell(matrix, var.getRow(), var.getCol())) {
			variables[make_pair(var.getRow(),var.getCol())].setValue(value);
			trail.push(make_pair(var.getRow(), var.getCol()));

			if (doFC)
				applyForwardChecking(var.getRow(), var.getCol(), value);

			//If we were able to find further solutions from this assignment, return success.
			if (solve(begin, limit, doFC, doMRV, doDH) == 1)
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
		if (variables.count(make_pair(row,i)))
			variables[make_pair(row,i)].removeValue(row, col, val);
	
	//Check down the column.
	for (int j = 0; j < matrix->getN(); j++)
		if (variables.count(make_pair(j, col)))
			variables[make_pair(j,col)].removeValue(row, col, val);

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix->getP(); i++)
		for (int j = 0; j < matrix->getQ(); j++)
			if (variables.count(make_pair(firstBlockCell.first + i, firstBlockCell.second + j)))
				variables[make_pair(firstBlockCell.first + i, firstBlockCell.second + j)].removeValue(row, col, val);
}

void BTSolver::undoForwardChecking(int row, int col) {
	//Check down the row.
	for (int i = 0; i < matrix->getN(); i++)
		if (variables.count(make_pair(row, i)))
			variables[make_pair(row, i)].undoChange(row, col);
	
	//Check down the column.
	for (int j = 0; j < matrix->getN(); j++)
		if (variables.count(make_pair(j, col)))
			variables[make_pair(j, col)].undoChange(row, col);

	//Check down the block.
	pair<int, int> firstBlockCell = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix->getP(); i++)
		for (int j = 0; j < matrix->getQ(); j++)
			if (variables.count(make_pair(firstBlockCell.first + i, firstBlockCell.second + j)))
				variables[make_pair(firstBlockCell.first + i, firstBlockCell.second + j)].undoChange(row, col);
}
