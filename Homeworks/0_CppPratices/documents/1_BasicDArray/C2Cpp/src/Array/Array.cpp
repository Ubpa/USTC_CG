
// implementation of class Array
#include <iostream>
#include <assert.h>

#include "Array.h"

using namespace std;

#define TRUE	1
#define FALSE	0


// default constructor
Array::Array()
{
	Init();
}

// set an array with default values
Array::Array(int nSize, double dValue)
{
	Init();

	if (nSize > ARRAY_MAX_ELEMENTS)
	{
		cout << "Cannot set array size which exceeds the max space!" << endl;
		exit(0);
	}

	for (int i = 0; i < nSize; i++)
	{
		m_dData[i] = dValue;
	}

	m_nSize = nSize;
}

// initilize the array
void	Array::Init()
{
	m_nSize = 0;

	// clear the data to 0
	for (int i = 0; i < ARRAY_MAX_ELEMENTS; i++)
		m_dData[i] = 0.0f;
}

// deconstructor
Array::~Array()
{
	Free();
}

// free the array
void	Array::Free()
{
	// no need to free anything, do nothing
	// m_nSize=0;
}

// get the size of the array
int		Array::GetSize()
{
	assert(m_nSize <= ARRAY_MAX_ELEMENTS);

	return m_nSize;
}

// judge the validate of an index
inline	int	Array::InvalidateIndex(int nIndex)
{
	if (nIndex < 0 || nIndex > m_nSize)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// get an element at an index
double	Array::GetAt(int nIndex)
{
	if (InvalidateIndex(nIndex))
	{
		cout << "Index is invalide in GetAt()!" << endl;
		return FALSE;
	}
	else
	{
		return m_dData[nIndex];
	}
}

// set the value of an element 
int		Array::SetAt(int nIndex, double dValue)
{
	if (InvalidateIndex(nIndex))
	{
		cout << "Index is invalide in SetAt()!" << endl;
		return FALSE;
	}
	else
	{
		m_dData[nIndex] = dValue;
		return TRUE;
	}
}

// add a new element at the end of the array
int		Array::PushBack(double dValue)
{
	if (m_nSize == ARRAY_MAX_ELEMENTS)
	{
		cout << "Cannot push any element into the array!" << endl;
		return FALSE;
	}

	assert(m_nSize < ARRAY_MAX_ELEMENTS);

	m_dData[m_nSize] = dValue;
	m_nSize++;

	return TRUE;
}

// set the size of the array
int		Array::SetSize(int nSize)
{
	if (nSize > ARRAY_MAX_ELEMENTS)
	{
		cout << "Cannot set array size which exceeds the max space!" << endl;
		exit(0);
	}

	if (m_nSize == nSize)
	{
		// no need to do anything
		return TRUE;
	}

	m_nSize = nSize;

	return TRUE;
}

// delete an element at some index
int		Array::DeleteAt(int nIndex)
{
	if (InvalidateIndex(nIndex))
	{
		cout << "Index is invalide in RemoveAt()!" << endl;
		return FALSE;
	}
	else
	{
		for (int i = nIndex; i < m_nSize - 1; i++)
		{
			m_dData[i] = m_dData[i + 1];
		}
		m_nSize--;
		return TRUE;
	}
}

// insert a new element at some index
int		Array::InsertAt(int nIndex, double dValue)
{
	if (InvalidateIndex(nIndex))
	{
		cout << "Index is invalide in InsertAt()!" << endl;
		return FALSE;
	}
	else if (m_nSize == ARRAY_MAX_ELEMENTS)
	{
		cout << "The array space is full, cannot insert any element." << endl;
		return FALSE;
	}
	else
	{
		for (int i = m_nSize; i > nIndex; i--)
		{
			m_dData[i] = m_dData[i - 1];
		}
		m_dData[nIndex] = dValue;
		m_nSize++;
		return TRUE;
	}
}

// display the elements of the array
void	Array::Display()
{
	cout << " size= " << m_nSize << ":";
	for (int i = 0; i < GetSize(); i++)
	{
		cout << " " << GetAt(i);
	}
	cout << endl;
}
