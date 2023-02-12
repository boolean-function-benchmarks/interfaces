//============================================================================
// Project     : General Boolean Function Benchmark Suite
// Version     : 1.0
// Description : C++ interface for PLU/PLA Boolean function benchmark files.
// License 	   :
//============================================================================

#include "BenchmarkFileReader.h"

int main() {

	BenchmarkFileReader<char>* reader = new BenchmarkFileReader<char>();

	reader->read_file("../../data/add2.pla");
	reader->print_truth_table();

	delete reader;
}
