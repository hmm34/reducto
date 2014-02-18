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
