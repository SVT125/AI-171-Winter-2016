// AI-171-Winter-2016.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <iostream>

using namespace std;

class SudokuMatrix {
private:
	vector<vector<int>> matrix = {{ 0 }};
	int m, n, p, q;
public:
	SudokuMatrix(int m, int n, int p, int q);
	vector<vector<int>> getMatrix() const { return this->matrix; }
	int getMatrixCell(int row, int col) const { return this->matrix[row][col]; }
	void setMatrixCell(int row, int col, int value) { this->matrix[row][col] = value; }
	int getM() const { return this->m; }
	int getN() const { return this->n; }
	int getP() const { return this->p; }
	int getQ() const { return this->q; }
	int getRowSize() const { return this->matrix.size(); }
	int getColumnSize() const { return this->matrix[0].size(); }

	//Returns the first [row,col] cell of the corresponding block that holds the given [row,col] cell.
	static pair<int,int> getBlock(const SudokuMatrix& matrix, int row, int col) { 
		int p = matrix.getRowSize(), q = matrix.getColumnSize(),
			blockRow = (row - (row % p)), blockCol = (col - (col % q));
		return make_pair(blockRow, blockCol);
	}
};

SudokuMatrix parseInput(string fileName) {
	ifstream inputFile(fileName);

	int m, n , p,q;
	string line;

	getline(inputFile, line)

	m = stoi(strtok(line, ‘ ‘));
	n = stoi(strtok(null, ‘ ‘)
	p = stoi(strtok(null, ‘ ‘)
	q = stoi(strtok(null, ‘ ‘)

	inputFile.close();

	if ( n != p*q || m > n*n)
		cout << “Matrix Invalid” << endl;
	else
		return SudokuMatrix(m,n,p,q);

}

//Returns true if we were successfully able to fill the matrix, false otherwise.
bool fillMatrix(SudokuMatrix& matrix) {
	default_random_engine generator(random_device{}());
	uniform_int_distribution<int> d(0, matrix.getN());
	for (int i = 0; i < matrix.getM(); i++) {
		int row = d(generator), col = d(generator), index = 0;
		vector<int> values;
		fill(values.begin(), values.end(), index++);
		while (values.size() > 0) {
			int chosenIndex = values[d(generator)];
			matrix.setMatrixCell(row, col, values[chosenIndex]); 
			values[chosenIndex] = 0;

			//If the value is invalid, undo the change by zero-ing the cell. 
			if (!checkValidCell(matrix, row, col))
				matrix.setMatrixCell(row, col, 0);
			else
				break;
		}
		if (values.size() == 0)
			return false;
	}
	return true;
}

bool checkValidCell(const SudokuMatrix& matrix, int row, int col) {
	//check row
	int cell = matrix.getMatrixCell(row, col);
	for (int i = 0; i < matrix.getRowSize(); i++) {
		if (i != row && matrix.getMatrixCell(i, col) != cell)
			return false;
	}

	//check rol
	for (int j = 0; j < matrix.getColumnSize(); j++) {
		if (j != col && matrix.getMatrixCell(row,j) != cell)
			return false;
	}

	//check block
	pair<int,int> block = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix.getP(); i++) {
		for (int j = 0; j < matrix.getQ(); j++) {
			int blockRow = block.first+i, blockCol = block.second+j;
			if (blockRow != row && blockCol != col && matrix[blockRow][blockCol] != cell)
				return false;
		}
	}
	return true;
}

void outputMatrix(const SudokuMatrix& matrix) {
	ofstream outputFile(fileName);
	
	outputFile << matrix.getM() << ‘ ‘ << matrix.getN() << ‘ ‘<< matrix.getP() << ‘ ‘
	 << matrix.getQ() << endl;

	for (int i = 0; i < matrix.get(); i ++){
		for (int j = 0; l < matrix.getQ; i++){
		outputFile << 
		}
	}
	

}

int _tmain(int argc, _TCHAR* argv[])
{
	
	return 0;
}

