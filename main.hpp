#pragma once

#include <cstddef>
#include <iostream>
#include <map>
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

std::map<int, int> genFullTable(const int cnt);
std::vector<int> genGreyTable(const int cnt);
void printKarnaugh(const std::vector<std::vector<int>> &karnaugh);
std::vector<std::vector<int>> genKarnaugh(std::map<int, int> &ftable, int varNum);

std::string decToBin(int a, const size_t format);
void printExpression(const token *root);
void printExpressionValues(const token *root, int args, int values);
bool calcExpression(token *root, int args, int values);
