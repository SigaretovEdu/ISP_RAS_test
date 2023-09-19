#include "main.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
	std::vector<int> vec;
	genTable(2, vec);

	token A(0, true), B(1), C(0), D(1, true);
	token AND1('a', &A, &B);
	token AND2('a', &C, &D);
	token OR('o', &AND1, &AND2);

	printExpression(&OR);
	std::cout << '\n';

	for(size_t i = 0; i < vec.size(); ++i) {
		std::cout << decToBin(vec[i], 2) << " = " << calcExpression(&OR, 2, vec[i]) << '\n';
	}

	return 0;
}

void genTable(const int cnt, std::vector<int> &table) {
	for(int i = 0; i < static_cast<int>(std::pow(2, cnt)); ++i) {
		table.push_back(i);
	}
}

std::string decToBin(int a, const size_t format = 0) {
	std::string out = "";
	while(a > 0) {
		out.push_back((a % 2) + '0');
		a /= 2;
	}
	while(out.size() < format)
		out.push_back('0');
	std::reverse(out.begin(), out.end());
	return out;
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
