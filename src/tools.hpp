/*
	@brief Contains the necessary tools to perform file compression and
	decompression using binary formats and SVD, as well as PCA to reduce
	high-dimensional data

	@copyright 2014 Drew Guarnera, Heather Michaud
*/

#include <string>

namespace reducto
{
	//! @brief Convert a provided PGM file in ASCII format to a binary file
	//! @param file The PGM file in ASCII format
	void asciiToBinary(std::string file);

	//! @brief Convert a provided PGM file in binary format to ASCII
	//! @param file The PGM file in binary format
	void binaryToAscii(std::string file);
}
