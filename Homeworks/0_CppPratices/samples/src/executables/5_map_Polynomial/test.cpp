
#include "PolynomialList.h"
#include "PolynomialMap.h"
#include <string>

#include "TestPolynomial.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void generateTestCase(int size, std::vector<int>& deg, std::vector<double>& cof)
{
    deg.clear(); cof.clear();
    for(int i=0; i < size; i++)
    {
        int c = rand() % 10000;
        double val = (double)(rand() % 100);
        deg.push_back(c);
        cof.push_back(val);
    }
}

int main()
{
    srand(unsigned(time(0)));
    TestPolynomial<PolynomialList> testList;
    TestPolynomial<PolynomialMap> testMap;
    //    testList.testConstructor();
    //    testMap.testConstructor();

    //    testList.testOperationCorrectness();
    //    testMap.testOperationCorrectness();

    //    std::vector<int> deg;
    //    std::vector<double> cof;

    //    generateTestCase(5,deg,cof);
    //    testList.testConstructorFromGivenData(deg,cof);
    //    testMap.testConstructorFromGivenData(deg,cof);

    //    generateTestCase(500,deg,cof);
    //    testList.testConstructorFromGivenData(deg,cof);
    //    testMap.testConstructorFromGivenData(deg,cof);


    //    generateTestCase(10000,deg,cof);
    //    testList.testConstructorFromGivenData(deg,cof);
    //    testMap.testConstructorFromGivenData(deg,cof);

    //    generateTestCase(100000,deg,cof);
    //    testList.testConstructorFromGivenData(deg,cof);
    //    testMap.testConstructorFromGivenData(deg,cof);

    std::vector<int> deg0, deg1;
    std::vector<double> cof0, cof1;
    generateTestCase(5, deg0, cof0);
    generateTestCase(5, deg1, cof1);
    testList.testOperationFromGivenData(deg0, cof0, deg0, cof1, true);
    testMap.testOperationFromGivenData(deg0, cof0, deg0, cof1, true);


    generateTestCase(100, deg0, cof0);
    generateTestCase(100, deg1, cof1);
    std::cout << "Test List:" << std::endl;
    testList.testOperationFromGivenData(deg0, cof0, deg1, cof1);
    std::cout << "Test Map: " << std::endl;
    testMap.testOperationFromGivenData(deg0, cof0, deg1, cof1);

    generateTestCase(100, deg0, cof0);
    generateTestCase(1000, deg1, cof1);
    std::cout << "Test List:" << std::endl;
    testList.testOperationFromGivenData(deg0, cof0, deg1, cof1);
    std::cout << "Test Map: " << std::endl;
    testMap.testOperationFromGivenData(deg0, cof0, deg1, cof1);

    return 0;
}
