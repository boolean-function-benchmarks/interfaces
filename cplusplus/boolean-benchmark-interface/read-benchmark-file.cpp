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
#include "TruthTable.h"

int main() {

	BenchmarkFileReader<int> reader;

	reader.open_file("../data/add3.pla");
	reader.read_header();

	reader.print_header();

	//reader.read_tt_file("../data/add3.tt");

	reader.read_pla_file("../data/add3.pla");

	TruthTable<int> table = reader.get_truth_table();

	table.print();
}
