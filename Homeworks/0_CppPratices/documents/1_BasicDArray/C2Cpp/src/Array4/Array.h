
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

