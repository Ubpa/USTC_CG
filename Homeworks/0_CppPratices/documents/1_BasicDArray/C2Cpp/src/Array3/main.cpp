#include <cstdio>
#include <cstdlib>

class DArray
{
private:
	int n; // the size of the array
	double *pData;  // the data of the array

public:
	DArray();
	~DArray();
	int		InitArray( );
	int		SetArraySize( int size );
	int		FreeArray( );
	int		SetValue( int k, double value );
	int		PrintArray(  );
};


void main()
{
	DArray pa;

	//pa.InitArray( );
	
	pa.SetArraySize(  3 );
	pa.SetValue(  0, 1.0 );
	pa.SetValue(  1, 2.0 );
	pa.SetValue( 2, 3.0 );

	pa.PrintArray( );

	//pa.FreeArray( );
}

DArray::DArray()
{
	InitArray();
}


DArray::~DArray()
{
	FreeArray();
}



int		DArray::InitArray( )
{
	n = 0;
	pData = NULL;

	return 1;
}

int		DArray::SetArraySize(  int size )
{
	n = size;
	pData = (double*)malloc( size * sizeof(double) );
	if( pData == NULL )
	{
		printf("no enough memory!\n");
		return 0;
	}

	return 1;
}

int		DArray::FreeArray( )
{
	if( pData != NULL )
	{
		free( pData );
		pData = NULL;
	}

	return 1;
}

int		DArray::SetValue( int k, double value )
{
	if( pData == NULL )
		return 0;

	if( k<0 || k>=n ) 
		return 0;

	pData[ k ] = value;
	return 1;
}


int	DArray::PrintArray()
{
	if(n==0) 
		return 0;

	if( pData == NULL )
		return 0;

	for( int i=0; i<n; i++) 
	{
		printf("%lf \n", pData[i] );
	}

	return 1;
}
