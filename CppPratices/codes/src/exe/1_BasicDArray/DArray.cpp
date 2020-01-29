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
bool DArray::SetSize(int nSize)
{
	//TODO
	return false;
}

// get an element at an index
double DArray::GetAt(int nIndex)
{
	//TODO
	return 0.;
}

// set the value of an element 
bool DArray::SetAt(int nIndex, double dValue)
{
	//TODO
	return false;
}

// overload operator '[]'
double DArray::operator[](int nIndex) const
{
	//TODO
	return 0.;
}

// add a new element at the end of the array
bool DArray::PushBack(double dValue)
{
	//TODO
	return false;
}

// delete an element at some index
bool DArray::DeleteAt(int nIndex)
{
	//TODO
	return false;
}

// insert a new element at some index
bool DArray::InsertAt(int nIndex, double dValue)
{
	//TODO
	return false;
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr)
{
	//TODO
	return *this;
}
