
#include <cstdio>

#include "Array.h"

void main()
{
	// test double array
	DArray<double> pa;

	//pa.InitArray( );
	
	pa.SetArraySize(  3 );
	pa.SetValue(  0, 1.0 );
	pa.SetValue(  1, 2.0 );
	pa.SetValue(  2, 3.0 );

	pa.PrintArray( );

	//pa.FreeArray( );



	// test int array
	DArray<int> pb;

	//pb.InitArray( );
	
	pb.SetArraySize(  3 );
	pb.SetValue(  0, 10 );
	pb.SetValue(  1, 11 );
	pb.SetValue(  2, 12 );

	pb.PrintArray( );

	//pb.FreeArray( );
}
