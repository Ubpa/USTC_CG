
#if !defined _DYNAMICARRAY_H_
#define _DYNAMICARRAY_H_

#include <iostream>

using namespace std;

// interfaces of Dynamic Array class DArray
template<class T>
class DArray
{
private:
	T		*m_pData;	// the pointer to the array memory
	int		m_nSize;	// the size of the array
	int		m_nMax;		

private:
	void	Init();		// initilize the array
	void	Free();		// free the array
	inline	int	InvalidateIndex(int nIndex);	// judge the validate of an index

public:
	DArray();			// default constructor
	DArray(int nSize, T dValue = 0);	// set an array with default values
	DArray(const DArray& arr);			// copy constructor
	~DArray();			// deconstructor

	void	Print();			// print the elements of the array

	int		GetSize();			// get the size of the array
	bool	SetSize(int nSize);			// set the size of the array

	T		GetAt(int nIndex);	// get an element at an index
	bool	SetAt(int nIndex, T dValue);	// set the value of an element

	T operator[](int nIndex) const;	// overload operator '[]'

	bool	PushBack(T dValue);	// add a new element at the end of the array
	bool	DeleteAt(int nIndex);		// delete an element at some index
	bool	InsertAt(int nIndex, T dValue);	// insert a new element at some index
	
	DArray &DArray::operator = (const DArray& arr);		//overload operator '='
};

// default constructor
template<class T>
DArray<T>::DArray()
{
	Init();
}

// set an array with default values
template<class T>
DArray<T>::DArray(int nSize, T dValue)
{
	m_pData = new T [nSize];
	for(int i=0; i<nSize; i++)
	{
		m_pData[i] = dValue;
	}

	m_nSize = nSize;
	m_nMax = nSize;
}

template<class T>
DArray<T>::DArray(const DArray& arr)
{
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;
	m_pData = new T [m_nSize];

	for	(int i=0; i<m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}
}

// deconstructor
template<class T>
DArray<T>::~DArray()
{
	Free();
}

// display the elements of the array
template<class T>
void DArray<T>::Print()
{
	cout<<"size= "<<m_nSize<<":";
	for(int i=0; i<m_nSize; i++)
	{
		cout<<" "<<GetAt(i);
	}
	cout<<endl;
}

// initilize the array
template<class T>
void DArray<T>::Init()
{
	m_nSize = 0;
	m_nMax = 0;
	m_pData = NULL;
}

// free the array
template<class T>
void DArray<T>::Free()
{
	delete [] m_pData;
	m_pData = NULL;

	m_nSize = 0;
	m_nMax = 0;
}

// judge the validate of an index
template<class T>
inline int DArray<T>::InvalidateIndex(int nIndex)
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
template<class T>
int DArray<T>::GetSize()
{
	return m_nSize;
}

// set the size of the array
template<class T>
bool DArray<T>::SetSize(int nSize)
{
	if( m_pData == NULL )
	{
		m_pData = new T [nSize];
		if( m_pData == NULL )
		{
			cout<< "Can not alloc memory in SetSize()!" << endl;
			return false;
		}

		for(int i=0; i<nSize; i++)
		{
			m_pData[i] = (T)0;
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
		T *pTemp=NULL;
		pTemp = new T [nSize];
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
template<class T>
T DArray<T>::GetAt(int nIndex)
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
template<class T>
bool DArray<T>::SetAt(int nIndex, T dValue)
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
template<class T>
T DArray<T>::operator[](int nIndex) const
{
	return m_pData[nIndex];
}

// add a new element at the end of the array
template<class T>
bool DArray<T>::PushBack(T dValue)
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

		T *pTemp=NULL;
		pTemp = new T [m_nMax];
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
template<class T>
bool DArray<T>::DeleteAt(int nIndex)
{
	if( InvalidateIndex(nIndex) )
	{
		cout<<"Index is invalide in DeleteAt()!"<<endl;
		return false;
	}
	else
	{
		T *pTemp=NULL;
		pTemp = new T [m_nSize-1];
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
template<class T>
bool DArray<T>::InsertAt(int nIndex, T dValue)
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
			T *pTemp=NULL;
			if(m_nMax == 0 )
			{
				m_nMax = 1;
			}
			else
			{
				m_nMax *= 2;
			}
			pTemp = new T [m_nMax];
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
template<class T>
DArray<T>& DArray<T>::operator = (const DArray& arr)
{
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;
	m_pData = new T [m_nSize];

	for	(int i=0; i<m_nSize; i++)
	{
		m_pData[i] = arr[i];
	}

	return *this;
}

#endif // _DYNAMICARRAY_H_