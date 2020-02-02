// implementation of class DArray
#include <iostream>

#include "DArray.h"

using namespace std;

// default constructor
DArray::DArray() {
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue)
	: m_pData(new double[nSize]), m_nSize(nSize)
{
	if (!m_pData) {
		cout << "Can not alloc memory in DArray::DArray()!" << endl;
		m_nSize = 0;
		return;
	}

	for (int i = 0; i < nSize; i++)
		m_pData[i] = dValue;
}

DArray::DArray(const DArray& arr)
	: m_pData(new double[arr.m_nSize]), m_nSize(arr.m_nSize)
{
	if (!m_pData) {
		cout << "Can not alloc memory in DArray::DArray()!" << endl;
		m_nSize = 0;
		return;
	}

	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr.m_pData[i];
}

// deconstructor
DArray::~DArray() {
	Free();
}

// display the elements of the array
void DArray::Print() {
	cout << "size= " << m_nSize << ":";
	for (int i = 0; i < m_nSize; i++)
		cout << " " << GetAt(i);
	cout << endl;
}

// initilize the array
void DArray::Init() {
	m_pData = nullptr;
	m_nSize = 0;
}

// free the array
void DArray::Free() {
	delete[] m_pData;
	m_pData = nullptr;

	m_nSize = 0;
}

// judge the validate of an index
bool DArray::InvalidateIndex(int nIndex) const {
	return nIndex < 0 || nIndex >= m_nSize;
}

// get the size of the array
int DArray::GetSize() {
	return m_nSize;
}

// set the size of the array
bool DArray::SetSize(int nSize) {
	if (m_nSize == nSize)
		return true;

	double* pData = new double[nSize];
	if (!pData) {
		cout << "Can not alloc memory in SetSize()!" << endl;
		return false;
	}

	int copyNum = nSize < m_nSize ? nSize : m_nSize;
	for (int i = 0; i < copyNum; i++)
		pData[i] = m_pData[i];
	for (int i = copyNum; i < nSize; i++)
		pData[i] = 0.;

	delete[] m_pData;
	m_pData = pData;
	m_nSize = nSize;

	return true;
}

// get an element at an index
double DArray::GetAt(int nIndex) {
	if (InvalidateIndex(nIndex)) {
		cout << "Index is invalide in GetAt()!" << endl;
		return 0.;
	}

	return m_pData[nIndex];
}

// set the value of an element 
bool DArray::SetAt(int nIndex, double dValue) {
	if (InvalidateIndex(nIndex)) {
		cout << "Index is invalide in SetAt()!" << endl;
		return false;
	}

	m_pData[nIndex] = dValue;
	return true;
}

// overload operator '[]'
double DArray::operator[](int nIndex) const {
	return m_pData[nIndex];
}

// add a new element at the end of the array
bool DArray::PushBack(double dValue) {
	double* pTemp = new double[static_cast<size_t>(m_nSize) + 1];
	if (pTemp == nullptr) {
		cout << "Can not alloc memory in PushBack()!" << endl;
		return false;
	}

	for (int i = 0; i < m_nSize; i++)
		pTemp[i] = m_pData[i];

	pTemp[m_nSize] = dValue;

	delete[] m_pData;
	m_pData = pTemp;
	m_nSize++;

	return true;
}

// delete an element at some index
bool DArray::DeleteAt(int nIndex) {
	if (InvalidateIndex(nIndex)) {
		cout << "Index is invalide in DeleteAt()!" << endl;
		return false;
	}

	double* pTemp = new double[static_cast<size_t>(m_nSize) - 1];
	if (!pTemp) {
		cout << "Can not alloc memory in DeleteAt()!" << endl;
		return false;
	}

	for (int i = 0; i < nIndex; i++)
		pTemp[i] = m_pData[i];

	for (int i = nIndex; i < m_nSize - 1; i++)
		pTemp[i] = m_pData[i + 1];

	delete[] m_pData;
	m_pData = pTemp;
	m_nSize--;

	return true;
}

// insert a new element at some index
bool DArray::InsertAt(int nIndex, double dValue) {
	if (InvalidateIndex(nIndex))
	{
		cout << "Index is invalide in InsertAt()!" << endl;
		return false;
	}
	
	double* pTemp = new double[m_nSize + 1];
	if (!pTemp) {
		cout << "Can not alloc memory in InsertAt()!" << endl;
		return false;
	}

	for (int i = 0; i < nIndex; i++)
		pTemp[i] = m_pData[i];

	pTemp[nIndex] = dValue;

	for (int i = nIndex + 1; i < m_nSize + 1; i++)
		pTemp[i] = m_pData[i - 1];

	delete[] m_pData;
	m_pData = pTemp;
	m_nSize++;

	return true;
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr) {
	delete[] m_pData;

	m_nSize = arr.m_nSize;
	m_pData = new double[m_nSize];

	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr[i];

	return *this;
}
