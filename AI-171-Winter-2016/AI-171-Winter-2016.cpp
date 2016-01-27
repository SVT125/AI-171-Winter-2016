// AI-171-Winter-2016.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "AI-171-Winter-2016.h"
#include "BTSolver.h"
#include <time.h>
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

bool isTimedOut(long curr, long limit) {
	return curr - limit > 0;
}

SudokuMatrix* parseInput(string fileName) {
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
		return nullptr;
	} else
		return new SudokuMatrix(m,n,p,q);
}

//Returns true if we were successfully able to fill the matrix, false otherwise.
bool fillMatrix(SudokuMatrix* matrix, clock_t begin, int limit) {
	//Initialize a vector of shuffled cell row/index points, to linearly take from when populating the matrix->
	int cellsIndex = 0;
	vector<pair<int, int>> cells;
	for (int i = 0; i < matrix->getN(); i++)
		for (int j = 0; j < matrix->getN(); j++)
			cells.push_back(make_pair(i, j));
	random_shuffle(cells.begin(), cells.end());

	for (int i = 0; i < matrix->getM(); i++) {
		//If timed out, return.
		if (isTimedOut(clock() - begin, limit))
			return true;

		pair<int, int> cell = cells[cellsIndex++];
		int row = cell.first, col = cell.second, index = 1;
		bool isCellFilled = false;

		//Store the values, row, and col indices possible to be shuffled later.
		vector<int> values(matrix->getN(), 0);

		//Initialize the vectors to their possible values; convert to letters if N > 9.
		for (size_t j = 0; j < values.size(); j++)
			values[j] = index++;

		//Shuffle the vectors for random picking.
		random_shuffle(values.begin(), values.end());

		for (size_t l = 0; l < values.size(); l++) {
			matrix->setMatrixCell(row, col, values[l]);

			//If the value is invalid, undo the change by zero-ing the cell. 
			if (!SudokuMatrix::checkValidCell(matrix, row, col))
				matrix->setMatrixCell(row, col, 0);
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

void outputMatrix(const SudokuMatrix* matrix, string fileName) {
	ofstream outputFile(fileName);
	
	outputFile << matrix->getM() << " " << matrix->getN() << " " << matrix->getP() << " "
	 << matrix->getQ() << endl;

	for (int i = 0; i < matrix->getN(); i ++){
		for (int j = 0; j < matrix->getN(); j++){
			int cell = matrix->getMatrixCell(i, j);
			//Convert any number above 10 via ASCII.
			char convertedCell = cell > 9 ? (char)(55+cell): (char)(48+cell);
			outputFile << convertedCell << " ";
		}
		outputFile << endl;
	}
	outputFile.close();
}

SudokuMatrix* readInput(std::string fileName) {
	ifstream inputFile(fileName);
	int n, p, q;
	string line;

	getline(inputFile, line, ' ');
	n = stoi(line);
	getline(inputFile, line, ' ');
	p = stoi(line);
	getline(inputFile, line, ' ');
	q = stoi(line);

	SudokuMatrix* matrix = new SudokuMatrix(-1, n, p, q);

	for (int i = 0; i < n; i++) {
		//Get the line, split by whitespace and feed into the vector.
		getline(inputFile, line);
		istringstream buffer(line);
		vector<string> ret;
		copy(istream_iterator<string>(buffer),
			istream_iterator<string>(),
			back_inserter(ret));

		for (int j = 0; j < n; j++) {
			matrix->setMatrixCell(i, j,stoi(ret[i]));
		}
	}

	return matrix;
}

void outputLog(SudokuMatrix* matrix, string fileName, int flag, clock_t start, clock_t s_start, clock_t s_end, vector<Variable> vars, int nodes, int bts) {
	ofstream outputFile(fileName);

	outputFile << "TOTAL_START = " << start << endl;
	outputFile << "PREPROCESSING_START = " << "Not applicable" << endl; //Redo for AC preprocessing later
	outputFile << "PREPROCESSING_END = " << "Not applicable" << endl; //Redo for AC preprocessing later
	outputFile << "SEARCH_START = " << s_start << endl;
	outputFile << "SEARCH_DONE = " << s_end << endl;
	outputFile << "STATUS = ";

	switch (flag) {
		case -1: outputFile << "error" << endl; break;
		case 0: outputFile << "timeout" << endl; break;
		case 1: outputFile << "success" << endl; break;
	}

	outputFile << "SOLUTION = ";

	if (vars.size() == 0)
		outputFile << "NONE" << endl;
	else 
		for (int i = 0; i < vars.size(); i++)
			outputFile << to_string(vars[i].getValue()) << " ";
	
	outputFile << endl;

	outputFile << "NODES = " << nodes << endl;
	outputFile << "DEADENDS = " << bts << endl;
}

int main(int argc, char* argv[])
{
	SudokuMatrix* matrix;
	int flag;
	clock_t begin = clock();

	if (argc < 5)
		return -1;

	int limit = atoi(argv[3]) * CLOCKS_PER_SEC;
	string inputFile = argv[1], outputFile = argv[2];
		

	matrix = strcmp(argv[4], "GEN") == 0 ? parseInput(inputFile) : readInput(inputFile);

	if (matrix == nullptr) {
		cout << "Failed to retrieve matrix." << endl;
		return -1;
	}

	if (strcmp(argv[4], "GEN") == 0) {
		while (!fillMatrix(matrix, begin, limit) && !isTimedOut(clock() - begin, limit)) {
			matrix = parseInput(inputFile);
		}

		if (isTimedOut(begin, clock())) {
			ofstream outputFile(outputFile);
			outputFile << "Timeout." << endl;
			outputFile.close();
			return -1;
		}

		outputMatrix(matrix, outputFile);
	}
	else if (strcmp(argv[4], "BT") == 0) {
		clock_t s_start, s_end;
		try {
			s_start = clock() - begin, s_end;
			BTSolver solver(matrix);
			flag = solver.solve();
			s_end = clock() - begin;
			outputLog(matrix, outputFile, flag, begin, s_start, s_end,solver.getVariables(),solver.getNodes(),solver.getBacktracks());
		}
		catch (exception& e) {
			vector<Variable> vars;
			outputLog(matrix, outputFile, -1, begin, s_start, clock() - begin,vars,0,0);
		}
	}
	else {
		//Fill in the rest of the arguments...
	}

	delete matrix;

	return 0;
}

