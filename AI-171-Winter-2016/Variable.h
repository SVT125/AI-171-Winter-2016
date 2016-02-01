#ifndef VARIABLE
#define VARIABLE

class Variable {
private:
	int row, col, value;
public:
	Variable(int row, int col);

	void setValue(int value);

	int getValue();
	int getRow();
	int getCol();
};

#endif