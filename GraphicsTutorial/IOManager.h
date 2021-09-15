#pragma once
#include <string>
#include <vector>

class IOManager
{
public:
	static bool readFileToBuffer(std::string file_path, std::vector<unsigned char>& buffer);
};

