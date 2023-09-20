#pragma once

#include "util.hpp"

#include <algorithm>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <complex>
#include <cstddef>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
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

	~token(){};
};

struct rec {
	size_t x1;
	size_t y1;
	size_t x2;
	size_t y2;

	rec(size_t y, size_t x) {
		x1 = x;
		y1 = y;
		x2 = x;
		y2 = y;
	}
};

inline bool operator<(const rec &l, const rec &r);

std::map<int, int> genFullTable(const int cnt);
std::vector<int> genGreyTable(const int cnt);
std::vector<std::vector<int>> genKarnaugh(std::map<int, int> &ftable, int varNum);
std::vector<std::vector<int>> expandK(const std::vector<std::vector<int>> &karnaugh);
void expandR(rec &r, const std::vector<std::vector<int>> &ex);
std::vector<rec> splitRec(rec &r, size_t h, size_t w);
std::set<rec> cutKarnaugh(const std::vector<std::vector<int>> &karnaugh);
token *makeExpression(std::set<rec> recs, int varNum);

bool calcExpression(token *root, int args, int values);

void printExpression(const token *root);
void printExpressionValues(const token *root, int args, int values);
void printEx(const std::vector<std::vector<int>> &e);
void printKarnaugh(const std::vector<std::vector<int>> &karnaugh);
