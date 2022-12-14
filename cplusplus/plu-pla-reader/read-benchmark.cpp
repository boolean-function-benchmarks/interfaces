//============================================================================
// Project     :
// Author      : Roman Kalkreuth
//         		 https://orcid.org/0000-0003-1449-5131,
//          	 https://www.researchgate.net/profile/Roman-Kalkreuth,
//         		 https://twitter.com/RomanKalkreuth
// Version     : 1.0
// Description :
// License 	   :
//============================================================================

#include "BenchmarkFileReader.h"

int main() {

	BenchmarkFileReader<int>* reader = new BenchmarkFileReader<int>();

	reader->read_plu_file("data/add1c.plu");
	reader->print_compressed_data();

	//reader->read_pla_file("data/add1.pla");
	//reader->print_truth_table();

	delete reader;
}