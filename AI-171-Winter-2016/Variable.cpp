#include "stdafx.h"
#include "Variable.h"

Variable::Variable(int row, int col) {
	this->row = row;
	this->col = col;
	value = 0;
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