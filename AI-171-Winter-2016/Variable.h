#ifndef VARIABLE
#define VARIABLE

#include<vector>
#include<stack>

class Variable {
private:
	int row, col, value;
	std::vector<int> possibleValues;
	std::stack<std::pair<std::pair<int, int>, int>> possibleValueChanges;
public:
	Variable(int row, int col, int n);
	Variable();

	void setValue(int value);

	int getValue();
	int getRow();
	int getCol();
	std::vector<int> getPossibleValues();
	std::stack<std::pair<std::pair<int, int>, int>> getPossibleValueChanges();
	void removeValue(int row, int col, int val);
	void undoChange(int row, int col);
};

#endif