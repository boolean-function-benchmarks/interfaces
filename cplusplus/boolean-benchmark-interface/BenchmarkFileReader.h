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
#include <cstring>

#include "TruthTable.h"
#include "Minterm.h"

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
 *  @author Roman Kalkreuth,
 *         	https://orcid.org/0000-0003-1449-5131,
 *          https://www.researchgate.net/profile/Roman-Kalkreuth,
 *         	https://twitter.com/RomanKalkreuth
 *
 */
template<class T>
class BenchmarkFileReader {
private:

	std::ifstream ifs;
	std::string line;

	TruthTable<T> table;

	int num_inputs;
	int num_outputs;
	int num_chunks;
	int num_product_terms;

	int header_size;

	std::string model_name;

	const int PLU = 0;
	const int PLA = 1;
	const int TT = 2;

	void validate_file(std::string file_path);
	int file_format(std::string file_path);

public:
	BenchmarkFileReader();
	~BenchmarkFileReader() = default;
	void open_file(std::string file_path);
	void close_file();
	void read_file(std::string file_path);
	void read_header();
	void read_tt_file(std::string file_path);
	void read_pla_file(std::string file_path);
	void read_plu_file(std::string file_path);
	std::string read_keyword(std::string keyword);
	void read_model_name();
	void read_num_inputs();
	void read_num_outputs();
	void read_num_product_terms();
	void read_names(std::string keyword, std::vector<std::string> &names);
	void read_input_names();
	void read_output_names();
	void print_compressed_data();
	void print_truth_table();
	void print_header();

	const std::vector<std::vector<T> >& get_compressed_inputs() const;
	const std::vector<std::vector<T> >& get_compressed_outputs() const;

	const TruthTable<T>& get_truth_table() const;

	std::vector<std::vector<char>>* generate_input_table();

};

template<class T>
BenchmarkFileReader<T>::BenchmarkFileReader() {
	this->num_inputs = -1;
	this->num_outputs = -1;
	this->num_chunks = -1;
	this->num_product_terms = -1;
	this->model_name = "";
}

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
 *
 */
template<class T>
const TruthTable<T>& BenchmarkFileReader<T>::get_truth_table() const {
	return this->table;
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

	// File has to be either an plu, pla or tt file
	if (extension != ".plu" && extension != ".pla" && extension != ".tt") {
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
/**
 * @param
 */
template<class T>
void BenchmarkFileReader<T>::open_file(std::string file_path) {

	// First, validate the file path
	this->validate_file(file_path);

	// Open the filestream then
	ifs.open(file_path, std::ifstream::in);

	if (!ifs.is_open()) {
		throw std::runtime_error("Cannot open benchmark file!");
	}
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::close_file() {
	ifs.close();
}

template<class T>
void BenchmarkFileReader<T>::read_file(std::string file_path) {

	this->open_file(file_path);

	if (ifs.is_open()) {
		this->read_header();

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
	} else {
		throw std::runtime_error("Benchmark file is not open!");
	}
}

/**
 * @param
 * @return
 */
template<class T>
std::string BenchmarkFileReader<T>::read_keyword(std::string keyword) {

	if (ifs.is_open()) {
		const char separator = ' ';
		std::vector<std::string> vec;

		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		std::string line;

		while (std::getline(ifs, line)) {
			if (line.find(keyword) != std::string::npos) {

				std::stringstream sstream(line);
				std::string s;

				while (std::getline(sstream, s, separator)) {
					vec.push_back(s);
				}

				break;
			}
		}

		if (vec.size() != 2) {
			return "";
		} else {
			return vec.at(1);
		}

	} else {
		throw std::runtime_error("Benchmark file is not open!");
	}
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_model_name() {
	this->model_name = this->read_keyword(".model");
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_num_inputs() {

	std::string s;
	s = this->read_keyword(".i");
	this->num_inputs = (s != "") ? std::stoi(s) : -1;
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_num_outputs() {

	std::string s;
	s = this->read_keyword(".o");
	this->num_outputs = (s != "") ? std::stoi(s) : -1;
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_num_product_terms() {

	std::string s;
	s = this->read_keyword(".p");
	this->num_product_terms = (s != "") ? std::stoi(s) : -1;
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_names(std::string keyword,
		std::vector<std::string> &names) {
	if (ifs.is_open()) {
		const char separator = ' ';

		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		std::string line;

		while (std::getline(ifs, line)) {
			if (line.find(keyword) != std::string::npos) {

				std::stringstream sstream(line);
				std::string s;

				while (std::getline(sstream, s, separator)) {
					names.push_back(s);
				}
			}
		}
		names.erase(names.begin());

	} else {
		throw std::runtime_error("Benchmark file is not open!");
	}
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_input_names() {

	std::vector<std::string> &input_names = this->table.get_input_names();
	input_names.clear();

	read_names(".ilb", input_names);
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_output_names() {

	std::vector<std::string> &output_names = this->table.get_output_names();
	output_names.clear();

	read_names(".ob", output_names);

}

template<class T>
void BenchmarkFileReader<T>::print_header() {

	std::vector<std::string> &input_names = this->table.get_input_names();
	std::vector<std::string> &output_names = this->table.get_output_names();

	if (this->model_name.length() > 0) {
		std::cout << "Model: " << this->model_name << std::endl;
	}

	if (this->num_inputs != -1) {
		std::cout << "Inputs: " << this->num_inputs << std::endl;
	}

	if (this->num_outputs != -1) {
		std::cout << "Outputs: " << this->num_outputs << std::endl;
	}

	if (this->num_product_terms != -1) {
		std::cout << "Product terms: " << this->num_product_terms << std::endl;
	}

	if (input_names.size() > 0) {
		std::cout << "Input names: ";
		this->table.print_input_names();
		std::cout << std::endl;
	}

	if (output_names.size() > 0) {
		std::cout << "Output names: ";
		this->table.print_output_names();
		std::cout << std::endl;
	}

}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_header() {

	this->header_size = 0;

	this->read_model_name();

	if (this->model_name.length() > 0) {
		this->header_size++;
	}

	this->read_num_inputs();

	if (this->num_inputs != -1) {
		this->header_size++;
	}

	this->read_num_outputs();

	if (this->num_outputs != -1) {
		this->header_size++;
	}

	this->read_num_product_terms();

	if (this->num_product_terms != -1) {
		this->header_size++;
	}

	std::vector<std::string> &input_names = this->table.get_input_names();
	std::vector<std::string> &output_names = this->table.get_output_names();

	this->read_input_names();

	if (input_names.size() > 0) {
		this->header_size++;
	}

	this->read_output_names();

	if (output_names.size() > 0) {
		this->header_size++;
	}

}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_tt_file(std::string file_path) {

	// Continue only when the filestream could be opened
	if (!ifs.is_open()) {
		this->open_file(file_path);
	}

	// Continue only when the filestream could be opened
	if (ifs.is_open()) {

		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		int rows;
		std::string line;
		std::string s;
		char *term;

		for (int i = 0; i < this->header_size; i++) {
			std::getline(ifs, line);
		}

		int offset = this->num_inputs + 1;

		// Vectors for the data of each row
		std::vector<T> row_inputs;
		std::vector<T> row_outputs;

		// Clear the table from potential previous data, reset the compressed status
		table.reset();

		rows = std::pow(2, this->num_inputs);
		this->table.set_compressed(false);

		// Iterate over the number of rows
		for (int i = 0; i < rows; i++) {

			std::getline(ifs, line);

			// Check if any error state flags have been set
			if (ifs.fail() || ifs.eof() || ifs.bad()) {
				throw std::runtime_error("Error while reading TT file!");
			}

			for (int j = 0; j < this->num_inputs; j++) {
				s = line.at(j);
				row_inputs.push_back(std::stoi(s));
			}

			for (int j = 0; j < this->num_outputs; j++) {
				s = line.at(offset + j);
				row_outputs.push_back(std::stoi(s));
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

/**
 *
 */
template<class T>
std::vector<std::vector<char>>* BenchmarkFileReader<T>::generate_input_table() {

	int n;
	int x = 0;
	int num_rows = std::pow(2.0, this->num_inputs);

	std::vector<std::vector<char>> *input_table = new std::vector<
			std::vector<char> >(num_rows,
			std::vector<char>(this->num_inputs, '0'));

	for (int i = this->num_inputs - 1; i >= 0; i--) {
		n = std::pow(2.0, x);
		for (int j = 0; j < num_rows; j++) {
			if (j % (n * 2) >= n) {
				(*input_table)[j][i] = '1';
			}
		}
		x++;
	}
	return input_table;
}

/**
 *
 */
template<class T>
void BenchmarkFileReader<T>::read_pla_file(std::string file_path) {

	// Continue only when the filestream could be opened
	if (!ifs.is_open()) {
		this->open_file(file_path);
	}

	// Continue only when the filestream could be opened
	if (ifs.is_open()) {

		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		std::string line;
		std::vector<char> minterm(this->num_inputs);
		int output_index;

		std::vector<Minterm*> minterms;

		for (int i = 0; i < this->header_size; i++) {
			std::getline(ifs, line);
		}

		for (int i = 0; i < this->num_product_terms; i++) {

			std::getline(ifs, line);

			// Check if any error state flags have been set
			if (ifs.fail() || ifs.eof() || ifs.bad()) {
				throw std::runtime_error("Error while reading TT file!");
			}

			for (int j = 0; j < this->num_inputs; j++) {
				minterm.at(j) = line.at(j);
			}

			for (int j = 0; j < this->num_outputs; j++) {
				char c = line.at(this->num_inputs + 1 + j);
				if (c == '1') {
					output_index = j;
				}
			}

			Minterm *term = new Minterm(this->num_inputs, output_index);

			term->set_term(&minterm);
			minterms.push_back(term);
		}

		std::vector<std::vector<char>> *input_table = generate_input_table();
		int num_rows = std::pow(2.0, this->num_inputs);

		this->table.generate_inputs(this->num_inputs);
		this->table.init_outputs(this->num_outputs, num_rows);

		/*
		for (int i = 0; i < num_rows; i++) {
			std::vector<char> input_vec = input_table->at(i);

			for (int j = 0; j < this->num_product_terms; j++) {
				bool match = minterms[j]->match(&input_vec);

				if (match) {
					int output_index = minterms[j]->get_output_index();
					table.set_output_at(i, output_index, 1);
				}
			}
		}*/

		for (int j = 0; j < this->num_product_terms; j++) {
			for (int i = 0; i < num_rows; i++) {
				std::vector<char> input_vec = input_table->at(i);
				bool match = minterms[j]->match(&input_vec);

				if (match) {
					int output_index = minterms[j]->get_output_index();
					table.set_output_at(i, output_index, 1);
				}
			}
		}

		delete input_table;
	}

}

/**
 * @brief Reads and stores compressed data of a respective truth table
 *
 * @details Uses file input stream. The data is stored in the 2D vectors of the truth table object.
 * The number inputs, outputs and chunks are obtained from the given benchmark file.
 *
 * @see TruthTable
 *
 * @param file_path Given path for the benchmark file
 */
template<class T>
void BenchmarkFileReader<T>::read_plu_file(std::string file_path) {

// First, validate the file path
this->validate_file(file_path);

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

	rows = this->num_chunks;
	this->table.set_compressed(true);

	// Iterate over the number of chunks
	for (int i = 0; i < rows; i++) {

		// Check if any error state flags have been set
		if (!ifs.good()) {
			throw std::runtime_error("Error while reading PLU file!");
		}

		// Read the compressed inputs of the current row and store
		// them in the temporary vector
		for (int j = 0; j < this->num_inputs; j++) {
			ifs >> input;
			row_inputs.push_back(input);
		}

		// Inputs and outputs are seperated with whitespaces
		// Therefore, we skip this whitespace
		do {
			ifs.get(c);
		} while (ifs.peek() == ' ');

		// Read and store the outputs of the current row
		for (int j = 0; j < this->num_outputs; j++) {
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
