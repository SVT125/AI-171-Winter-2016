// AI-171-Winter-2016.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <vector>
#include <string>

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

}

bool fillMatrix(SudokuMatrix& matrix, int m) {
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

}

int _tmain(int argc, _TCHAR* argv[])
{
	
	return 0;
}

