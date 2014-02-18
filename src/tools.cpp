/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>

namespace reducto
{
	void asciiToBinary(std::string file)
	{
		std::string line;
		std::ifstream inputFile(file);
		int value;
		int lineNum = 0;
		std::vector<char> buffer;

		while (std::getline(inputFile, line))
		{
			std::istringstream iss(line);
			while (iss >> value) 
			{
				if (++lineNum == 1)
				{
					// Only the width and the height get two bytes
					if (value > 255)
					{
						buffer.push_back(value / 256);
						buffer.push_back(value % 256);
					}
					else
					{
						buffer.push_back(0);
						buffer.push_back(value);	
					}
				}
				else if (lineNum == 3)
				{
					// Ignore possible comment line
					if (iss.str().find("#") != std::string::npos)
					{
						buffer.push_back(value);
					}
				}
				else 
				{
					buffer.push_back(value);
				}
			}
		}

		// Write to output file image_b.pgm
		int lastIndex = file.find_last_of(".");
		std::string oFile = file.substr(0, lastIndex) + "_b.pgm";
		std::ofstream outputFile(oFile, std::ofstream::binary);
		for (int i = 0; i < buffer.size(); ++i)
			outputFile.write((char*)&buffer[i], sizeof(buffer[i]));
		outputFile.close();

		// Write to console for debugging purposes
		/*
		for (int i = 0; i < buffer.size(); ++i)
			std::cerr << buffer[i] << " ";
		std::cerr << "\n";
		*/
	}
}
