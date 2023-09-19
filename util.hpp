#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

template<typename T>
void print(const std::vector<std::vector<T>>& a, char del = ' ') {
	if(a.size() == 0)
		std::cout << "_\n";
	for(size_t i = 0; i < a.size(); ++i) {
		if(a[i].size() == 0)
			std::cout << '_';
		for(size_t j = 0; j < a[i].size(); ++j) {
			std::cout << a[i][j] << del;
		}
		std::cout << '\n';
	}
}

template<typename T>
void print(const std::vector<T>& a, char del = ' ') {
	if(a.size() == 0)
		std::cout << '_';
	for(size_t i = 0; i < a.size(); ++i)
		std::cout << a[i] << del;
	std::cout << '\n';
}

void bprint(const std::vector<std::vector<int>>& a, const size_t format = 4, char del = ' ') {
	if(a.size() == 0)
		std::cout << "_\n";
	for(size_t i = 0; i < a.size(); ++i) {
		if(a[i].size() == 0)
			std::cout << '_';
		for(size_t j = 0; j < a[i].size(); ++j) {
			std::cout << decToBin(a[i][j], format) << del;
		}
		std::cout << '\n';
	}
}

void bprint(const std::vector<int>& a, const size_t format = 4, char del = ' ') {
	if(a.size() == 0)
		std::cout << '_';
	for(size_t i = 0; i < a.size(); ++i)
		std::cout << decToBin(a[i], format) << del;
	std::cout << '\n';
}

void print(const std::string& s) {
	std::cout << s << '\n';
}

template<typename T>
void print(const std::set<T>& s, char del = ' ') {
	for(const auto it: s)
		std::cout << it << del;
	std::cout << '\n';
}

template<typename T>
void print(const std::unordered_set<T>& s, char del = ' ') {
	for(const auto it: s)
		std::cout << it << del;
	std::cout << '\n';
}

template<typename T, typename L>
void print(const std::map<T, L>& m, char del = ' ') {
	for(const auto it: m) {
		std::cout << it.first << del << it.second << '\n';
	}
}

void bprint(const std::map<int, int>& m, const size_t format, char del = ' ') {
	for(const auto it: m) {
		std::cout << decToBin(it.first, format) << del << it.second << '\n';
	}
}

template<typename T, typename L>
void print(const std::unordered_map<T, L>& m, char del = ' ') {
	for(const auto it: m) {
		std::cout << it.first << del << it.second << '\n';
	}
}

template<typename T>
void print(const T& a) {
	std::cout << a << '\n';
}
