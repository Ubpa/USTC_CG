
#include <stdio.h>

#include "Array.h"

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
