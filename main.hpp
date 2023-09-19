#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

struct token {
	char tokenType;
	char opType;
	token *left;
	token *right;
	int id;
	bool inv;

	token(int a, bool f = false) {
		tokenType = 'v';
		id = a;
		inv = f;
		left = nullptr;
		right = nullptr;
	}
	token(char op) {
		tokenType = 'o';
		opType = op;
		left = nullptr;
		right = nullptr;
	}
	token(char op, token *l, token *r) {
		tokenType = 'o';
		opType = op;
		left = l;
		right = r;
	}
};

void genTable(const int cnt, std::vector<int> &table);
std::string decToBin(int a, const size_t format);
void printExpression(const token *root);
void printExpressionValues(const token *root, int args, int values);
bool calcExpression(token *root, int args, int values);
