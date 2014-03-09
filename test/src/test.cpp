/*
	@brief Unit testing, timing, and storage metric collection

	@copyright 2014 Drew Guarnera, Heather Michaud
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctime>
#include "../../src/tools.hpp"

const std::string testUsage = "Expected usage: \n"\
							 "\ttest : runs all test cases\n"\
							 "\ttest storage : runs metrics on compression sizes\n"\
							 "\ttest timing : runs timing metrics\n"\
							 "\ttest unit : runs unit tests\n";

std::string pgmFiles[] = {
	"test/fixtures/2DGel-2.pgm",
	"test/fixtures/FEEP.pgm",
	"test/fixtures/apollonian_gasket.ascii.pgm",
	"test/fixtures/baboon.ascii.pgm",
	"test/fixtures/balloons.ascii.pgm",
	"test/fixtures/balloons_noisy.ascii.pgm",
	"test/fixtures/barbara.ascii.pgm",
	"test/fixtures/body1.ascii.pgm",
	"test/fixtures/body2.ascii.pgm",
	"test/fixtures/body3.ascii.pgm",
	"test/fixtures/brain_398.ascii.pgm",
	"test/fixtures/brain_492.ascii.pgm",
	"test/fixtures/brain_508.ascii.pgm",
	"test/fixtures/brain_604.ascii.pgm",
	"test/fixtures/casablanca.ascii.pgm",
	"test/fixtures/coins.ascii.pgm",
	"test/fixtures/columns.ascii.pgm",
	"test/fixtures/columns.pgm",
	"test/fixtures/dla.ascii.pgm",
	"test/fixtures/dragon.ascii.pgm",
	"test/fixtures/f14.ascii.pgm",
	"test/fixtures/feep.ascii.pgm",
	"test/fixtures/fractal_tree.ascii.pgm",
	"test/fixtures/gator.ascii.pgm",
	"test/fixtures/glassware_noisy.ascii.pgm",
	"test/fixtures/hands.ascii.pgm",
	"test/fixtures/handsmat.ascii.pgm",
	"test/fixtures/lena.ascii.pgm",
	"test/fixtures/marcie.ascii.pgm",
	"test/fixtures/mona_lisa.ascii.pgm",
	"test/fixtures/mother_daughter.ascii.pgm",
	"test/fixtures/mountain.ascii.pgm",
	"test/fixtures/pbmlib.ascii.pgm",
	"test/fixtures/pepper.ascii.pgm",
	"test/fixtures/roi_14.ascii.pgm",
	"test/fixtures/saturn.ascii.pgm",
	"test/fixtures/screws.ascii.pgm",
	"test/fixtures/snap.ascii.pgm",
	"test/fixtures/surf.ascii.pgm",
	"test/fixtures/totem.ascii.pgm",
	"test/fixtures/tracks.ascii.pgm",
	"test/fixtures/tracks.pgm",
	"test/fixtures/venus1.ascii.pgm",
	"test/fixtures/venus2.ascii.pgm",
	"test/fixtures/x31_f18.ascii.pgm"
};

//! @brief Creates a randomly generated PGM file with specified width and height
void createPGM(std::string file, int width, int height)
{
	int seed = 0;

	std::ofstream output(file.c_str());
	output << "P2\n";
	output << "# I will make you teensy!\n";
	output << width << " " << height << "\n";
	output << "255\n";
	for (int columns = 0; columns < width; ++columns)
	{
		for (int rows = 0; rows < height; ++rows)
		{
			srand(seed++);
			output << rand() % 255 + 1;
			if (rows != height - 1)
				output << " ";
		}
		output << "\n";
	}
	output.close();
}

//! @brief Executes a series of timing tests for metric collection
void runTimingTests()
{
	std::cout << "-------------- Timing Metrics ---------------\n";
	// Run timing tests and write to a CSV file [with AVERAGES - not all tests..]

	std::string resultFile = "timing-results.csv";
	std::ofstream output(resultFile.c_str());
	output << "width, height, total pixels, ";
	output << "ascii to binary average time (of 10 runs) in ms, ";
	output << "binary to ascii average time (of 10 runs) in ms\n";

	std::string test1 = "test.pgm";
	std::string test2 = "test_b.pgm";
	int maxSize = 500;
	for (int size = 50; size <= maxSize; size+=50)
	{
		long double msTotal1 = 0; // for ascii to binary
		long double msTotal2 = 0; // for binary to ascii
		int numRuns = 10;
		for (int times = 0; times < numRuns; ++times)
		{
			createPGM(test1.c_str(), size, size);

			std::clock_t start1 = std::clock();
			reducto::asciiToBinary(test1);
			std::clock_t end1 = std::clock();

			std::clock_t start2 = std::clock();
			reducto::binaryToAscii(test2);
			std::clock_t end2 = std::clock();

			remove(test1.c_str());
			remove(test2.c_str());
			msTotal1 += 1000.0 * (end1 - start1) / CLOCKS_PER_SEC;
			msTotal2 += 1000.0 * (end2 - start2) / CLOCKS_PER_SEC;
		}
		std::cerr << "Timing: " << size << " of " << maxSize << "\r";

		double average1 = msTotal1 / (double)numRuns;
		double average2 = msTotal2 / (double)numRuns;
		output << size << ", " << size << ", " << size * size << ", ";
		output << average1 << "," << average2 << "\n";
	}
	std::cerr << "\n";

	// Clean up
	int lastIndex = test1.find_last_of(".");
	std::string oFile = test1.substr(0, lastIndex) + "_b.pgm";
	remove(oFile.c_str());
	output.close();
}

//! @brief Executes a series of storage tests for metric collection
void runStorageTests()
{
	std::cout << "-------------- Storage Metrics --------------\n";
	// Run storage tests and write to a CSV file [with AVERAGES - not all tests..]

	std::string resultFile = "storage-results.csv";
	std::ofstream output(resultFile.c_str());
	output << "total pixels, size of ascii file (bytes), ";
	output << "size of compressed binary file (bytes), ";
	output << "size of decompressed ascii file (bytes)\n";

	std::string test1 = "test.pgm";
	int maxSize = 1000;
	for (int size = 50; size <= maxSize; size+=50)
	{
		output << size * size << ", ";

		createPGM(test1.c_str(), size, size);
		struct stat ifilestatus;
		stat(test1.c_str(), &ifilestatus);
		output << ifilestatus.st_size << ", ";
		reducto::asciiToBinary(test1);
		remove(test1.c_str());

		std::string oFile1 = "test_b.pgm";
		struct stat ofilestatus1;
		stat(oFile1.c_str(), &ofilestatus1);
		output << ofilestatus1.st_size << ", ";

		std::string oFile2 = "test2.pgm";
		reducto::binaryToAscii(oFile1);
		struct stat ofilestatus2;
		stat(oFile2.c_str(), &ofilestatus2);
		output << ofilestatus2.st_size << "\n";

		remove(oFile1.c_str());
		remove(oFile2.c_str());

		std::cerr << "Storage: " << size << " of " << maxSize << "\r";
	}
	std::cerr << "\n";
	output.close();
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
