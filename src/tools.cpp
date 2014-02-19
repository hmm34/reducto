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
#include <stdio.h>

namespace reducto
{
	void asciiToBinary(std::string file)
	{
		std::string line;
		std::ifstream inputFile(file);
		int value;
		int lineNum = 0;
		std::vector<unsigned char> buffer;

		while (std::getline(inputFile, line))
		{
			++lineNum;
			std::istringstream iss(line);
			while (iss >> value) 
			{
				if (lineNum <= 3)
				{
					// Ignore possible comment line
					if (iss.str().find("#") != std::string::npos)
					{
						buffer.push_back(value);
					}

					// Only the width and the height get two bytes
					else if (value > 255)
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
	}

  	void compareBinaryToAscii(std::string asciiFile, std::string binaryFile) 
  	{
  	}


	void binaryToAscii(std::string file)
	{
		std::ifstream inputFile(file, std::ifstream::binary | std::ifstream::ate);

 		std::streampos size = inputFile.tellg();
 		char* memblock = new char [size];
 		inputFile.seekg (0, std::ios::beg);
 		inputFile.read (memblock, size);
 		inputFile.close();

 		std::cerr << "size is: " << size << "\n";
	    for (int i = 0; i < size; ++i)
	    {
	    	int num = memblock[i];
	    	std::cerr << num << " ";
	    }
	    std::cerr << "\n";
	    delete[] memblock;
	}
}
