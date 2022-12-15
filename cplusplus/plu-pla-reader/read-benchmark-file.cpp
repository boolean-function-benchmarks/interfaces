//============================================================================
// Project     : General Boolean Function Benchmark Suite
// Author      : Roman Kalkreuth
//         		 https://orcid.org/0000-0003-1449-5131,
//          	 https://www.researchgate.net/profile/Roman-Kalkreuth,
//         		 https://twitter.com/RomanKalkreuth
// Version     : 1.0
// Description : C++ interface for PLU/PLA Boolean function benchmark files.
// License 	   :
//============================================================================

#include "BenchmarkFileReader.h"

int main() {

	BenchmarkFileReader<char>* reader = new BenchmarkFileReader<char>();

	reader->read_pla_file("test-data/add2.pla");
	reader->print_truth_table();

	delete reader;
}
