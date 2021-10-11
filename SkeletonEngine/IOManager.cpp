#include "IOManager.h"

#include <fstream>

namespace SkeletonEngine
{

	bool IOManager::readFileToBuffer(std::string file_path, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(file_path, std::ios::binary);
		if (file.fail())
		{
			perror(file_path.c_str());
			return false;
		}

		// seek to end of file
		file.seekg(0, std::ios::end);

		// get file size
		int file_size = file.tellg();
		file.seekg(0, std::ios::beg);

		// reduce file size by headers present
		file_size -= file.tellg();

		buffer.resize(file_size);
		file.read((char*)&(buffer[0]), file_size);
		file.close();

		return true;
	}

}
