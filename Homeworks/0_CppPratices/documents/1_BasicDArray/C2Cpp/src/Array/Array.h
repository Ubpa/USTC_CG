#if !defined _DYNAMICARRAY_H_
#define _DYNAMICARRAY_H_

#define ARRAY_MAX_ELEMENTS  15

// interfaces of Array class
class Array
{
private:
	double	m_dData[ARRAY_MAX_ELEMENTS];	// the pointer to the array memory
	int		m_nSize;	// the size of the array

private:
	void	Init();		// initilize the array
	void	Free();		// free the array
	inline	int	InvalidateIndex(int nIndex);	// judge the validate of an index

public:
	Array();			// default constructor
	Array(int nSize, double dValue = 0);	// set an array with default values
	~Array();			// deconstructor


	void	Display();			// display the elements of the array
	int		GetSize();			// get the size of the array
	double	GetAt(int nIndex);	// get an element at an index
	int		SetAt(int nIndex, double dValue);	// set the value of an element

	int		PushBack(double dValue);	// add a new element at the end of the array
	int		DeleteAt(int nIndex);		// delete an element at some index
	int		InsertAt(int nIndex, double dValue);	// insert a new element at some index
	int		SetSize(int nSize);			// set the size of the array
};

#endif // _DYNAMICARRAY_H_