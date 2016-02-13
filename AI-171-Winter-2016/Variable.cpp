#include "stdafx.h"
#include "Variable.h"

#include <vector>

using namespace std;

Variable::Variable(int row, int col, int n) {
	this->row = row;
	this->col = col;
	value = 0;
	for (int i = 1; i <= n; i++)
		possibleValues.push_back(i);
}

Variable::Variable() {
	this->row = -1;
	this->col = -1;
	this->value = -1;
}

void Variable::setValue(int value) {
	this->value = value;
}

int Variable::getValue() {
	return this->value;
}

int Variable::getRow() {
	return this->row;
}

int Variable::getCol() {
	return this->col;
}

vector<int> Variable::getPossibleValues() {
	return this->possibleValues;
}

stack<pair<pair<int, int>, int>> Variable::getPossibleValueChanges() {
	return this->possibleValueChanges;
}

void Variable::removeValue(int row, int col, int val) {
	//Remove the value from this variable's possible values remaining.
	for (int i = 0; i < possibleValues.size(); i++)
		if (possibleValues[i] == val) {
		possibleValues.erase(possibleValues.begin() + i);
		break;
		}

	//Add this change to the stack in case of later backtracking.
	possibleValueChanges.push(make_pair(make_pair(row, col), val));
}

void Variable::undoChange(int row, int col) {
	if (possibleValueChanges.size() > 0 && possibleValueChanges.top().first == make_pair(row, col)) {
		this->value = possibleValueChanges.top().second;
		possibleValueChanges.pop();
	}
}