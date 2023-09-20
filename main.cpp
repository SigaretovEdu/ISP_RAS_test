#include "main.hpp"

/*
!X0*!X1 + !X0*X2*X3 + !X1*X2*X3 + X0*X1*!X3
*/

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

	std::set<rec> cuts = cutKarnaugh(karnaugh);
	std::cout << "\ncuts from karnaugh map:\n";
	for(auto const &it: cuts) {
		std::cout << "rec:\n";
		std::cout << '\t' << it.y1 << ' ' << it.x1 << '\v';
		std::cout << ' ' << it.y2 << ' ' << it.x2 << '\n';
	}

	token *expression = makeExpression(cuts, varNum);
	std::cout << '\n';
	printExpression(expression);
	std::cout << '\n';

	return 0;
}

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

std::vector<rec> splitRec(rec &r, size_t h, size_t w) {
	if(r.x1 > r.x2)
		r.x2 += w;
	if(r.y1 > r.y2)
		r.y2 += h;

	std::vector<rec> recs;
	recs.push_back(r);
	int nh = std::pow(2, static_cast<int>(log2(r.y2 - r.y1 + 1)));
	int nw = std::pow(2, static_cast<int>(log2(r.x2 - r.x1 + 1)));

	if(nh != (r.y2 - r.y1 + 1)) {
		recs.push_back(r);
		recs[0].y2 = recs[0].y1 + nh - 1;
		recs[1].y1 = recs[1].y2 - nh + 1;
	}

	if(nw != (r.x2 - r.x1 + 1)) {
		recs.push_back(recs[0]);
		recs.push_back(recs[1]);
		recs[0].x2 = recs[0].x1 + nw - 1;
		recs[2].x1 = recs[2].x2 - nw + 1;
		recs[1].x2 = recs[1].x1 + nw - 1;
		recs[3].x1 = recs[3].x2 - nw + 1;
	}

	for(size_t i = 0; i < recs.size(); ++i) {
		recs[i].x1 %= w;
		recs[i].x2 %= w;
		recs[i].y1 %= h;
		recs[i].y2 %= h;
	}

	return recs;
}

std::set<rec> cutKarnaugh(const std::vector<std::vector<int>> &karnaugh) {
	size_t h = karnaugh.size(), w = karnaugh[0].size();

	std::vector<std::vector<int>> ex = expandK(karnaugh);
	std::vector<std::vector<bool>> vis(h, std::vector<bool>(w, false));
	std::set<rec> recs;

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
				std::vector<rec> splitRecs = splitRec(r, h, w);

				for(size_t k = 0; k < splitRecs.size(); ++k)
					recs.insert(splitRecs[k]);
			}
		}
	}

	return recs;
}

token *makeExpression(std::set<rec> recs, int varNum) {
	int leftNums = static_cast<int>(std::floor(varNum / 2.0));
	int rightNums = static_cast<int>(std::ceil(varNum / 2.0));
	size_t h = std::pow(2, leftNums);
	size_t w = std::pow(2, rightNums);

	std::vector<int> leftGTabel = genGreyTable(leftNums);
	std::vector<int> rightGTabel = genGreyTable(rightNums);

	std::vector<token *> tokenList;

	for(auto it: recs) {
		int left = leftGTabel[it.y1];
		int right = rightGTabel[it.x1];
		int lmask = 0, rmask = 0;

		if(it.x2 < it.x1)
			it.x2 += w;
		if(it.y2 < it.y1)
			it.y2 += h;

		for(size_t i = it.y1 + 1; i <= it.y2; ++i) {
			lmask |= left ^ leftGTabel[i % h];
		}
		for(size_t i = it.x1 + 1; i <= it.x2; ++i) {
			rmask |= right ^ rightGTabel[i % w];
		}

		lmask = (1 << leftNums) - 1 - lmask;
		rmask = (1 << rightNums) - 1 - rmask;
		int expr = (left << rightNums) + right;
		int mask = (lmask << rightNums) + rmask;

		int pos = varNum - 1;
		std::vector<token *> vars;
		while(mask > 0) {
			if(mask % 2) {
				int inv = !((expr >> (varNum - pos - 1)) % 2);
				token *tmp = new token(pos, inv);
				vars.push_back(tmp);
			}
			mask /= 2;
			--pos;
		}
		std::reverse(vars.begin(), vars.end());

		if(vars.size() == 1) {
			tokenList.push_back(vars[0]);
		}
		else {
			token *AND = new token('a', vars[0], vars[1]);
			if(vars.size() == 2) {
				tokenList.push_back(AND);
			}
			else {
				size_t pos = 2;
				token *tmp = AND;
				while(pos < vars.size()) {
					token *AND1 = new token('a', tmp, vars[pos]);
					++pos;
					tmp = AND1;
				}
				tokenList.push_back(tmp);
			}
		}
	}

	if(tokenList.size() == 1) {
		return tokenList[0];
	}
	else {
		token *OR = new token('o', tokenList[0], tokenList[1]);
		if(tokenList.size() == 2) {
			return OR;
		}
		else {
			size_t pos = 2;
			token *tmp = OR;
			while(pos < tokenList.size()) {
				token *OR1 = new token('o', tmp, tokenList[pos]);
				++pos;
				tmp = OR1;
			}
			return tmp;
		}
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
			std::cout << "*";
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
			std::cout << " + ";
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
			std::cout << "*";
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
			std::cout << " + ";
			printExpressionValues(root->right, args, values);
		}
		return;
	}
}

void printEx(const std::vector<std::vector<int>> &e) {
	int cnt1 = 0, cnt2 = 0;
	size_t h = e.size() / 3, w = e[0].size() / 3;
	for(size_t i = 0; i < e.size(); ++i) {
		for(size_t j = 0; j < e[0].size(); ++j) {
			std::cout << e[i][j];
			++cnt2;
			if(cnt2 == w) {
				cnt2 = 0;
				std::cout << ' ';
			}
		}
		std::cout << '\n';
		++cnt1;
		if(cnt1 == h) {
			cnt1 = 0;
			std::cout << '\n';
		}
	}
}

void printKarnaugh(const std::vector<std::vector<int>> &karnaugh) {
	int leftNums = static_cast<int>(log2(karnaugh.size()));
	int rightNums = static_cast<int>(log2(karnaugh[0].size()));

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

