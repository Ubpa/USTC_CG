#include "PolynomialList.h"

using namespace std;

PolynomialList::PolynomialList(const PolynomialList& other) {
    // TODO
}

PolynomialList::PolynomialList(const string& file) {
    // TODO
}

PolynomialList::PolynomialList(const double* cof, const int* deg, int n) {
    // TODO
}

PolynomialList::PolynomialList(const vector<int>& deg, const vector<double>& cof) {
    // TODO
}

double PolynomialList::coff(int i) const {
    // TODO
    return 0.; // you should return a correct value
}

double& PolynomialList::coff(int i) {
    // TODO
    static double ERROR; // you should delete this line
    return ERROR; // you should return a correct value
}

void PolynomialList::compress() {
    // TODO
}

PolynomialList PolynomialList::operator+(const PolynomialList& right) const {
    // TODO
    return {}; // you should return a correct value
}

PolynomialList PolynomialList::operator-(const PolynomialList& right) const {
    // TODO
    return {}; // you should return a correct value
}

PolynomialList PolynomialList::operator*(const PolynomialList& right) const {
    // TODO
    return {}; // you should return a correct value
}

PolynomialList& PolynomialList::operator=(const PolynomialList& right) {
    // TODO
    return *this;
}

void PolynomialList::Print() const {
    // TODO
}

bool PolynomialList::ReadFromFile(const string& file) {
    // TODO
    return false; // you should return a correct value
}

PolynomialList::Term& PolynomialList::AddOneTerm(const Term& term) {
    // TODO
    static Term ERROR; // you should delete this line
    return ERROR; // you should return a correct value
}
