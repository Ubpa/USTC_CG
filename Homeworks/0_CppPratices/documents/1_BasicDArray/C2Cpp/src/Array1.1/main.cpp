#include <cstdio>
#include <cstdlib>

typedef struct
{
	int n; // the size of the array
	double *pData;  // the data of the array
}DArray;


//double pData[100];

int		InitArray( DArray *a );
int		SetArraySize(DArray *a,  int size );
int		FreeArray(DArray *a );
int		SetValue( DArray *a, int k, double value );
int		PrintArray(DArray *a );

void main()
{
	printf("set pa:\n" );
	DArray pa;

	InitArray( &pa );
	
	SetArraySize( &pa, 3 );
	SetValue( &pa, 0, 1.0 );
	SetValue( &pa, 1, 2.0 );
	SetValue( &pa, 2, 3.0 );

	PrintArray(&pa);

	FreeArray(&pa);

	printf("set pb:\n" );
	DArray pb;
	InitArray( &pb );
	SetArraySize( &pb, 4 );
	SetValue( &pb, 0, 1.0 );
	SetValue( &pb, 1, 2.0 );
	SetValue( &pb, 2, 3.0 );
	SetValue( &pb, 3, 4.0 );
	PrintArray(&pb);
	FreeArray(&pb);
}

int		InitArray( DArray *a )
{
	a->n = 0;
	a->pData = NULL;

	return 1;
}

int		SetArraySize( DArray *a, int size )
{
	a->n = size;
	a->pData = (double*)malloc( size * sizeof(double) );
	if( a->pData == NULL )
	{
		printf("no enough memory!\n");
		return 0;
	}

	return 1;
}

int		FreeArray(DArray *a)
{
	if( a->pData != NULL )
	{
		free( a->pData );
		a->pData = NULL;
	}

	return 1;
}

int		SetValue( DArray *a, int k, double value )
{
	if( a->pData == NULL )
		return 0;

	if( k<0 || k>=a->n ) 
		return 0;

	a->pData[ k ] = value;
	return 1;
}


int	PrintArray(DArray *a)
{
	if(a->n==0) 
		return 0;

	if( a->pData == NULL )
		return 0;

	for( int i=0; i<a->n; i++) 
	{
		printf("%lf \n", a->pData[i] );
	}

	return 1;
}
