#include "SimpleObjLoader.h"

#include <Basic/File.h>

#include <UGM/UGM>

#include <Engine/Scene/SObj.h>

#include <Engine/Scene/CmptMaterial.h>
#include <Engine/Scene/CmptTransform.h>
#include <Engine/Scene/CmptGeometry.h>

#include <Engine/Primitive/TriMesh.h>
#include <Engine/Material/BSDF_Frostbite.h>

#include <Basic/StrAPI.h>

#include <iostream>
#include <sstream>

using namespace Ubpa;
using namespace std;

Ptr<SObj> SimpleObjLoader::Load(const string& path) {
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
	auto obj = SObj::New(nullptr, StrAPI::Split(tmp, '/').back());
	CmptTransform::New(obj);
	CmptGeometry::New(obj, TriMesh::New(indices, positions));
	CmptMaterial::New(obj, BSDF_Frostbite::New());

	return obj;
}
