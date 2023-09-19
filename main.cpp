#include "main.hpp"

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

struct dot {
	size_t x;
	size_t y;
	dot *right;
	dot *down;
	dot *left;
	dot *up;

	dot(size_t x, size_t y) {
		x = x;
		y = y;
		right = nullptr;
		down = nullptr;
		left = nullptr;
		up = nullptr;
	}
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

inline bool operator<(const rec &l, const rec &r) {
	if(l.x1 != r.x1) {
		return l.x1 < r.x1;
	}
	else if(l.x2 != r.x2) {
		return l.x2 < r.x2;
	}
	else if(l.y1 != r.y1) {
		return l.y1 < r.y1;
	}
	else {
		return l.y2 < r.y2;
	}
}

std::vector<std::vector<int>> expandK(const std::vector<std::vector<int>> &karnaugh) {
	size_t h = karnaugh.size(), w = karnaugh[0].size();

	std::vector<std::vector<int>> e(h * 3, std::vector<int>(w * 3, 0));

	for(size_t i = 0; i < h; ++i) {
		bool f = false;
		for(size_t j = 0; j < w; ++j) {
			e[i + h][j + w] = karnaugh[i][j];
			if(karnaugh[i][j] == 0)
				f = true;
		}

		if(f) {
			for(size_t j = 0; j < w; ++j) {
				e[i + h][j] = karnaugh[i][j];
				e[i + h][j + 2 * w] = karnaugh[i][j];
			}
		}
	}

	for(size_t j = 0; j < w; ++j) {
		bool f = false;
		for(size_t i = 0; i < h; ++i) {
			if(karnaugh[i][j] == 0)
				f = true;
		}

		if(f) {
			for(size_t i = 0; i < h; ++i) {
				e[i][j + w] = karnaugh[i][j];
				e[i + 2 * h][j + w] = karnaugh[i][j];
			}
		}
	}

	// int cnt1 = 0, cnt2 = 0;
	// for(size_t i = 0; i < e.size(); ++i) {
	// 	for(size_t j = 0; j < e[0].size(); ++j) {
	// 		std::cout << e[i][j];
	// 		++cnt2;
	// 		if(cnt2 == karnaugh[0].size()) {
	// 			cnt2 = 0;
	// 			std::cout << ' ';
	// 		}
	// 	}
	// 	std::cout << '\n';
	// 	++cnt1;
	// 	if(cnt1 == karnaugh.size()) {
	// 		cnt1 = 0;
	// 		std::cout << '\n';
	// 	}
	// }

	return e;
}

void expandR(rec &r, const std::vector<std::vector<int>> &ex) {
	size_t h = ex.size(), w = ex[0].size();

	// try right
	while(ex[r.y2][r.x2 + 1] == 1) {
		++r.x2;
	}
	// try left
	while(ex[r.y1][r.x1 - 1] == 1) {
		--r.x1;
	}
	// try down
	while(true) {
		bool f = true;
		for(size_t j = r.x1; j <= r.x2; ++j) {
			if(ex[r.y2 + 1][j] == 0) {
				f = false;
				break;
			}
		}
		if(f) {
			++r.y2;
		}
		else {
			break;
		}
	}
	// try up
	while(true) {
		bool f = true;
		for(size_t j = r.x1; j <= r.x2; ++j) {
			if(ex[r.y1 - 1][j] == 0) {
				f = false;
				break;
			}
		}
		if(f) {
			--r.y1;
		}
		else {
			break;
		}
	}
}

void cutKarnaugh(const std::vector<std::vector<int>> &karnaugh) {
	size_t h = karnaugh.size(), w = karnaugh[0].size();

	std::cout << '\n';
	print(karnaugh);
	std::cout << '\n';

	std::vector<std::vector<int>> ex = expandK(karnaugh);

	std::set<rec> recs;
	std::vector<std::vector<bool>> vis(h, std::vector<bool>(w, false));
	for(size_t i = 0; i < h; ++i) {
		for(size_t j = 0; j < w; ++j) {
			if(karnaugh[i][j] == 1 && !vis[i][j]) {
				rec r(i + h, j + w);
				expandR(r, ex);
				r.x1 %= w;
				r.x2 %= w;
				r.y1 %= h;
				r.y2 %= h;
				for(size_t y = r.y1; y <= r.y2; ++y) {
					for(size_t x = r.x1; x <= r.x2; ++x) {
						vis[y][x] = true;
					}
				}
				recs.insert(r);
			}
		}
	}

	for(auto const &it: recs) {
		std::cout << "rec:\n";
		std::cout << '\t' << it.y1 << ' ' << it.x1 << '\n';
		std::cout << '\t' << it.y2 << ' ' << it.x2 << '\n';
	}
}

int main() {
	int varNum = 4;

	// gen ftable
	// std::map<int, int> ftable = genFullTable(varNum);
	std::map<int, int> ftable = {
		{0, 1}, {1, 1}, {2, 1},	 {3, 1},  {4, 0},  {5, 0},	{6, 0},	 {7, 1},
		{8, 0}, {9, 0}, {10, 0}, {11, 1}, {12, 1}, {13, 0}, {14, 1}, {15, 0},
	};
	std::cout << "\ngeneration of truth ftable:\n";
	bprint(ftable, varNum);

	// gen carno map
	std::vector<std::vector<int>> karnaugh = genKarnaugh(ftable, varNum);
	std::cout << "\ncarno map:\n";
	printKarnaugh(karnaugh);

	cutKarnaugh(karnaugh);

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

void printKarnaugh(const std::vector<std::vector<int>> &karnaugh) {
	int tmp1 = static_cast<int>(karnaugh.size());
	int tmp2 = static_cast<int>(karnaugh[0].size());
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
		print(karnaugh[i], '\t');
	}
	return;
}

std::vector<std::vector<int>> genKarnaugh(std::map<int, int> &ftable, int varNum) {
	int leftNums = static_cast<int>(std::floor(varNum / 2.0));
	int rightNums = static_cast<int>(std::ceil(varNum / 2.0));

	std::vector<int> leftGTabel = genGreyTable(leftNums);
	std::vector<int> rightGTabel = genGreyTable(rightNums);

	std::vector<std::vector<int>> karnaugh(leftGTabel.size(), std::vector<int>(rightGTabel.size()));

	for(size_t i = 0; i < karnaugh.size(); ++i) {
		for(size_t j = 0; j < karnaugh[0].size(); ++j) {
			karnaugh[i][j] = ftable[(leftGTabel[i] << rightNums) + rightGTabel[j]];
		}
	}

	return karnaugh;
}

