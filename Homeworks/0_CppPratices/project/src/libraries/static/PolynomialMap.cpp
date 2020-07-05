#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;

PolynomialMap::PolynomialMap(const PolynomialMap& other) {
    // TODO
}

PolynomialMap::PolynomialMap(const string& file) {
    ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double* cof, const int* deg, int n) {
	// TODO
}

PolynomialMap::PolynomialMap(const vector<int>& deg, const vector<double>& cof) {
	assert(deg.size() == cof.size());
	// TODO
}

double PolynomialMap::coff(int i) const {
	// TODO
	return 0.f; // you should return a correct value
}

double& PolynomialMap::coff(int i) {
	// TODO
	static double ERROR; // you should delete this line
	return ERROR; // you should return a correct value
}

void PolynomialMap::compress() {
	// TODO
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap& right) const {
	// TODO
	return {}; // you should return a correct value
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap& right) const {
	// TODO
	return {}; // you should return a correct value
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap& right) const {
	// TODO
	return {}; // you should return a correct value
}

PolynomialMap& PolynomialMap::operator=(const PolynomialMap& right) {
	// TODO
	return *this;
}

void PolynomialMap::Print() const {
	// TODO
}

bool PolynomialMap::ReadFromFile(const string& file) {
    m_Polynomial.clear();
	// TODO
	return false; // you should return a correct value
}
