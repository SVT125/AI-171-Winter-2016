#ifndef SUDOKU_MATRIX
#define SUDOKU_MATRIX

#include<vector>

class SudokuMatrix {
private:
	std::vector<std::vector<int>> matrix;
	int m, n, p, q;

public:
	SudokuMatrix(int m, int n, int p, int q);

	SudokuMatrix(const SudokuMatrix& sm);

	std::vector<std::vector<int>> getMatrix() const;
	int getMatrixCell(int row, int col) const;
	void setMatrixCell(int row, int col, int value);
	int getM() const;
	int getN() const;
	int getP() const;
	int getQ() const;
	static std::pair<int, int> getBlock(const SudokuMatrix* matrix, int row, int col);
	static bool checkValidCell(const SudokuMatrix* matrix, int row, int col);
};

#endif