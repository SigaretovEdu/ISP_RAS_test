#include "main.hpp"

#include "util.hpp"

#include <algorithm>
#include <climits>
#include <cmath>
#include <complex>
#include <cstddef>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

void cutCarno(const std::vector<std::vector<int>> &carno) {
	for(size_t i = 0; i < carno.size(); ++i) {
		for(size_t j = 0; j < carno[0].size(); ++j) {
			if(carno[i][j] == 1) {
				std::cout << i << ' ' << j << '\n';
			}
		}
	}
}

int main() {
	int varNum = 4;

	// gen ftable
	// std::map<int, int> ftable = genFullTable(varNum);
	std::map<int, int> ftable = {
		{0, 0}, {1, 0}, {2, 0},	 {3, 0},  {4, 1},  {5, 1},	{6, 0},	 {7, 1},
		{8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 1}, {13, 1}, {14, 0}, {15, 1},
	};
	std::cout << "\ngeneration of truth ftable:\n";
	bprint(ftable, varNum);

	// gen carno map
	std::vector<std::vector<int>> carno = genCarno(ftable, varNum);
	std::cout << "\ncarno map:\n";
	printCarno(carno);

	cutCarno(carno);

	return 0;
}

std::map<int, int> genFullTable(const int cnt) {
	int limit = static_cast<int>(std::pow(2, cnt));

	std::map<int, int> ftable;

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 1);

	for(int i = 0; i < limit; ++i) {
		ftable[i] = uni(rng) % 2;
	}

	return ftable;
}

std::vector<int> genGreyTable(const int cnt) {
	size_t limit = static_cast<size_t>(std::pow(2, cnt));
	std::vector<int> gtable;
	gtable.reserve(limit);
	gtable.push_back(0);
	gtable.push_back(1);
	int pos = 1;

	while(gtable.size() < limit) {
		for(auto it = gtable.rbegin(); it != gtable.rend(); ++it) {
			gtable.push_back(*it);
		}
		for(size_t i = 0; i < gtable.size() / 2; ++i) {
			gtable[i] += (0 << pos);
		}
		for(size_t i = gtable.size() / 2; i < gtable.size(); ++i) {
			gtable[i] += (1 << pos);
		}
		++pos;
	}
	return gtable;
}

void printExpression(const token *root) {
	if(root == nullptr) {
		throw std::runtime_error("tokens structure error: catch nullptr child");
	}
	if(root->tokenType == 'v') {
		if(root->inv) {
			std::cout << '!';
		}
		std::cout << "X" << root->id;
		return;
	}
	else {
		if(root->opType == 'a') {
			if(root->left->opType == 'o') {
				std::cout << '(';
				printExpression(root->left);
				std::cout << ')';
			}
			else {
				printExpression(root->left);
			}
			std::cout << '*';
			if(root->right->opType == 'o') {
				std::cout << '(';
				printExpression(root->right);
				std::cout << ')';
			}
			else {
				printExpression(root->right);
			}
		}
		else {
			printExpression(root->left);
			std::cout << '+';
			printExpression(root->right);
		}
		return;
	}
}

void printExpressionValues(const token *root, int args, int values) {
	if(root == nullptr) {
		throw std::runtime_error("tokens structure error: catch nullptr child");
	}
	if(root->tokenType == 'v') {
		if(root->inv) {
			std::cout << '!';
		}
		std::cout << ((values >> (args - root->id - 1)) & 1);
		return;
	}
	else {
		if(root->opType == 'a') {
			if(root->left->opType == 'o') {
				std::cout << '(';
				printExpressionValues(root->left, args, values);
				std::cout << ')';
			}
			else {
				printExpressionValues(root->left, args, values);
			}
			std::cout << '*';
			if(root->right->opType == 'o') {
				std::cout << '(';
				printExpressionValues(root->right, args, values);
				std::cout << ')';
			}
			else {
				printExpressionValues(root->right, args, values);
			}
		}
		else {
			printExpressionValues(root->left, args, values);
			std::cout << '+';
			printExpressionValues(root->right, args, values);
		}
		return;
	}
}

bool calcExpression(token *root, int args, int values) {
	if(root == nullptr) {
		throw std::runtime_error("tokens structure error: catch nullptr child");
	}
	if(root->tokenType == 'v') {
		if(!root->inv)
			return static_cast<bool>((values >> (args - root->id - 1)) & 1);
		else
			return !static_cast<bool>((values >> (args - root->id - 1)) & 1);
	}
	if(root->opType == 'a') {
		return calcExpression(root->left, args, values) & calcExpression(root->right, args, values);
	}
	if(root->opType == 'o') {
		return calcExpression(root->left, args, values) | calcExpression(root->right, args, values);
	}
	return false;
}

void printCarno(const std::vector<std::vector<int>> &carno) {
	int tmp1 = static_cast<int>(carno.size());
	int tmp2 = static_cast<int>(carno[0].size());
	int leftNums = 0, rightNums = 0;
	while(tmp1 > 1) {
		++leftNums;
		tmp1 /= 2;
	}
	while(tmp2 > 1) {
		++rightNums;
		tmp2 /= 2;
	}

	std::vector<int> leftGTabel = genGreyTable(leftNums);
	std::vector<int> rightGTabel = genGreyTable(rightNums);

	std::cout << "\t";
	bprint(rightGTabel, rightNums, '\t');
	std::cout << '\n';
	for(size_t i = 0; i < leftGTabel.size(); ++i) {
		std::cout << decToBin(leftGTabel[i], leftNums) << '\t';
		print(carno[i], '\t');
	}
	return;
}

std::vector<std::vector<int>> genCarno(std::map<int, int> &ftable, int varNum) {
	int leftNums = static_cast<int>(std::floor(varNum / 2.0));
	int rightNums = static_cast<int>(std::ceil(varNum / 2.0));

	std::vector<int> leftGTabel = genGreyTable(leftNums);
	std::vector<int> rightGTabel = genGreyTable(rightNums);

	std::vector<std::vector<int>> carno(leftGTabel.size(), std::vector<int>(rightGTabel.size()));

	for(size_t i = 0; i < carno.size(); ++i) {
		for(size_t j = 0; j < carno[0].size(); ++j) {
			carno[i][j] = ftable[(leftGTabel[i] << rightNums) + rightGTabel[j]];
		}
	}

	return carno;
}

