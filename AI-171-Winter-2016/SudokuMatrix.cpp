#include "stdafx.h"
#include "SudokuMatrix.h"

using namespace std;

SudokuMatrix::SudokuMatrix(int m, int n, int p, int q) {
	this->m = m;
	this->n = n;
	this->p = p;
	this->q = q;
	matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
}

SudokuMatrix::SudokuMatrix(const SudokuMatrix& sm) {
	this->m = sm.getM();
	this->n = sm.getN();
	this->p = sm.getP();
	this->q = sm.getQ();
	matrix = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
}

vector<vector<int>> SudokuMatrix::getMatrix() const { return this->matrix; }

int SudokuMatrix::getMatrixCell(int row, int col) const { return this->matrix[row][col]; }

void SudokuMatrix::setMatrixCell(int row, int col, int value) { this->matrix[row][col] = value; }

int SudokuMatrix::getM() const { return this->m; }

int SudokuMatrix::getN() const { return this->n; }

int SudokuMatrix::getP() const { return this->p; }

int SudokuMatrix::getQ() const { return this->q; }

//Returns the first [row,col] cell of the corresponding block that holds the given [row,col] cell.
std::pair<int, int> SudokuMatrix::getBlock(const SudokuMatrix* matrix, int row, int col) {
	int p = matrix->getP(), q = matrix->getQ(),
		blockRow = (row - (row % p)), blockCol = (col - (col % q));
	return std::make_pair(blockRow, blockCol);
}

bool SudokuMatrix::checkValidCell(const SudokuMatrix* matrix, int row, int col) {
	//check row
	int cell = matrix->getMatrixCell(row, col);
	for (int i = 0; i < matrix->getN(); i++) {
		if (i != row && matrix->getMatrixCell(i, col) == cell)
			return false;
	}

	//check col
	for (int j = 0; j < matrix->getN(); j++) {
		if (j != col && matrix->getMatrixCell(row, j) == cell)
			return false;
	}

	//check block
	std::pair<int, int> block = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix->getP(); i++) {
		for (int j = 0; j < matrix->getQ(); j++) {
			int blockRow = block.first + i, blockCol = block.second + j;
			if (blockRow != row && blockCol != col && matrix->getMatrixCell(blockRow, blockCol) == cell)
				return false;
		}
	}
	return true;
}