#ifndef BENCHMARKS_BOOL_BENCHMARKREADER_H_
#define BENCHMARKS_BOOL_BENCHMARKREADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <vector>
#include <any>
#include <memory>
#include <cmath>
#include <utility>
#include <algorithm>

#include "TruthTable.h"


/*
 *  @brief The generic class BenchmarkFileReader provides methods for reading PLU as well as
 *  PLA files.
 *
 *  @details In PLU files the truth tables are divided into chunks and compressed to integers.
 *  PlA files contain the whole truth table of the respective Boolean function.The compressed input
 *  and output data is stored in 2D vectors. Likewise a truth table is also stored in 2D vectors
 *  and bundled in a pair.This is a generic class which uses templates.
 *
 *  @tparam T Generic type which is used for the truth table.
 *
 */
template<class T>
class BenchmarkFileReader {
private:

	std::ifstream ifs;
	std::string line;

	TruthTable<T> table;

	int inputs;
	int outputs;
	int chunks;

	const int PLU = 0;
	const int PLA = 1;

	void validate_file(std::string file_path);
	int file_format(std::string file_path);

public:
	BenchmarkFileReader() = default;
	~BenchmarkFileReader() = default;
	void read_plu_file(std::string file_path);
	void read_pla_file(std::string file_path);
	void read_file(std::string file_path);
	void print_compressed_data();
	void print_truth_table();

	const std::vector<std::vector<T> >& get_compressed_inputs() const;
	const std::vector<std::vector<T> >& get_compressed_outputs() const;

	const std::pair<std::vector<std::vector<T>>, std::vector<std::vector<T>> >& get_truth_table() const;


};

/**
 * @brief Prints the truth tables row-wise in a raw fashion without any header
 *
 * @details Inputs and outputs are separated with whitespace.
 *
 */
template<class T>
void BenchmarkFileReader<T>::print_truth_table() {
	this->table.print();
}


/**
 * @brief Validate the benchmark file
 *
 * @details Checks the length of the file path and the extension of the file.
 *
 * @param file_path Given path for the benchmark file
 */
template<class T>
void BenchmarkFileReader<T>::validate_file(std::string file_path) {

	// Check if file path is empty
	if (file_path.size() == 0) {
		throw std::runtime_error("File path is an empty string!");
	}

	// Check whether the file exists
	if (!std::filesystem::exists(file_path)) {
		throw std::runtime_error("File does not exist!");
	}

	// Extract the file extension
	std::string extension = std::filesystem::path(file_path).extension();

	// Convert to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(),
			::tolower);

	if (extension != ".plu" && extension != ".pla") {
		throw std::runtime_error("Invalid file format!");
	}

}

/**
 * @brief Checks and returns the format of the benchmark file.
 *
 * @details Extracts the file extension, transforms it to lower case
 * and checks if the format is PLU or PLA.
 *
 * @param file_path Given path for the benchmark file
 *
 * @return File format status of the file:
 * 		   0 : PLU
 * 		   1 : PLA
 */
template<class T>
int BenchmarkFileReader<T>::file_format(std::string file_path) {

	// Extract the file extension
	std::string extension = std::filesystem::path(file_path).extension();

	// Convert to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(),
			::tolower);

	if (extension == ".plu") {
		return this->PLU;
	} else if (extension == ".pla") {
		return this->PLA;
	} else {
		throw std::runtime_error("Invalid file format!");
	}

}

/**
 * @brief Reads and stores uncompressed or compressed data of a respective truth table
 *
 * @details Uses file input stream. The data is stored in the 2D vectors of the truth table object.
 * The number inputs, outputs and chunks are obtained from the given benchmark file.
 *
 * @see TruthTable
 *
 * @param file_path Given path for the benchmark file
 */
template<class T>
void BenchmarkFileReader<T>::read_file(std::string file_path) {

	// First, validate the file path
	this->validate_file(file_path);

	// Determine the file format
	int format = this->file_format(file_path);

	// Open the filestream then
	ifs.open(file_path, std::ifstream::in);

	// Continue only when the filestream could be opened
	if (ifs.is_open()) {

		char c;
		int rows;

		std::string str;

		T input;
		T output;

		// Vectors for the data of each row
		std::vector<T> row_inputs;
		std::vector<T> row_outputs;

		// Clear the table from potential previous data, reset the compressed status
		table.reset();

		// Skip the markers for the number of inputs, outputs and chunks (i, e and p)
		// and read the respective values
		ifs >> str >> this->inputs;
		ifs >> str >> this->outputs;

		// Depending on the format, read the number of chunks and calculate the number
		// of rows of the table
		if (format == this->PLU) {
			ifs >> str >> this->chunks;
			rows = this->chunks;
			this->table.set_compressed(true);
		} else {
			rows = pow(2, this->inputs);
		}

		// Iterate over the number of chunks
		for (int i = 0; i < rows; i++) {

			// Check if any error state flags have been set
			if (!ifs.good()) {
				throw std::runtime_error("Error while reading PLA file!");
			}

			// Read the compressed inputs of the current row and store
			// them in the temporary vector
			for (int j = 0; j < this->inputs; j++) {
				ifs >> input;
				row_inputs.push_back(input);
			}

			// Inputs and outputs are seperated with whitespaces
			// Therefore, we skip this whitespace
			do {
				ifs.get(c);
			} while (ifs.peek() == ' ');

			// Read and store the outputs of the current row
			for (int j = 0; j < this->outputs; j++) {
				ifs >> output;
				row_outputs.push_back(output);
			}

			// Store the chunks in the 2D vectors of the truth table
			this->table.append_inputs(row_inputs);
			this->table.append_outputs(row_outputs);

			row_inputs.clear();
			row_outputs.clear();

		}

	} else {
		throw std::runtime_error("Error opening benchmark file!");
	}
}

#endif /* BENCHMARKS_BOOL_BENCHMARKREADER_H_ */
