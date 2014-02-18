/*
	@brief Command line interface that executes various linear algebra 
	operations, such as SVD or PCA to compress images and reduce high-dimensional
	data to less dimensions.

	@copyright 2014 Drew Guarnera, Heather Michaud
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>

std::string usage = "Expected usage: \n"\
                    "\treducto 1 <image.pgm> \n"\
                    "\treducto 2 <image_b.pgm> \n"\
                    "\treducto 3 <header.txt> <SVD.txt> k \n"\
                    "\treducto 4 <image_b.pgm.SVD> \n";

const std::string ASCII_TO_BINARY = "1";
const std::string BINARY_TO_ASCII = "2";
const std::string COMPRESS_PGM    = "3";
const std::string DECOMPRESS_PGM  = "4";

//! @brief All of the main required tooling is namespaced under reducto
namespace reducto
{
	//! @brief Convert a provided PGM file in ASCII format to a binary file
	//! @param file The PGM file in ASCII format
	void asciiToBinary(std::string file)
	{
		std::string line;
		std::ifstream inputFile(file);
		int value;
		int lineNum = 0;
		std::vector<unsigned int> buffer;

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
		for (int i = 0; i < buffer.size(); ++i)
			std::cerr << buffer[i] << " ";
		
		std::cerr << "\n";
	}
}

//! @brief Verifies that there are the expected number of arguments and, if
//! 	not, returns false
//! and prints out the expected usage.
//! @param argc The number of arguments
//! @param expectedArgs The expected number of arguments
//! @retval true  If there are three arguments
//! @retval false If there aren't three arguments
bool areValidArguments(int argc, int expectedArgs)
{
	if (argc != expectedArgs)
	{
		std::cout << usage;
		return false;
	}
	return true;
}

int main(int argc, const char* argv[])
{
	if (argc < 2 || argc > 5)
	{
		std::cout << usage;
		return 1;	
	}

	std::string option = argv[1];
	std::string file   = argv[2];
	if (option == ASCII_TO_BINARY)
	{
		if (!areValidArguments(argc, 3))
			return 2;
		reducto::asciiToBinary(file);
	}
	else if (option == BINARY_TO_ASCII)
	{
		if (!areValidArguments(argc, 3))
			return 3;

		// file is the binary file to convert to PGM
		// Do things!
	}
	else if (option == COMPRESS_PGM)
	{
		if (!areValidArguments(argc, 5))
			return 4;

		// file is the header.txt
		std::string svd = argv[3];	// svd.txt
		int k = atoi(argv[4]);		// k
		// Do things!
	}
	else if (option == DECOMPRESS_PGM)
	{
		if (!areValidArguments(argc, 3))
			return 5;

		// file is the compressed image that needs decompressed
		// Do things!
	}
	else
	{
		std::cout << usage;
		return 6;
	}

	return 0;
}
