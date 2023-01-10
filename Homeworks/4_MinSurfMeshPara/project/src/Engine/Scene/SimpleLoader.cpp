#include "SimpleLoader.h"

#include <Basic/File.h>

#include <UGM/UGM>

#include <Engine/Scene/SObj.h>

#include <Engine/Scene/CmptMaterial.h>
#include <Engine/Scene/CmptTransform.h>
#include <Engine/Scene/CmptGeometry.h>
#include <Engine/Scene/CmptSimu/CmptSimulate.h>
#include <Engine/Scene/CmptSimu/MassSpring.h>

#include <Engine/Primitive/TriMesh.h>
#include <Engine/Primitive/TetMesh.h>
#include <Engine/Material/BSDF_Frostbite.h>

#include <Basic/StrAPI.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace Ubpa;
using namespace std;

Ptr<SObj> SimpleLoader::LoadObj(const string& path) {
	File objfile(path, File::Mode::READ);
	if (!objfile.IsValid()) {
		cout << "ERROR::SimpleObjLoader::Load:" << endl
			<< "\t" << "open file (" << path << ") fail" << endl;
	}

	auto data = objfile.ReadAllLines();
	vector<pointf3> positions;
	vector<unsigned> indices;
	for (auto line : data) {
		if (line.empty() || line == "\n")
			continue;
		stringstream ss(line);
		char id;
		ss >> id;
		switch (id)
		{
		case 'v': {
			pointf3 pos;
			ss >> pos;
			positions.push_back(pos);
		}
			break;
		case 'f': {
			valu3 tri;
			ss >> tri;
			for (auto idx : tri)
				indices.push_back(idx - 1);
		}
			break;
		default:
			break;
		}
	}

	// offset, scale
	bboxf3 box;
	for (const auto& pos : positions)
		box.combine_with(pos);
	auto center = box.center();
	float scale = sqrt(3.f / box.diagonal().norm2());
	for (auto& pos : positions)
		pos = (scale * (pos - center)).cast_to<pointf3>();

	auto tmp = StrAPI::Replace(path, "\\", "/");
	auto sobj = SObj::New(nullptr, StrAPI::Split(tmp, '/').back());
	CmptTransform::New(sobj);
	auto triMesh = TriMesh::New(indices, positions);
	CmptGeometry::New(sobj, triMesh);
	CmptMaterial::New(sobj, BSDF_Frostbite::New());

	// add CmptSimulate
	auto spring = MassSpring::New(triMesh);
	spring->InitSimu();

	//auto springmesh = SObj::New(sobj, "SpringSimulate");
	CmptSimulate::New(sobj, spring);
	//CmptTransform::New(springmesh);

	return sobj;
}

Ptr<SObj> SimpleLoader::LoadTet(const std::string& dir)
{
	vector<pointf3> poses;
	vector<unsigned> tetindices;

	std::ifstream tet;
	tet.open(dir);
	if (!tet)
	{
		cout << "fail to open the file" << endl;
		return nullptr;
	}
	char c;
	tet >> c;
	tet >> c;
	tet >> c;
	int numberofpoints, numberoftetrahedra;
	tet >> numberofpoints;
	tet >> numberoftetrahedra;

	cout << "point:  " << numberofpoints << "    tet:  " << numberoftetrahedra << endl;

	pointf3 pos;
	for (size_t i = 0; i < numberofpoints; i++)
	{
		tet >> pos[0];
		tet >> pos[1];
		tet >> pos[2];
		poses.push_back(pos);
	}

	unsigned a = 0;
	for (size_t i = 0; i < 4 * numberoftetrahedra; i++)
	{
		tet >> a;
		tetindices.push_back(a);
	}
	tet.close();


	// offset, scale
	pointf3 minP(std::numeric_limits<float>::max());
	pointf3 maxP(-std::numeric_limits<float>::max());
	for (const auto& pos : poses) {
		minP = pointf3::min(minP, pos);
		maxP = pointf3::max(maxP, pos);
	}
	auto offset = -pointf3::mid(minP, maxP).cast_to<vecf3>();
	float scale = sqrt(3.f / (maxP - minP).norm2());
	for (auto& pos : poses)
		pos = (scale * (pos.cast_to<vecf3>() + offset)).cast_to<pointf3>();

	auto tetMesh = TetMesh::New(poses, tetindices);

	auto spring = MassSpring::New(tetMesh);
	spring->InitSimu();


	auto sobj = SObj::New(nullptr, "tetmesh_");
	//CmptGeometry::New(sobj, tetMesh);
	CmptTransform::New(sobj);

	auto springmesh = SObj::New(sobj, "SpringSimulate");
	//CmptGeometry::New(sobj, spring);
	CmptSimulate::New(sobj, spring);
	CmptTransform::New(springmesh);

	auto meshObj = SObj::New(springmesh, "trimesh");
	CmptTransform::New(meshObj);
	CmptGeometry::New(springmesh, tetMesh->GetTriMesh());

	auto bsdf = BSDF_Frostbite::New();
	CmptMaterial::New(springmesh, bsdf);

	return sobj;
}
