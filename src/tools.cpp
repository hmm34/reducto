/*
	@copydoc tools.hpp
*/

#include "tools.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include "../eigen3/Eigen/SVD"
#include "../eigen3/Eigen/Eigen"

// Debugging
#include <iostream>

namespace reducto
{
	// --------------------------------------------------------------------- //
	void asciiToBinary(std::string file)
	{
		std::string line;
		std::ifstream inputFile(file.c_str());

		if (!inputFile)
		{
			std::cerr << "Could not open file " << file << "\n";
			return;
		}

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
		
		for (unsigned int i = 0; i < buffer.size(); ++i)
			outputFile.write((char*)&buffer[i], sizeof(buffer[i]));

		outputFile.close();
	}

	// --------------------------------------------------------------------- //
	void binaryToAscii(std::string file)
	{
		std::ifstream inputFile(file.c_str(), std::ifstream::binary | std::ifstream::ate);

		if (!inputFile)
		{
			std::cerr << "Could not open file " << file << "\n";
			return;
		}

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
 				outputFile << "\n";
 			else
 				outputFile << " ";
 		}
	    delete[] memblock;
	    outputFile.close();
	}

	// --------------------------------------------------------------------- //
	void asciiToSvd(std::string file)
	{
		int lastIndex = file.find_last_of(".pgm");
		std::string header = file.substr(0, lastIndex - 3) + "_header.txt";
		std::string svd = file.substr(0, lastIndex - 3) + "_svd.txt";

		std::ifstream inputFile(file.c_str());
		if (!inputFile)
		{
			std::cerr << "Could not open file " << file << "\n";
			return;
		}

		std::string line;
		getline(inputFile, line);	// "P2"
		getline(inputFile, line);	// "#Created by Irfan View"

		float value;
		std::stringstream ss;

		int height, width, grayscale;
		ss << inputFile.rdbuf();
		ss >> width >> height;		// X, Y
		ss >> grayscale;			// Max

		Eigen::MatrixXf a(height, width);
		for (int yPos = 0; yPos < height; ++yPos)
		{
			for (int xPos = 0; xPos < width; ++xPos) 
			{
				ss >> value;
				a(yPos, xPos) = value;
			}
		}
		inputFile.close();

		std::ofstream headerFile(header.c_str());
		headerFile << width << " " << height << " " << grayscale;
		headerFile.close();

		Eigen::MatrixXf u(width, width), s(height, width), v(height, height);
		Eigen::VectorXf singularVals;

		std::cout << a << "\n";
		Eigen::JacobiSVD<Eigen::MatrixXf> svdm(a, Eigen::ComputeThinU | Eigen::ComputeThinV);

		u = svdm.matrixU();
		v = svdm.matrixV();

		singularVals = svdm.singularValues();
		for (int i = 0; i < a.rows(); ++i)
		{
			for (int j = 0; j < a.cols(); ++j)
			{
				int val = 0;
				if (i == j)
					val = singularVals(i);
				s(i, j) = val;
			}
		}

		std::ofstream svdFile(svd.c_str());
		svdFile << u << "\n" << s << "\n" << v << "\n";
		svdFile.close();
	}

	// --------------------------------------------------------------------- //
	void svdCompress(std::string header, std::string svd, int k)
	{
		std::vector<unsigned char> buffer;

		std::ifstream fheader(header.c_str());

		if (!fheader)
		{
			std::cerr << "Could not open file " << header << "\n";
			return;
		}

		int width, height, grayscale;
		fheader >> width >> height >> grayscale;
		fheader.close();

		// This can probably be condensed into a method. Add width, height, and
		// max gray scale value to the buffer
		buffer.push_back(width / 256);
		buffer.push_back(width % 256);
		buffer.push_back(height / 256);
		buffer.push_back(height % 256);
		buffer.push_back(grayscale);

		// Must store the rank so we know where to fill in values for
		// recreating the original matrix
		buffer.push_back(k / 256);
		buffer.push_back(k % 256);

		// We'll want to take the resulting compressed image and name it
		// image_b.pgm.SVD - which could take some finessing of the original
		// ascii to binary te preserve method length.
		std::string temp = "image_b.pgm.svd";
		std::ofstream output(temp.c_str(), std::ofstream::binary);

		// Write to values
		for (unsigned int i = 0; i < buffer.size(); ++i)
			output.write((char*)&buffer[i], sizeof(buffer[i]));

		int rank = 0;
		std::ifstream fsvd(svd.c_str());

		if (!fsvd)
		{
			std::cerr << "Could not open file " << svd << "\n";
			return;
		}

		// Given an m by n matrix A, U is m by m
		for (int i = 0; i < height; ++i)
		{
			rank = 1;
			std::string line;
			std::getline(fsvd, line);
			std::stringstream ss(line);
			for (int j = 0; j < height; ++j)
			{
				float temp;
				ss >> temp; // throw away

				// keep those values up to the rank ond disregard unneeded
				// dimensions. the right side of U can be removed.
				if (rank <= k) 
				{
					std::cerr << "COMPRESS U: " << temp << "\n";
					output.write(reinterpret_cast<char*>(&temp), sizeof temp);
					++rank;
				}
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
				float temp;
				ss >> temp;	// throw away

				// keep the singlar values along the diagonal, but only up to
				// the given rank approximation. the bottom of sigma can be
				// removed
				if (i == j && rank <= k) 
				{
					std::cerr << "COMPRESS SIGMA: " << temp << "\n";
					output.write(reinterpret_cast<char*>(&temp), sizeof temp);
					++rank;
				}
			}
		}

		// Given an m by n matrix A, V is n by n
		for (int i = 0; i < width; ++i)
		{
			rank = 1;
			std::string line;
			std::getline(fsvd, line);
			std::stringstream ss(line);
			for (int j = 0; j < width; ++j)
			{
				float temp;
				ss >> temp; // thow away

				// keep those values up to the rank and disregard unneeded
				// dimensions. the right of V can be removed (bottom of V^T)
				if (rank <= k) 
				{
					std::cerr << "COMPRESS V: " << temp << "\n";
					output.write(reinterpret_cast<char*>(&temp), sizeof temp);
					++rank;
				}
			}
		}

		fsvd.close();
		output.close();
	}

	// --------------------------------------------------------------------- //
	void svdDecompress(std::string file)
	{
		// Open binary file. Read gray scale value, width, height of image
		std::ifstream inputFile(file.c_str(), std::ifstream::binary | std::ifstream::ate);

		if (!inputFile)
		{
			std::cerr << "Could not find file " << file << "\n";
			return;
		}

		const int bytesInHeader = 7;
 		unsigned char* memblock = new unsigned char [bytesInHeader];
 		inputFile.seekg (0, std::ios::beg);
 		inputFile.read ((char*)memblock, bytesInHeader);

		// Read SVD values and write to full matrices based on height and
		// width dimensions for multiplication.

		/*
 			First 5 bytes of data consist of header information
 			* Bytes 0-1: Width
 			* Bytes 2-3: Height
 			* Byte 4: Maximum Pixel Value
 			* Bytes 5-6: Rank approximation
 		*/
 		int width = memblock[0] * 256 + memblock[1];	// m
 		int height = memblock[2] * 256 + memblock[3];	// n
 		int maxPixel = memblock[4];
 		int rank = memblock[5] * 256 + memblock[6];
 		std::cout << "rank is: " << rank << "\n";

 		// Matrix U: m by m (height by height)
 		Eigen::MatrixXf u(height, height);
 		int k = 1;
 		for (int i = 0; i < height; ++i) 
 		{ 
 			k = 1;
 			for (int j = 0; j < height; ++j) 
 			{
 				float val = 0;
 				if (k <= rank) {
 					inputFile.read(reinterpret_cast<char*>( &val ), sizeof val);
 					std::cout << "Read to value... " << val << "\n";
 					++k;
 				}
 				else {
 					std::cout << "NO READ.... " << val << "\n";
 				}
 				u(i, j) = val;
 			}
 		}

 		// Matrix S: m by n (height by width)
 		Eigen::MatrixXf s(height, width);
 		k = 1;
 		for (int i = 0; i < height; ++i) 
 		{
 			for (int j = 0; j < width; ++j)
 			{
 				float val = 0;
 				if (k <= rank && i == j) 
 				{
 					inputFile.read(reinterpret_cast<char*>( &val ), sizeof val);
 					std::cout << "Read to value... " << val << "\n";
 					++k;
 				}
 				else {
 					std::cout << "NO READ.... " << val << "\n";
 				}
 				s(i, j) = val;
 			}
 		}

 		// Matrix V: n by n (width by width)
 		Eigen::MatrixXf v(width, width);
 		k = 1;
 		for (int i = 0; i < width; ++i)
 		{
 			for (int j = 0; j < width; ++j)
 			{
 				float val = 0;
 				if (k <= rank) {
 					inputFile.read(reinterpret_cast<char*>( &val ), sizeof val);
 					std::cout << "Read to value... " << val << "\n";
		 		}
		 		else {
 					std::cout << "NO READ.... " << val << "\n";
 				}
 				v(i, j) = val;
 			}
 			++k;
 		}

 		delete[] memblock;

		// Multiply matrices to get the original image in matrix A
		Eigen::MatrixXf vT = v.transpose();
		Eigen::MatrixXf a = (u * s) * vT;

		std::cout << "u\n" << u << "\n";
		std::cout << "s\n" << s << "\n";
		std::cout << "vt\n" << vT << "\n";
		std::cout << "a\n" << a << "\n";

		// Write gray scale value, width, height, matrix A into ASCII PGM file

		//! @todo I think k needs renamed to be what the k value was used
 		//! to produce this image.
 		std::ofstream outputFile("image_k.pgm");

 		// Header
 		outputFile << "P2\n";
 		outputFile << "# I will make you teensy!\n";
 		outputFile << width << " " << height << "\n";
 		outputFile << maxPixel << "\n";
 		
 		// Approximate original image
 		for (int i = 0; i < a.rows(); ++i)
 		{
 			for (int j = 0; j < a.cols(); ++j)
 			{
 				float temp = a(i, j);
 				int out = static_cast<int>(temp);
 				outputFile << out << " ";
 			}
 			outputFile << "\n";
 		}
 		//outputFile << a << "\n";
	}

}

