#ifndef TESTPOLYNOMIAL_H
#define TESTPOLYNOMIAL_H

#include <iostream>
#include <ctime>
#include <vector>

template<class Polynomial>
class TestPolynomial {
public:
    bool testConstructor() const {
        std::cout << "Test Constructor" << std::endl;
        Polynomial p;

        p.coff(2) = 3;

        p.Print();

        Polynomial p1(p);

        p.Print();

        std::cout << std::endl;
        return true;
    }

    bool testOperationCorrectness() {
        std::cout << "test OperationCorrectness" << std::endl;
        Polynomial p0;
        p0.coff(2) = 3;
        p0.coff(0) = 5;
        p0.coff(3) = 4;
        Polynomial p1;
        (p0 * p1).Print();
        p1.coff(3) = -4;
        p1.coff(100) = 1;
        (p0 * p1).Print();
        (p0 + p1).Print();
        (p0 - p1).Print();
        Polynomial p2 = p1;
        p2.coff(10) = 11;
        (p0 * p2).Print();
        (p0 + p2).Print();
        (p0 - p2).Print();
        std::cout << std::endl;
        return true;
    }

    bool testConstructorFromGivenData(const std::vector<int>& deg, const std::vector<double>& cof) {
        clock_t t0 = clock();
        std::cout << "Test Constructor with Size: " << deg.size() << std::endl;
        Polynomial p(deg, cof);
        std::cout << "Test Constructor time: " << clock() - t0 << std::endl;
        std::cout << std::endl;
        return true;
    }

    bool testOperationFromGivenData(const std::vector<int>& deg0, const std::vector<double>& cof0,
        const std::vector<int>& deg1, const std::vector<double>& cof1, bool showOutput = false)
    {
        clock_t t0 = clock();
        Polynomial p0(deg0, cof0);
        Polynomial p1(deg1, cof1);
        if (showOutput) {
            (p0 * p1).Print();
            (p0 + p1).Print();
            (p0 - p1).Print();
        }
        else {
            p0 * p1;
            p0 + p1;
            p0 - p1;
        }
        std::cout << "Test Constructor time: " << clock() - t0 << std::endl;
        std::cout << std::endl;
        return true;
    }
};

#endif // TESTPOLYNOMIAL_H
