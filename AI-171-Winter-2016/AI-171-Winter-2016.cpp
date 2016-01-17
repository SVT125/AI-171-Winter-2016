// AI-171-Winter-2016.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <iostream>

using namespace std;

class SudokuMatrix {
private:
	vector<vector<int>> matrix;
	int m, n, p, q;
public:
	SudokuMatrix(int m, int n, int p, int q) {
		this->m = m;
		this->n = n;
		this->p = p;
		this->q = q;
		matrix = vector<vector<int>>(n,vector<int>(n,0));
	}
	vector<vector<int>> getMatrix() const { return this->matrix; }
	int getMatrixCell(int row, int col) const { return this->matrix[row][col]; }
	void setMatrixCell(int row, int col, int value) { this->matrix[row][col] = value; }
	int getM() const { return this->m; }
	int getN() const { return this->n; }
	int getP() const { return this->p; }
	int getQ() const { return this->q; }

	//Returns the first [row,col] cell of the corresponding block that holds the given [row,col] cell.
	static pair<int,int> getBlock(const SudokuMatrix& matrix, int row, int col) { 
		int p = matrix.getP(), q = matrix.getQ(),
			blockRow = (row - (row % p)), blockCol = (col - (col % q));
		return make_pair(blockRow, blockCol);
	}
};

SudokuMatrix parseInput(string fileName) {
	ifstream inputFile(fileName);

	int m, n , p,q;
	string line;

	getline(inputFile, line, ' ');
	m = stoi(line);
	getline(inputFile, line, ' ');
	n = stoi(line);
	getline(inputFile, line, ' ');
	p = stoi(line);
	getline(inputFile, line, ' ');
	q = stoi(line);

	inputFile.close();

	if (n != p*q || m > n*n) {
		throw exception("Invalid file!");
	} else
		return SudokuMatrix(m,n,p,q);
}

bool checkValidCell(const SudokuMatrix& matrix, int row, int col) {
	//check row
	int cell = matrix.getMatrixCell(row, col);
	for (int i = 0; i < matrix.getN(); i++) {
		if (i != row && matrix.getMatrixCell(i, col) == cell)
			return false;
	}

	//check col
	for (int j = 0; j < matrix.getN(); j++) {
		if (j != col && matrix.getMatrixCell(row, j) == cell)
			return false;
	}

	//check block
	pair<int, int> block = SudokuMatrix::getBlock(matrix, row, col);
	for (int i = 0; i < matrix.getP(); i++) {
		for (int j = 0; j < matrix.getQ(); j++) {
			int blockRow = block.first + i, blockCol = block.second + j;
			if (blockRow != row && blockCol != col && matrix.getMatrixCell(blockRow,blockCol) == cell)
				return false;
		}
	}
	return true;
}

//Returns true if we were successfully able to fill the matrix, false otherwise.
bool fillMatrix(SudokuMatrix& matrix) {
	default_random_engine generator(random_device{}());
	uniform_int_distribution<int> d(0, matrix.getN() - 1);

	//Initialize a vector of shuffled cell row/index points, to linearly take from when populating the matrix.
	int cellsIndex = 0;
	vector<pair<int, int>> cells;
	for (int i = 0; i < matrix.getN(); i++)
		for (int j = 0; j < matrix.getN(); j++)
			cells.push_back(make_pair(i, j));
	shuffle(cells.begin(), cells.end(), generator);

	for (int i = 0; i < matrix.getM(); i++) {
		pair<int, int> cell = cells[cellsIndex++];
		int row = cell.first, col = cell.second, index = 1;
		bool isCellFilled = false;

		//Store the values, row, and col indices possible to be shuffled later.
		vector<int> values(matrix.getN(), 0);

		//Initialize the vectors to their possible values; convert to letters if N > 9.
		for (int j = 0; j < values.size(); j++)
			values[j] = index++;

		//Shuffle the vectors for random picking.
		shuffle(values.begin(), values.end(),generator);

		for(int l = 0; l < values.size(); l++) {
			matrix.setMatrixCell(row, col, values[l]); 

			//If the value is invalid, undo the change by zero-ing the cell. 
			if (!checkValidCell(matrix, row, col))
				matrix.setMatrixCell(row, col, 0);
			else {
				isCellFilled = true;
				break;
			}
		}
		if (!isCellFilled)
			return false;
	}
	return true;
}

void outputMatrix(const SudokuMatrix& matrix, string fileName) {
	ofstream outputFile(fileName);
	
	outputFile << matrix.getM() << " " << matrix.getN() << " " << matrix.getP() << " "
	 << matrix.getQ() << endl;

	for (int i = 0; i < matrix.getN(); i ++){
		for (int j = 0; j < matrix.getN(); j++){
			int cell = matrix.getMatrixCell(i, j);
			char convertedCell = cell > 9 ? (char)(55+cell): (char)(48+cell);
			outputFile << convertedCell << " ";
		}
		outputFile << endl;
	}
}

int main(int argc, char* argv[])
{
	//if (argc < 2)
	//	return -1;

	SudokuMatrix matrix = parseInput("testMatrix.txt");
	while (!fillMatrix(matrix)) {
		cout << "Filling" << endl;
		matrix = parseInput("testMatrix.txt");
	}
	
	outputMatrix(matrix,"outputMatrix.txt");
	return 0;
}

