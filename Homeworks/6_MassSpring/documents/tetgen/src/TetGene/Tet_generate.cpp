#include "Tet_generate.h"
#include <fstream>


using namespace std;
CTet_generate::CTet_generate(void)
{
}


CTet_generate::~CTet_generate(void)
{
}

tetgenio* CTet_generate::poly_in(char* filename)
{
	string extension;
	string f;
	int size = 0;
	while (filename[size] != '\0')
	{
		size++;
	}
	while (filename[size] != '.' && size >= 0)
	{
		size--;
	}

	if (size < 0)
	{
		cout << "wrong file name" << endl;
		return nullptr;
	}
	int fnsize = size;
	size++;

	while (filename[size] != '\0')
	{
		extension.push_back(filename[size]);
		size++;
	}

	for (int i = 0; i < fnsize; i++)
	{
		f.push_back(filename[i]);
	}

	char *fn = new char[f.size() + 1];
	strcpy_s(fn, f.size() + 1,f.c_str());
	tetgenio* in = new tetgenio;
	//cout << extension << endl << fn << endl;
	if (extension == "stl")
	{
		in->load_stl(fn);
	}
	if (extension == "poly")
	{
		in->load_poly(fn);
	}
	//in->load_poly(filename);
	delete[] fn;
	return in;
	
}

tetgenio* CTet_generate::tet_out(tetgenio* in)
{
	tetgenio* out = new tetgenio;
	tetgenbehavior* command = new tetgenbehavior;
	char new_command[] = "pqnnfea0.1z";   //ÃüÁîĞĞ²ÎÊı
	//char new_command[] = "pqnnfe";
	command->parse_commandline(new_command);
	tetrahedralize(command, in, out);
	return out;
}

void CTet_generate::tet_output(tetgenio  *a, char* filename)
{
	ofstream file(filename);
    file <<"tet"<<' ' << a->numberofpoints <<' ' << a->numberoftetrahedra << endl;
	for (int i = 0; i < a->numberofpoints; i++)
	{
		file << a->pointlist[3 * i] << ' ' << a->pointlist[3 * i + 1] << ' ' << a->pointlist[3 * i + 2] << endl;
	}


	for (int i = 0; i < a->numberoftetrahedra; i++)
	{
		file << a->tetrahedronlist[4 * i] << ' ' << a->tetrahedronlist[4 * i + 1] << ' ' << a->tetrahedronlist[4 * i + 2] << ' ' << a->tetrahedronlist[4 * i + 3] << endl;
	}
	file.close();
}
