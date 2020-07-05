#pragma once

// interfaces of Dynamic Array class DArray
class DArray {
public:
	DArray(); // default constructor
	DArray(int nSize, double dValue = 0); // set an array with default values
	DArray(const DArray& arr); // copy constructor
	~DArray(); // deconstructor

	void Print() const; // print the elements of the array

	int GetSize() const; // get the size of the array
	void SetSize(int nSize); // set the size of the array

	const double& GetAt(int nIndex) const; // get an element at an index
	void SetAt(int nIndex, double dValue); // set the value of an element

	double& operator[](int nIndex); // overload operator '[]'
	const double& operator[](int nIndex) const; // overload operator '[]'

	void PushBack(double dValue); // add a new element at the end of the array
	void DeleteAt(int nIndex); // delete an element at some index
	void InsertAt(int nIndex, double dValue); // insert a new element at some index

	DArray& operator = (const DArray& arr); //overload operator '='

private:
	double* m_pData; // the pointer to the array memory
	int m_nSize; // the size of the array
	int m_nMax;

private:
	void Init(); // initilize the array
	void Free(); // free the array
	void Reserve(int nSize); // allocate enough memory
};
