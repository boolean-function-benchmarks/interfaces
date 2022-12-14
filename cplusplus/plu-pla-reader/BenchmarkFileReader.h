/*
 *  The class BenchmarkFileReader provides methods for reading PLU as well as
 *  PLA files.
 *
 *  In PLU files the truth tables are divided into chunks and compressed to integers.
 *  PlA files contain the whole truth table of the respective Boolean function.
 *
 *  The compressed input and output data is stored in 2D vectors.
 *  Likewise a truth table is also stored in 2D vectors and bundled in a pair.
 *
 *  This is a generic class which uses templates.
 *
 *  Author: Roman Kalkreuth,
 *         	https://orcid.org/0000-0003-1449-5131,
 *          https://www.researchgate.net/profile/Roman-Kalkreuth,
 *         	https://twitter.com/RomanKalkreuth
 */

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

template<class T>
class BenchmarkFileReader {
private:

	std::pair<std::vector<std::vector<T>>, std::vector<std::vector<T>> > truth_table;

	std::vector<std::vector<T>> compressed_inputs;
	std::vector<std::vector<T>> compressed_outputs;

	std::ifstream ifs;
	std::string line;

	int inputs;
	int outputs;
	int chunks;

	void validate_file(std::string file_path, std::string format);

public:
	BenchmarkFileReader() = default;
	~BenchmarkFileReader() = default;
	void read_plu_file(std::string file_path);
	void read_pla_file(std::string file_path);
	void print_compressed_data();
	void print_truth_table();

	const std::vector<std::vector<T> >& getCompressedInputs() const;
	const std::vector<std::vector<T> >& getCompressedOutputs() const;

	const std::pair<std::vector<std::vector<T>>, std::vector<std::vector<T>> >& getTruthTable() const;

};

/**
 * Prints the truth tables row-wise in a raw fashion without any header.
 * However, inputs and outputs are separated with whitespace.
 */
template<class T>
void BenchmarkFileReader<T>::print_truth_table() {

	// Get input and output vectors from the pair
	std::vector<std::vector<T> > table_inputs = this->truth_table.first;
	std::vector<std::vector<T> > table_outputs = this->truth_table.second;

	// First check whether the vectors contain
	if (table_inputs.size() == 0) {
		throw std::runtime_error("Input data of the truth table is empty!");
		return;
	}

	if (table_outputs.size() == 0) {
		throw std::runtime_error("Output data of the truth table is empty!");
		return;
	}

	// Determine the number of rows for the iteration
	int rows = table_inputs.size();

	// Genetic variables to store the values of the current input and output
	T input;
	T output;

	// Iterate over the number of rows
	for (int i = 0; i < rows; i++) {

		for (int j = 0; j < this->inputs; j++) {
			input = table_inputs.at(i).at(j);
			std::cout << input;
		}

		std::cout << "   ";

		for (int j = 0; j < this->outputs; j++) {
			output = table_outputs.at(i).at(j);
			std::cout << output;
		}

		std::cout << std::endl;
	}

}

/**
 * Prints the compressed input and output data chunk-wise.
 * Inputs and outputs are separated with whitespace.
 */
template<class T>
void BenchmarkFileReader<T>::print_compressed_data() {

	// The method is implemented in a similar fashion as the print_truth_table method.

	if (compressed_inputs.size() == 0) {
		throw std::runtime_error("No compressed input data available!");
		return;
	}

	if (compressed_outputs.size() == 0) {
		throw std::runtime_error("No compressed output data available!");
		return;
	}

	T input;
	T output;

	for (int i = 0; i < chunks; i++) {

		for (int j = 0; j < this->inputs; j++) {
			input = compressed_inputs.at(i).at(j);
			std::cout << input << " ";
		}

		std::cout << "   ";

		for (int j = 0; j < this->outputs; j++) {
			output = compressed_outputs.at(i).at(j);
			std::cout << output << " ";
		}

		std::cout << std::endl;
	}
}

/**
 * Validate the file which is stored at the given path by checking
 * the length of the file path and the extension of the file.
 */
template<class T>
void BenchmarkFileReader<T>::validate_file(std::string file_path,
		std::string format) {

	if (file_path.size() == 0) {
		throw std::runtime_error("File path is an empty string!");
	}

	std::string extension = std::filesystem::path(file_path).extension();

	if (extension != format) {
		throw std::runtime_error("Wrong file format!");
	}

}

/**
 * Reads the compressed data of the respective truth table chunk-wise with file streams
 * and stores it in the 2D vectors.
 *
 * The number inputs, outputs and chunks are obtained from the given PLU file.
 */
template<class T>
void BenchmarkFileReader<T>::read_plu_file(std::string file_path) {

	// First, validate the file path
	this->validate_file(file_path, ".plu");

	// Open the filestream then
	ifs.open(file_path, std::ifstream::in);

	// Continue only when the filestream could be opened
	if (ifs.is_open()) {

		char c;

		std::string str;

		T input;
		T output;

		// Skip the markers for the number of inputs, outputs and chunks (i, e and p)
		// and read the respective values.
		ifs >> str >> this->inputs;
		ifs >> str >> this->outputs;
		ifs >> str >> this->chunks;

		// Vectors for the temporary storage of the chunks
		std::vector<T> input_chunk;
		std::vector<T> output_chunk;

		// Iterate over the number of chunks
		for (int i = 0; i < this->chunks; i++) {

			// Check if any error state flags have been set
			if (!ifs.good()) {
				throw std::runtime_error("Error while reading PLU file!");
			}

			// Read the compressed inputs of a chunk and store
			// them in the temporary vector
			for (int j = 0; j < this->inputs; j++) {
				ifs >> input;
				input_chunk.push_back(input);
			}

			// Inputs and outputs are seperated with three whitespaces
			ifs.get(c);
			ifs.get(c);
			ifs.get(c);

			// Read and store the outputs of the current chunk.
			for (int j = 0; j < this->outputs; j++) {
				ifs >> output;
				output_chunk.push_back(output);
			}

			// Store the chunks in the 2D vectors
			this->compressed_inputs.push_back(input_chunk);
			this->compressed_outputs.push_back(output_chunk);

			input_chunk.clear();
			output_chunk.clear();
		}

	} else {
		throw std::runtime_error("Error opening PLU file!");
	}

}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_pla_file(std::string file_path) {

	// The method is implemented in a similar fashion as the read_plu_file method

	this->validate_file(file_path, ".pla");

	ifs.open(file_path, std::ifstream::in);

	if (ifs.is_open()) {

		char c;
		int rows;

		std::string str;
		T input;
		T output;

		std::vector<T> row_inputs;
		std::vector<T> row_outputs;

		std::vector<std::vector<T> > table_inputs;
		std::vector<std::vector<T> > table_outputs;

		ifs >> str >> this->inputs;
		ifs >> str >> this->outputs;

		rows = pow(2, this->inputs);

		for (int i = 0; i < rows; i++) {

			if (!ifs.good()) {
				throw std::runtime_error("Error while reading PLU file!");
			}

			for (int j = 0; j < this->inputs; j++) {
				ifs >> input;
				row_inputs.push_back(input);
			}

			ifs.get(c);
			ifs.get(c);
			ifs.get(c);

			for (int j = 0; j < this->outputs; j++) {
				ifs >> output;
				row_outputs.push_back(input);
			}

			table_inputs.push_back(row_inputs);
			table_outputs.push_back(row_outputs);

			row_inputs.clear();
			row_outputs.clear();
		}

		truth_table.first = table_inputs;
		truth_table.second = table_outputs;

	} else {
		throw std::runtime_error("Error opening PLU file!");
	}

}

/**
 * The Getters return a const reference to the compressed data or truth table.
 */

template<class T>
const std::vector<std::vector<T> >& BenchmarkFileReader<T>::getCompressedInputs() const {
	return compressed_inputs;
}

template<class T>
const std::vector<std::vector<T> >& BenchmarkFileReader<T>::getCompressedOutputs() const {
	return compressed_outputs;
}

template<class T>
const std::pair<std::vector<std::vector<T>>, std::vector<std::vector<T>> >& BenchmarkFileReader<
		T>::getTruthTable() const {
	return truth_table;
}

#endif /* BENCHMARKS_BOOL_BENCHMARKREADER_H_ */
