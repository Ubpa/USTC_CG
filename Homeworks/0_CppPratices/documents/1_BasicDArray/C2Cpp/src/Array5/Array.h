#include <stdio.h>
#include <stdlib.h>

#include <typeinfo.h>
#include <string.h>

template <class DataType>
class DArray
{
private:
	int n; // the size of the array
	DataType *pData;  // the data of the array

public:
	DArray();
	~DArray();
	int		InitArray( );
	int		SetArraySize( int size );
	int		FreeArray( );
	int		SetValue( int k, DataType value );
	int		PrintArray(  );
};

template<class DataType>
DArray<DataType>::DArray()
{
	InitArray( );
}

template<class DataType>
DArray<DataType>::~DArray()
{
	FreeArray( );
}

template<class DataType>
int		DArray<DataType>::InitArray( )
{
	n = 0;
	pData = NULL;

	return 1;
}

template<class DataType>
int		DArray<DataType>::SetArraySize(  int size )
{
	n = size;
	pData = (DataType*)malloc( size * sizeof(DataType) );
	if( pData == NULL )
	{
		printf("no enough memory!\n");
		return 0;
	}

	return 1;
}

template<class DataType>
int		DArray<DataType>::FreeArray( )
{
	if( pData != NULL )
	{
		free( pData );
		pData = NULL;
	}

	return 1;
}

template<class DataType>
int		DArray<DataType>::SetValue( int k, DataType value )
{
	if( pData == NULL )
		return 0;

	if( k<0 || k>=n ) 
		return 0;

	pData[ k ] = value;
	return 1;
}


template<class DataType>
int	DArray<DataType>::PrintArray()
{
	if(n==0) 
		return 0;

	if( pData == NULL )
		return 0;

	for( int i=0; i<n; i++) 
	{
		char const *typen = typeid( DataType ).name();
		if( !strcmp( typen, "double" ) )
			printf(" type: %lf ", pData[i] );
		else if( !strcmp( typen, "int" ) )
			printf(" type: %d ", pData[i] );

		printf("\n");
	}

	return 1;
}
