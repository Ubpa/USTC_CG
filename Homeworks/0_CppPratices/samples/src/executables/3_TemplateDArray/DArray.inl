#include <iostream>
#include <assert.h>

// default constructor
template<typename T>
DArray<T>::DArray() {
	Init();
}

// set an array with default values
template<typename T>
// set an array with default values
DArray<T>::DArray(int nSize, const T& dValue)
	: m_pData(new T[nSize]), m_nSize(nSize), m_nMax(nSize)
{
	for (int i = 0; i < nSize; i++)
		m_pData[i] = dValue;
}

template<typename T>
DArray<T>::DArray(const DArray& arr)
	: m_pData(new T[arr.m_nSize]), m_nSize(arr.m_nSize), m_nMax(arr.m_nSize)
{
	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr.m_pData[i];
}

// deconstructor
template<typename T>
DArray<T>::~DArray()
{
	Free();
}

// display the elements of the array
template<typename T>
void DArray<T>::Print() const {
	std::cout << "size= " << m_nSize << ":";
	for (int i = 0; i < m_nSize; i++)
		std::cout << " " << GetAt(i);

	std::cout << std::endl;
}

// initilize the array
template<typename T>
void DArray<T>::Init() {
	m_pData = nullptr;
	m_nSize = 0;
	m_nMax = 0;
}

// free the array
template<typename T>
void DArray<T>::Free() {
	delete[] m_pData;
	m_pData = nullptr;

	m_nSize = 0;
	m_nMax = 0;
}

// get the size of the array
template<typename T>
int DArray<T>::GetSize() const {
	return m_nSize;
}

template<typename T>
void DArray<T>::Reserve(int nSize) {
	if (m_nMax >= nSize)
		return;

	while (m_nMax < nSize)
		m_nMax = (m_nMax == 0) ? 1 : 2 * m_nMax;

	T* pData = new T[m_nMax];

	for (int i = 0; i < m_nSize; i++)
		pData[i] = m_pData[i];

	delete[] m_pData;
	m_pData = pData;
}

// set the size of the array
template<typename T>
void DArray<T>::SetSize(int nSize) {
	if (m_nSize == nSize)
		return;

	Reserve(nSize);

	for (int i = m_nSize; i < nSize; i++)
		m_pData[i] = static_cast<T>(0);

	m_nSize = nSize;
}

// get an element at an index
template<typename T>
const T& DArray<T>::GetAt(int nIndex) const {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// set the value of an element 
template<typename T>
void DArray<T>::SetAt(int nIndex, const T& dValue) {
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

// overload operator '[]'
template<typename T>
const T& DArray<T>::operator[](int nIndex) const {
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// add a new element at the end of the array
template<typename T>
void DArray<T>::PushBack(const T& dValue) {
	Reserve(m_nSize + 1);

	m_pData[m_nSize] = dValue;
	m_nSize++;
}

// delete an element at some index
template<typename T>
void DArray<T>::DeleteAt(int nIndex) {
	assert(nIndex >= 0 && nIndex < m_nSize);

	for (int i = nIndex + 1; i < m_nSize; i++)
		m_pData[i - 1] = m_pData[i];

	m_nSize--;
}
// insert a new element at some index
template<typename T>
void DArray<T>::InsertAt(int nIndex, const T& dValue) {
	assert(nIndex >= 0 && nIndex <= m_nSize); // nIndex == m_nSize is legal

	Reserve(m_nSize + 1);

	for (int i = m_nSize; i > nIndex; i--)
		m_pData[i] = m_pData[i - 1];

	m_pData[nIndex] = dValue;

	m_nSize++;
}

// overload operator '='
template<typename T>
DArray<T>& DArray<T>::operator = (const DArray& arr) {
	if (this == &arr)
		return *this;

	delete[] m_pData;

	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nMax;

	m_pData = new T[m_nMax];

	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr[i];

	return *this;
}
