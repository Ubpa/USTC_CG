#include <stdio.h>
#include <stdlib.h>

int n; // the size of the array
double *pData;  // the data of the array

//double pData[100];

int SetArraySize( int size );
int FreeArray();
int SetValue( int k, double value );
int PrintArray();

void main()
{
	SetArraySize( 3 );
	SetValue( 0, 1.0 );
	SetValue( 1, 2.0 );
	SetValue( 2, 3.0 );

	PrintArray();

	FreeArray();
}

int SetArraySize( int size )
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

int FreeArray()
{
	if( pData != NULL )
	{
		free( pData );
		pData = NULL;
	}

	return 1;
}

int SetValue( int k, double value )
{
	if( pData == NULL )
		return 0;

	if( k<0 || k>=n ) 
		return 0;

	pData[ k ] = value;
	return 1;
}


int	PrintArray()
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
