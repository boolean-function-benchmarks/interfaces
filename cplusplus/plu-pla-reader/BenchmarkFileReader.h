/*
 * BenchmarkReader.h
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

template<class T>
void BenchmarkFileReader<T>::print_truth_table() {

	std::vector<std::vector<T> > table_inputs = this->truth_table.first;
	std::vector<std::vector<T> > table_outputs = this->truth_table.second;

	if (table_inputs.size() == 0) {
		throw std::runtime_error("Input data of the truth table is empty!");
		return;
	}

	if (table_outputs.size() == 0) {
		throw std::runtime_error("Output data of the truth table is empty!");
		return;
	}

	int rows = table_inputs.size();

	T input;
	T output;

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

template<class T>
void BenchmarkFileReader<T>::print_compressed_data() {

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

template<class T>
void BenchmarkFileReader<T>::read_plu_file(std::string file_path) {

	this->validate_file(file_path, ".plu");

	ifs.open(file_path, std::ifstream::in);

	if (ifs.is_open()) {

		char c;

		std::string str;
		T input;
		T output;

		ifs >> str >> this->inputs;
		ifs >> str >> this->outputs;
		ifs >> str >> this->chunks;

		std::vector<T> input_chunk;
		std::vector<T> output_chunk;

		for (int i = 0; i < this->chunks; i++) {

			for (int j = 0; j < this->inputs; j++) {
				ifs >> input;
				input_chunk.push_back(input);
			}

			ifs.get(c);
			ifs.get(c);
			ifs.get(c);

			for (int j = 0; j < this->outputs; j++) {
				ifs >> output;
				output_chunk.push_back(output);
			}

			compressed_inputs.push_back(input_chunk);
			compressed_outputs.push_back(output_chunk);

			input_chunk.clear();
			output_chunk.clear();
		}

		if (!ifs.good()) {
			throw std::runtime_error("Error while reading PLU file!");
		}

		ifs.close();

	} else {
		throw std::runtime_error("Error opening PLU file!");
	}

}

template<class T>
void BenchmarkFileReader<T>::read_pla_file(std::string file_path) {

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
	}

}

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
