#pragma once

#include <Basic/Ptr.h>

#include <string>

namespace Ubpa {
	class SObj;
	namespace SimpleLoader {
		Ptr<SObj> LoadObj(const std::string& path);
		Ptr<SObj> LoadTet(const std::string& path);
	}
}
