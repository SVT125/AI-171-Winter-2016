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
	vector<vector<int>> getMatrix() { return this->matrix; }
	int getMatrixCell(int row, int col) { return this->matrix[row][col]; }
	void setMatrixCell(int row, int col, int value) { this->matrix[row][col] = value; }
	int getM() { return this->m; }
	int getN() { return this->n; }
	int getP() { return this->p; }
	int getQ() { return this->q; }
};

SudokuMatrix& parseInput(string fileName) {

}

SudokuMatrix& fillMatrix(SudokuMatrix& matrix) {

}

bool checkValidCell(const SudokuMatrix& matrix, int row, int col) {

}

void outputMatrix(const SudokuMatrix& matrix) {

}

int _tmain(int argc, _TCHAR* argv[])
{
	
	return 0;
}

