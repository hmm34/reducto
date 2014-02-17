/*
	@brief Unit testing, timing, and storage metric collection

	@copyright 2014 Drew Guarnera, Heather Michaud
*/

#include <iostream>

const std::string testUsage = "Expected usage: \n"\
							 "\ttest : runs all test cases\n"\
							 "\ttest storage : runs metrics on compression sizes\n"\
							 "\ttest timing : runs timing metrics\n"\
							 "\ttest unit : runs unit tests\n";


//! @brief Executes a series of timing tests for metric collection
void runTimingTests()
{
	std::cout << "-------------- Timing Metrics ---------------\n";
	// Run timing tests and write to a CSV file [with AVERAGES - not all tests..]
}

//! @brief Executes a series of storage tests for metric collection
void runStorageTests()
{
	std::cout << "-------------- Storage Metrics --------------\n";
	// Run storage tests and write to a CSV file [with AVERAGES - not all tests..]
}

//! @brief Executes a series of unit tests for verification
void runUnitTests()
{
	std::cout << "---------------- Unit Tests -----------------\n";

	/*
	char* pgmFile = "image.pgm";
	reducto::asciiToBinary(pgmFile);

	char* binaryFile = "image_b.pgm";
	reducto::binaryToAscii(binaryFile);

	char* pgmFile2 = "image.pgm";
	char* headerFile = "header.txt";
	char* svdFile = "SVD.txt";
	int k = 2;
	reducto::compressFile(pgmFile2, headerFile, svdFile, k);

	char* svdFile "image2.pgm.svd";
	reducto::decompressFile(svdFile);
	*/
}

int main(int argc, char const *argv[])
{
	// If no arguments have been provided, execute all tests
	if (argc < 2)
	{
		runUnitTests();
		runStorageTests();
		runTimingTests();
	}
	
	// Otherwise run the specific test provided
	else if (argc == 2)
	{
		if (strcmp(argv[1], "storage") == 0)
		{
			runStorageTests();
		}
		else if (strcmp(argv[1], "timing") == 0)
		{
			runTimingTests();
		}
		else if (strcmp(argv[1], "unit") == 0)
		{
			runUnitTests();
		}
		else
		{
			std::cout << testUsage;
			return 2;
		}
	}
	else
	{
		std::cout << testUsage;
		return 1;
	}
	return 0;
}
