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
	m_pData = new double [nSize];
	for(int i=0; i<nSize; i++)
	{
		m_pData[i] = dValue;
	}

	m_nSize = nSize;
	m_nMax = nSize;
}

DArray::DArray(const DArray& arr)
{
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;
	m_pData = new double [m_nSize];

	for	(int i=0; i<m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}
}

// deconstructor
DArray::~DArray()
{
	Free();
}

// display the elements of the array
void DArray::Print()
{
	cout<<"size= "<<m_nSize<<":";
	for(int i=0; i<m_nSize; i++)
	{
		cout<<" "<<GetAt(i);
	}
	cout<<endl;
}

// initilize the array
void DArray::Init()
{
	m_nSize = 0;
	m_nMax = 0;
	m_pData = NULL;
}

// free the array
void DArray::Free()
{
	delete [] m_pData;
	m_pData = NULL;

	m_nSize = 0;
	m_nMax = 0;
}

// judge the validate of an index
inline int DArray::InvalidateIndex(int nIndex)
{
	if(nIndex < 0 || nIndex > m_nSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// get the size of the array
int DArray::GetSize()
{
	return m_nSize;
}

// set the size of the array
bool DArray::SetSize(int nSize)
{
	if( m_pData == NULL )
	{
		m_pData = new double [nSize];
		if( m_pData == NULL )
		{
			cout<< "Can not alloc memory in SetSize()!" << endl;
			return false;
		}

		for(int i=0; i<nSize; i++)
		{
			m_pData[i] = 0;
		}

		m_nSize = nSize;
		m_nMax = nSize;
	}
	else if( m_nSize==nSize )
	{
		return true;
	}
	else
	{
		double *pTemp=NULL;
		pTemp = new double [nSize];
		if( pTemp == NULL )
		{
			cout<< "Can not alloc memory in SetSize()!" << endl;
			return false;
		}
		for(int i=0; i<nSize; i++)
		{
			pTemp[i] = 0;
		}

		int nBound = 0;
		if( m_nSize<nSize )
		{
			nBound = m_nSize;
		}
		else
		{
			nBound = nSize;
		}
		for( int i=0; i<nBound; i++ )
		{
			pTemp[i] = m_pData[i];
		}

		delete [] m_pData;
		m_pData = pTemp;
		m_nSize = nSize;
		m_nMax = nSize;
	}

	return true;
}

// get an element at an index
double DArray::GetAt(int nIndex)
{
	if( InvalidateIndex(nIndex) )
	{
		cout<<"Index is invalide in GetAt()!"<<endl;
		return false;
	}
	else
	{
		return m_pData[nIndex];
	}
}

// set the value of an element 
bool	DArray::SetAt(int nIndex, double dValue)
{
	if( InvalidateIndex(nIndex) )
	{
		cout<<"Index is invalide in SetAt()!"<<endl;
		return false;
	}
	else
	{
		m_pData[nIndex]=dValue;
		return true;
	}
}

// overload operator '[]'
double DArray::operator[](int nIndex) const
{
	return m_pData[nIndex];
}

// add a new element at the end of the array
bool DArray::PushBack(double dValue)
{
	if( m_nSize < m_nMax )
	{
		m_pData[m_nSize] = dValue;
		m_nSize++;
	}
	else
	{
		if( m_nMax == 0 )
		{
			m_nMax = 1;
		}
		else
		{
			m_nMax *= 2;
		}

		double *pTemp=NULL;
		pTemp = new double [m_nMax];
		if( pTemp == NULL )
		{
			cout<< "Can not alloc memory in PushBack()!" << endl;
			return false;
		}

		for( int i=0; i<m_nSize; i++ )
		{
			pTemp[i] = m_pData[i];
		}
		pTemp[m_nSize] = dValue;

		delete [] m_pData;
		m_pData = pTemp;
		m_nSize++;
	}

	return true;
}

// delete an element at some index
bool DArray::DeleteAt(int nIndex)
{
	if( InvalidateIndex(nIndex) )
	{
		cout<<"Index is invalide in DeleteAt()!"<<endl;
		return false;
	}
	else
	{
		double *pTemp=NULL;
		pTemp = new double [m_nSize-1];
		if( pTemp == NULL )
		{
			cout<< "Can not alloc memory in DeleteAt()!" << endl;
			return false;
		}

		for( int i=0; i<nIndex; i++ )
		{
			pTemp[i] = m_pData[i];
		}

		for( int i=nIndex; i<m_nSize-1; i++ )
		{
			pTemp[i] = m_pData[i+1];
		}

		delete [] m_pData;
		m_pData = pTemp;
		m_nSize--;
		m_nMax = m_nSize;

		return true;
	}
}

// insert a new element at some index
bool DArray::InsertAt(int nIndex, double dValue)
{
	if( InvalidateIndex(nIndex) )
	{
		cout<<"Index is invalide in InsertAt()!"<<endl;
		return false;
	}
	else
	{
		if(m_nSize < m_nMax)
		{
			for( int i=m_nSize; i>nIndex; i-- )
			{
				m_pData[i] = m_pData[i-1];
			}
			m_pData[nIndex]=dValue;
			m_nSize++;
		}
		else
		{
			//assert(m_nSize == m_nMax);

			double *pTemp=NULL;
			if(m_nMax == 0 )
			{
				m_nMax = 1;
			}
			else
			{
				m_nMax *= 2;
			}
			pTemp = new double [m_nMax];
			if( pTemp == NULL )
			{
				cout<< "Can not alloc memory in InsertAt()!" << endl;
				return false;
			}

			for( int i=0; i<nIndex; i++ )
			{
				pTemp[i] = m_pData[i];
			}
			pTemp[nIndex]=dValue;
			for( int i=nIndex+1; i<m_nSize+1; i++ )
			{
				pTemp[i] = m_pData[i-1];
			}

			delete [] m_pData;
			m_pData = pTemp;
			m_nSize++;
		}

		return true;
	}
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr)
{
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;
	m_pData = new double [m_nSize];

	for	(int i=0; i<m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}

	return *this;
}
