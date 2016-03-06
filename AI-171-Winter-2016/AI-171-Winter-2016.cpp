// AI-171-Winter-2016.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "AI-171-Winter-2016.h"
#include "BTSolver.h"
#include "LocalSolver.h"
#include <time.h>
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <iterator>

using namespace std;

bool isTimedOut(long curr, long limit) {
	return curr - limit >= 0;
}

SudokuMatrix* parseInput(string fileName) {
	ifstream inputFile(fileName);

	int m, n, p, q;
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
	}
	else
		return new SudokuMatrix(m, n, p, q);
}

//Returns true if we were successfully able to fill the matrix, false otherwise.
bool fillMatrix(SudokuMatrix* matrix, clock_t begin, int limit) {
	//Initialize a vector of shuffled cell row/index points, to linearly take from when populating the matrix->
	int cellsIndex = 0;
	vector<pair<int, int>> cells;
	default_random_engine generator(random_device{}());

	for (int i = 0; i < matrix->getN(); i++)
		for (int j = 0; j < matrix->getN(); j++)
			cells.push_back(make_pair(i, j));
	shuffle(cells.begin(), cells.end(), generator);

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
		shuffle(values.begin(), values.end(), generator);

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

	outputFile << matrix->getN() << " " << matrix->getP() << " "
		<< matrix->getQ() << endl;

	for (int i = 0; i < matrix->getN(); i++){
		for (int j = 0; j < matrix->getN(); j++){
			int cell = matrix->getMatrixCell(i, j);
			//Convert any number above 10 via ASCII.
			char convertedCell = cell > 9 ? (char)(55 + cell) : (char)(48 + cell);
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
	getline(inputFile, line);
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

		for (int j = 0; j < n; j++)
			matrix->setMatrixCell(i, j, atoi(ret[j].c_str()));
	}

	return matrix;
}

void outputLog(SudokuMatrix* matrix, string fileName, int flag, clock_t start, clock_t s_start, clock_t s_end, vector<Variable> vars, int nodes, int bts) {
	ofstream outputFile(fileName);

	outputFile << "TOTAL_START = " << start << endl;
	outputFile << "PREPROCESSING_START = " << 0 << endl; //Redo for AC preprocessing later
	outputFile << "PREPROCESSING_END = " << 0 << endl; //Redo for AC preprocessing later
	outputFile << "SEARCH_START = " << ((double)(s_start)) / CLOCKS_PER_SEC << endl;
	outputFile << "SEARCH_DONE = " << ((double)(s_end)) / CLOCKS_PER_SEC << endl;
	outputFile << "STATUS = ";

	switch (flag) {
	case -1: outputFile << "error" << endl; break;
	case 0: outputFile << "timeout" << endl; break;
	case 1: outputFile << "success" << endl; break;
	}

	outputFile << "SOLUTION = ";

	if (vars.size() == 0)
		outputFile << "NONE" << endl;
	else {
		outputFile << "(";
		for (int i = 0; i < vars.size(); i++) {
			int val = vars[i].getValue();
			outputFile << (val > 9 ? (char)(55 + val) : (char)(48 + val));
			if (i == vars.size() - 1)
				outputFile << ")" << endl;
			else
				outputFile << ",";
		}
	}

	outputFile << endl;

	outputFile << "NODES = " << nodes << endl;
	outputFile << "DEADENDS = " << bts << endl;
}

bool findFlag(int argc, char* argv[], std::string flag) {
	for (int i = 0; i < argc; i++)
		if (strcmp(argv[i], flag.c_str()) == 0)
			return true;
	return false;
}

int main(int argc, char* argv[])
{
	SudokuMatrix* matrix;
	int flag;
	clock_t begin = clock();
	bool doGen = findFlag(argc, argv, "GEN"), doFC = findFlag(argc, argv, "FC"), doMRV = findFlag(argc, argv, "MRV"), doDH = findFlag(argc, argv, "DH"),
		doLCV = findFlag(argc, argv, "LCV"), doLS = findFlag(argc, argv, "LS");

	if (argc < 4)
		return -1;

	clock_t limit = atof(argv[3]) * CLOCKS_PER_SEC;
	string inputFileName = argv[1], outputFileName = argv[2];

	//If more than 4 arguments, then we can specify GEN. Otherwise if there are no flags, then default to BT + FC.
	if (argc > 4 && !doLS)
		matrix = doGen ? parseInput(inputFileName) : readInput(inputFileName);
	else
		matrix = readInput(inputFileName);

	if (matrix == nullptr) {
		cout << "Failed to retrieve matrix." << endl;
		return -1;
	}

	if (doGen) {
		while (!fillMatrix(matrix, begin, limit) && !isTimedOut(clock() - begin, limit)) {
			matrix = parseInput(inputFileName);
		}

		if (isTimedOut(clock() - begin, limit)) {
			ofstream outputFile(outputFileName);
			outputFile << "Timeout." << endl;
			outputFile.close();
			return -1;
		}

		outputMatrix(matrix, outputFileName);
	} else if(!doLS) {
		clock_t s_start, s_end;
		try {
			s_start = clock() - begin, s_end;
			BTSolver solver(matrix);
			flag = argc == 4 ? solver.solve(begin, limit, false, false, false, false) : solver.solve(begin, limit, doFC, doMRV, doDH, doLCV);
			s_end = clock() - begin;
			outputLog(matrix, outputFileName, flag, 0, s_start, s_end, solver.getVariableVector(), solver.getNodes(), solver.getBacktracks());
		}
		catch (exception& e) {
			vector<Variable> vars;
			outputLog(matrix, outputFileName, -1, 0, s_start, clock() - begin, vars, 0, 0);
		}
	} else {
		clock_t s_start, s_end;
		try {
			s_start = clock() - begin, s_end;
			LocalSolver solver(matrix);
			flag = solver.applyLocalSearch(begin,limit);
			s_end = clock() - begin;
			outputLog(matrix, outputFileName, flag, 0, s_start, s_end, solver.getVariableVector(), 0, 0);
		}
		catch (exception& e) {
			vector<Variable> vars;
			outputLog(matrix, outputFileName, -1, 0, s_start, clock() - begin, vars, 0, 0);
		}
	}

	delete matrix;

	return 0;
}

