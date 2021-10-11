#pragma once
#include <string>
#include <vector>

namespace SkeletonEngine
{

	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string file_path, std::vector<unsigned char>& buffer);
	};

}
