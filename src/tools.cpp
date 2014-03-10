/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include "../eigen3/Eigen/SVD"

namespace reducto
{
	void asciiToBinary(std::string file)
	{
		std::string line;
		std::ifstream inputFile(file.c_str());
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
		inputFile.close();

		// Write to output file image_b.pgm
		int lastIndex = file.find_last_of(".");
		std::string oFile = file.substr(0, lastIndex) + "_b.pgm";
		std::ofstream outputFile(oFile.c_str(), std::ofstream::binary);
		
		for (int i = 0; i < buffer.size(); ++i)
			outputFile.write((char*)&buffer[i], sizeof(buffer[i]));

		outputFile.close();
	}

	void binaryToAscii(std::string file)
	{
		std::ifstream inputFile(file.c_str(), std::ifstream::binary | std::ifstream::ate);

 		std::streampos size = inputFile.tellg();
 		unsigned char* memblock = new unsigned char [size];
 		inputFile.seekg (0, std::ios::beg);
 		inputFile.read ((char*)memblock, size);
 		inputFile.close();

 		int lastIndex = file.find_last_of("_");
 		std::string oFile = file.substr(0, lastIndex) + "2.pgm";
 		std::ofstream outputFile(oFile.c_str());

 		 /*
 			First 5 bytes of data consist of header information
 			* Bytes 0-1: Width
 			* Bytes 2-3: Height
 			* Byte 4: Maximum Pixel Value
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

	void svdCompress(std::string header, std::string svd, int k)
	{
		std::vector<unsigned char> buffer;

		std::ifstream fheader(header);
		int width, height, grayscale;
		fheader >> width >> height >> grayscale;
		fheader.close();

		// This can probably be condensed into a method. Add width, height, and
		// max gray scale value to the buffer
		buffer.push_back(width / 256);
		buffer.push_back(width % 256);
		buffer.push_back(height / 256);
		buffer.push_back(height % 256);
		buffer.push_back(grayscale % 256);
		buffer.push_back(grayscale % 256);

		int diff = 0;
		bool wide = false, tall = false;
		int rank = 1;
		if (width > height)
		{
			diff = width - height;
			// diff is how many columns are removed from the right of Σ,
			// and how many rows can be removed from the bottom of V^T, 
			// which is the equivalent to the number of columns that can be
			// removed from V
			wide = true;
		}
		else if (height > width)
		{
			diff = height - width;
			// diff is how many columns can be removed from the right of U,
			// and how many rows can be removed from the bottom of Σ
			tall = true;
		}

		std::ifstream fsvd(svd);

		// Given an m by n matrix A, U is m by m
		for (int i = 0; i < width; ++i)
		{
			rank = 1;
			std::string line;
			std::getline(fsvd, line);
			std::stringstream ss(line);
			for (int j = 0; j < width; ++j)
			{
				int temp;
				ss >> temp; // throw away

				// keep those values up to the rank ond disregard unneeded
				// dimensions. the right side of U can be removed.
				//
				// note: how can we gaurantee that temp < 256 ? or that it's
				// even an integer?
				if (((wide && i < width - diff) || !wide) && rank <= k)
					buffer.push_back(temp);
				++rank;
			}
		}

		// Given an m by n matrix A, Σ (S) is m by n
		rank = 1;
		for (int i = 0; i < height; ++i)
		{
			std::string line;
			std::getline(fsvd, line);
			std::stringstream ss(line);
			for (int j = 0; j < width; ++j)
			{
				int temp;
				ss >> temp;	// throw away

				// keep the singlar values along the diagonal, but only up to
				// the given rank approximation. the bottom of sigma can be
				// removed
				//
				// note: how can we gaurantee that temp < 256 ? or that it's
				// even an integer?
				if (i == j && rank <= k)
					buffer.push_back(temp); 
			}
			++rank;
		}

		// Given an m by n matrix A, V is n by n
		rank = 1;
		for (int i = 0; i < height; ++i)
		{
			std::string line;
			std::getline(fsvd, line);
			std::stringstream ss(line);
			for (int j = 0; j < height; ++j)
			{
				int temp;
				ss >> temp; // thow away

				// keep those values up to the rank and disregard unneeded
				// dimensions. the bottom of V^T can be removed
				//
				// note: how can we gaurantee that temp < 256 ? or that it's
				// even an integer?
				if (((tall & j < height - diff) || !tall) && rank <= k)
					buffer.push_back(temp);
			}
			++rank;
		}

		fsvd.close();

		// We'll want to take the resulting compressed image and name it
		// image_b.pgm.SVD - which could take some finessing of the original
		// ascii to binary te preserve method length.
		std::string temp = "image_b.pgm.svd";
		std::ofstream output(temp, std::ofstream::binary);

		// Take width, height, and grayscale value of image. Then add needed
		// values for U based on rank, the singular values of Σ based on rank
		// (only the diagonal), and the needed values for transpose of V based
		// on rank. When read in, we'll fill the remaining values with zeroes
		// that weren't stored - but the width and height are needed to know
		// where we should fill on the gaps.
		for (int i = 0; i < buffer.size(); ++i)
			outputFile.write((char*)&buffer[i], sizeof(buffer[i]));

		output.close();
	}
}
