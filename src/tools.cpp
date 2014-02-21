/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
#include <fstream>
#include <sstream>
#include <vector>

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

	void binaryToAscii(std::string file)
	{
		std::ifstream inputFile(file, std::ifstream::binary | std::ifstream::ate);

 		std::streampos size = inputFile.tellg();
 		unsigned char* memblock = new unsigned char [size];
 		inputFile.seekg (0, std::ios::beg);
 		inputFile.read ((char*)memblock, size);
 		inputFile.close();

 		int lastIndex = file.find_last_of("_");
 		std::string oFile = file.substr(0, lastIndex) + "2.pgm";
 		std::ofstream outputFile(oFile);

 		 /*
 			First 5 bytes of data consist of header information
 			Bytes 0-1: Width
 			Bytes 2-3: Height
 			Byte 4: Maximum Pixel Value
 		*/
 		int width = memblock[0] * 256 + memblock[1];
 		int height = memblock[2] * 256 + memblock[3];
 		int maxPixel = memblock[4];

 		// Write out pgm header
 		outputFile << "P2\n";
 		outputFile << "# I will make you teensy!\n";
 		outputFile << width << " " << height << "\n";
 		outputFile << maxPixel << "\n";

 		// Iterate over pixel values from remaining bytes in binary file
 		for (int index = 0; index < width * height; ++index)
 		{
 			// Write out pixel value in standard integer ascii representation
 			outputFile << (int)memblock[5 + index];
 			if (((index + 1) % width == 0) && index > 0)
 			{
 				outputFile << "\n";
 			}
 			else
 			{
 				outputFile << " ";
 			}
 		}
	    delete[] memblock;
	    outputFile.close();
	}
}
