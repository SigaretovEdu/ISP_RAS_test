#ifndef KARNAUGH
#define KARNAUGH

#include <algorithm>
#include <cmath>
#include <complex>
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
void freeTokens(token *root);

void read();

std::vector<int> genTable(const int varNum);
std::map<int, int> genFullTable(const int varNum, int num = -1);
std::vector<int> genGreyTable(const int varNum);
std::vector<std::vector<int>> genKarnaugh(std::map<int, int> &ftable, const int varNum);
std::vector<std::vector<int>> expandK(const std::vector<std::vector<int>> &karnaugh);
void expandR(rec &r, const std::vector<std::vector<int>> &ex);
std::vector<rec> splitRec(rec r, const size_t h, const size_t w);
std::vector<rec> cutKarnaugh(const std::vector<std::vector<int>> &karnaugh);
token *makeExpression(const std::vector<rec> recs, const int varNum);
token *makeFExplression(const std::map<int, int> &ftable, const int varNum);

bool calcExpression(const token *root, const int args, const int values);
void compareExpressions(const std::vector<token *> functionsList, const int varNum);

void printExpression(const token *root);
void printExpanded(const std::vector<std::vector<int>> &e);
void printKarnaugh(const std::vector<std::vector<int>> &karnaugh);
std::string decToBin(int a, const size_t format = 0);
void bprint(const std::vector<int> &a, const size_t format = 4, const char del = ' ');
void bprint(const std::map<int, int> &m, const size_t format, const char del = ' ');

void test(const int Varnum, const int seed = -1);

#endif
