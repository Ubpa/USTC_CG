#pragma once

#include <Basic/Ptr.h>

#include <string>

namespace Ubpa {
	class SObj;
	namespace SimpleObjLoader {
		Ptr<SObj> Load(const std::string& path);
	}
}
