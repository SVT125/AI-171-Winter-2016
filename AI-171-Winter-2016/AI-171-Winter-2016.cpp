// AI-171-Winter-2016.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <time.h>
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

bool isTimedOut(long begin, long end) {
	return end - begin > 0;
}

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
		ofstream outputFile(fileName);
		outputFile << "Error: Invalid input parameters" << endl;
		outputFile.close();
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
bool fillMatrix(SudokuMatrix& matrix, clock_t begin, int limit) {
	//Initialize a vector of shuffled cell row/index points, to linearly take from when populating the matrix.
	int cellsIndex = 0;
	vector<pair<int, int>> cells;
	for (int i = 0; i < matrix.getN(); i++)
		for (int j = 0; j < matrix.getN(); j++)
			cells.push_back(make_pair(i, j));
	random_shuffle(cells.begin(), cells.end());

	while (!isTimedOut(begin, clock())) {
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
			random_shuffle(values.begin(), values.end());

			for (int l = 0; l < values.size(); l++) {
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
			//Convert any number above 10 via ASCII.
			char convertedCell = cell > 9 ? (char)(55+cell): (char)(48+cell);
			outputFile << convertedCell << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

int main(int argc, char* argv[])
{
	//if (argc < 2)
	//	return -1;

	int limit = 60 * CLOCKS_PER_SEC;

	if (argc > 2)
		limit = atoi(argv[2]) * CLOCKS_PER_SEC;

	string inputFile = "testMatrix.txt", outputFile = "outputMatrix.txt";
	clock_t begin = clock();

	SudokuMatrix matrix = parseInput(inputFile);
	while (!fillMatrix(matrix, begin, limit) && !isTimedOut(begin, clock())) {
		matrix = parseInput(outputFile);
	}
	
	if (isTimedOut(begin, clock())) {
		ofstream outputFile(outputFile);
		outputFile << "Timeout." << endl;
		outputFile.close();
		return -1;
	}

	outputMatrix(matrix,outputFile);
	return 0;
}

