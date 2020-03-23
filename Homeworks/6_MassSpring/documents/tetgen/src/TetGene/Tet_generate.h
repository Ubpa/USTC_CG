#pragma once
#include "tetgen.h"
#include "Vec.h"

using trimesh::vec;

class CTet_generate
{
public:
	CTet_generate(void);
	~CTet_generate(void);
	tetgenio* poly_in(char* filename);
	tetgenio* tet_out(tetgenio* in);
	void tet_output(tetgenio  *a, char* filename);
	//vec get_normal(tetgenio* tet, int index);
};

