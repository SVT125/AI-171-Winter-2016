#ifndef VARIABLE
#define VARIABLE

class Variable {
private:
	int row, col, value;
public:
	Variable(int row, int col) {
		this->row = row;
		this->col = col;
		value = 0;
	}

	void setValue(int value) {
		this->value = value;
	}

	int getValue() { return this->value; }
	int getRow() { return this->row; }
	int getCol() { return this->col; }
};

#endif