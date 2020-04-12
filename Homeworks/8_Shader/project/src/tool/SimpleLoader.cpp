#include "SimpleLoader.h"

#include <fstream>
#include <sstream>
#include <random>

using namespace Ubpa;
using namespace std;

namespace Ubpa::detail::SimpleLoader_ {
	vector<pointf2> GenUV(const vector<pointf3>& positions);
	vector<normalf> GenNormal(const vector<pointf3>& positions, const vector<unsigned> indices);
	vector<vecf3> GenTangent(const vector<pointf3>& positions,
		const vector<unsigned>& indices,
		const vector<normalf>& normals,
		const vector<pointf2>& texcoords);
}

SimpleLoader::OGLResources::~OGLResources() {
	delete va;
	delete eb;
	for (const auto& [name, vb] : name2vb)
		delete vb;
}

SimpleLoader::OGLResources* SimpleLoader::LoadObj(const std::string& path, bool noise) {
	// [read file to buffer]
	ifstream objfile(path, ios::in | ios::ate);
	if (!objfile.is_open()) {
		cout << "ERROR::SimpleLoader::LoadObj:" << endl
			<< "\t" << "open file (" << path << ") fail" << endl;
		return nullptr;
	}

	size_t size = objfile.tellg();
	objfile.seekg(0, ios::beg);
	char* buffer = new char[size+1];
	buffer[size] = 0;
	objfile.read(buffer, size);
	objfile.close();

	// [buffer to lines]
	vector<string> lines;
	size_t i = 0;
	while (i <= size) {
		size_t j = 0;
		while ((buffer[i + j] != '\n') && (buffer[i + j] != '\0'))
			j++;
		lines.emplace_back(buffer + i, j);
		i += j + 1;
	}
	delete[] buffer;
	buffer = nullptr;

	/*for (const auto& line : lines)
		cout << line << endl;*/

	// [decode]
	vector<pointf3> positions;
	vector<normalf> normals;
	vector<pointf2> texcoords;
	vector<unsigned> indices;
	vector<vecf3> tangents;
	for (auto line : lines) {
		if (line.empty() || line == "\n")
			continue;
		stringstream ss(line);
		string id;
		ss >> id;
		if (id == "v") {
			pointf3 pos;
			ss >> pos;
			positions.push_back(pos);
		}
		else if (id == "f") {
			valu3 tri;
			ss >> tri;
			for (auto idx : tri)
				indices.push_back(idx - 1);
		}
		else if (id == "vn") {
			normalf n;
			ss >> n;
			normals.push_back(n.normalize());
		}
		else if (id == "vt") {
			pointf2 uv;
			ss >> uv;
			texcoords.push_back(uv);
		}
	}

	auto rst = new OGLResources;

	// [offset, scale positions]
	bboxf3 box;
	for (const auto& pos : positions)
		box.combine_to_self(pos);
	auto center = box.center();
	float scale = sqrt(3.f / box.diagonal().norm2());
	for (auto& pos : positions)
		pos = (scale * (pos - center)).cast_to<pointf3>();

	const float lambda = 0.03f;
	if (noise) {
		if (normals.empty())
			normals = detail::SimpleLoader_::GenNormal(positions, indices);
		uniform_real_distribution distribution(0.f, 1.f);
		default_random_engine engine;
		map<pointf3, vecf3> p2o;
		for (size_t i = 0; i < positions.size(); i++) {
			pointf3 pos = positions[i];
			vecf3 offset;

			auto target = p2o.find(pos);
			if (target == p2o.end()) {
				float Xi = distribution(engine);
				offset = lambda * Xi * normals[i].cast_to<vecf3>();
				p2o[pos] = offset;
			}
			else
				offset = target->second;

			positions[i] += offset;
		}
		//normals.clear();
	}

	// [generate texcoords, normals, tangents]
	if (texcoords.empty())
		texcoords = detail::SimpleLoader_::GenUV(positions);
	if (normals.empty())
		normals = detail::SimpleLoader_::GenNormal(positions, indices);
	if (tangents.empty())
		tangents = detail::SimpleLoader_::GenTangent(positions, indices, normals, texcoords);
	
	/*for (const auto& uv : texcoords)
		cout << uv << endl;
	for (const auto& n : normals)
		cout << n << endl;
	for (const auto& t : tangents)
		cout << t << endl;*/

	// [generate VAO, VBO, EBO]
	auto vb_pos = new gl::VertexBuffer(positions.size() * sizeof(pointf3), positions[0].data());
	auto vb_uv = new gl::VertexBuffer(texcoords.size() * sizeof(pointf2), texcoords[0].data());
	auto vb_n = new gl::VertexBuffer(normals.size() * sizeof(normalf), normals[0].data());
	auto vb_t = new gl::VertexBuffer(tangents.size() * sizeof(vecf3), tangents[0].data());
	auto eb = new gl::ElementBuffer(gl::BasicPrimitiveType::Triangles, indices.size() / 3, indices.data());

	gl::VertexArray::Format format;
	format.attrptrs.push_back(vb_pos->AttrPtr(3, gl::DataType::Float, false, sizeof(pointf3)));
	format.attrptrs.push_back(vb_uv->AttrPtr(2, gl::DataType::Float, false, sizeof(pointf2)));
	format.attrptrs.push_back(vb_n->AttrPtr(3, gl::DataType::Float, false, sizeof(normalf)));
	format.attrptrs.push_back(vb_t->AttrPtr(3, gl::DataType::Float, false, sizeof(vecf3)));
	format.eb = eb;

	rst->name2vb["pos"] = vb_pos;
	rst->name2vb["uv"] = vb_uv;
	rst->name2vb["n"] = vb_n;
	rst->name2vb["t"] = vb_t;
	rst->eb = eb;
	rst->va = new gl::VertexArray({ 0,1,2,3 }, format);
	rst->positions = move(positions);
	rst->texcoords = move(texcoords);
	rst->normals = move(normals);
	rst->tangents = move(tangents);
	rst->indices = move(indices);

	return rst;
}

namespace Ubpa::detail::SimpleLoader_ {
	vector<pointf2> GenUV(const vector<pointf3>& positions) {
		vector<pointf2> texcoords;
		bboxf3 box;
		for (const auto& pos : positions)
			box.combine_to_self(pos);
		auto center = box.center();
		auto s = scalef3{ 2.f } / box.diagonal().cast_to<scalef3>();
		for (const auto& pos : positions) {
			normalf sphereNormal = (s * (pos - center).cast_to<scalef3>()).cast_to<normalf>().normalize();
			texcoords.push_back(sphereNormal.to_sphere_texcoord());
		}
		return texcoords;
	}

	vector<normalf> GenNormal(const vector<pointf3>& positions, const vector<unsigned> indices) {
		vector<normalf> wNs(positions.size(), normalf{ 0.f });
		for (size_t i = 0; i < indices.size(); i += 3) {
			size_t idx0 = indices[i];
			size_t idx1 = indices[i + 1];
			size_t idx2 = indices[i + 2];
			const auto& p0 = positions[idx0];
			const auto& p1 = positions[idx1];
			const auto& p2 = positions[idx2];
			auto v01 = p1 - p0;
			auto v02 = p2 - p0;
			auto wN = v01.cross(v02).cast_to<normalf>();
			wNs[idx0] += wN;
			wNs[idx1] += wN;
			wNs[idx2] += wN;
		}
		for (auto& n : wNs)
			n.normalize_self();
		return wNs;
	}

	vector<vecf3> GenTangent(
		const vector<pointf3>& positions,
		const vector<unsigned>& indices,
		const vector<normalf>& normals,
		const vector<pointf2>& texcoords)
	{
		const size_t vertexNum = positions.size();

		vector<vecf3> tanS(vertexNum);
		vector<vecf3> tanT(vertexNum);

		vector<vecf3> tangents(vertexNum);

		for (size_t i = 0; i < indices.size(); i += 3) {
			auto i1 = indices[i];
			auto i2 = indices[i + 1];
			auto i3 = indices[i + 2];

			const pointf3& v1 = positions.at(i1);
			const pointf3& v2 = positions.at(i2);
			const pointf3& v3 = positions.at(i3);

			const pointf2& w1 = texcoords.at(i1);
			const pointf2& w2 = texcoords.at(i2);
			const pointf2& w3 = texcoords.at(i3);

			float x1 = v2[0] - v1[0];
			float x2 = v3[0] - v1[0];
			float y1 = v2[1] - v1[1];
			float y2 = v3[1] - v1[1];
			float z1 = v2[2] - v1[2];
			float z2 = v3[2] - v1[2];

			float s1 = w2[0] - w1[0];
			float s2 = w3[0] - w1[0];
			float t1 = w2[1] - w1[1];
			float t2 = w3[1] - w1[1];

			float denominator = s1 * t2 - s2 * t1;
			float r = denominator == 0.f ? 1.f : 1.f / denominator;
			vecf3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r);
			vecf3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r);

			tanS[i1] += sdir;
			tanS[i2] += sdir;
			tanS[i3] += sdir;
			tanT[i1] += tdir;
			tanT[i2] += tdir;
			tanT[i3] += tdir;
		}

		for (size_t i = 0; i < vertexNum; i++) {
			const vecf3& n = normals.at(i).cast_to<vecf3>();
			const vecf3& t = tanS[i];

			// Gram-Schmidt orthogonalize
			auto projT = t - n * n.dot(t);
			tangents.at(i) = projT.norm2() == 0.f ? uniform_on_sphere<float>().cast_to<vecf3>() : projT.normalize();

			// Calculate handedness
			tangents.at(i) *= (n.cross(t).dot(tanT[i]) < 0.f) ? -1.f : 1.f;
		}

		return tangents;
	}
}
