// implementation of class DArray
#include <iostream>

#include "DArray.h"

using namespace std;

// default constructor
DArray::DArray()
{
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue)
{
	//TODO
}

DArray::DArray(const DArray& arr)
{
	//TODO
}

// deconstructor
DArray::~DArray()
{
	Free();
}

// display the elements of the array
void DArray::Print()
{
	//TODO
}

// initilize the array
void DArray::Init()
{
	//TODO
}

// free the array
void DArray::Free()
{
	//TODO
}

// get the size of the array
int DArray::GetSize()
{
	//TODO
	return 0;
}

// set the size of the array
void DArray::SetSize(int nSize)
{
	//TODO
}

// get an element at an index
double DArray::GetAt(int nIndex)
{
	//TODO
	return 0.;
}

// set the value of an element 
void DArray::SetAt(int nIndex, double dValue)
{
	//TODO
}

// overload operator '[]'
double DArray::operator[](int nIndex) const
{
	//TODO
	return 0.;
}

// add a new element at the end of the array
void DArray::PushBack(double dValue)
{
	//TODO
}

// delete an element at some index
void DArray::DeleteAt(int nIndex)
{
	//TODO
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue)
{
	//TODO
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr)
{
	//TODO
	return *this;
}
