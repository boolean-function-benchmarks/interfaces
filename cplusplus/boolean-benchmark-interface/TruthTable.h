#ifndef TRUTHTABLE_H_
#define TRUTHTABLE_H_

#include <vector>
#include <cassert>

/*
 * @brief Implements a truth table which stores the inputs and output
 *
 * @details The data is stored in two dimensional vectors of generic type.
 * This class can be used for compressed and uncompressed truth tables.
 * This is a generic class which uses templates.
 *
 * @tparam T Generic type for the input and output vectors.
 *
 */
template<class T>
class TruthTable {
private:
	std::vector<std::vector<T>> inputs;
	std::vector<std::vector<T>> outputs;

	bool compressed = false;
public:
	TruthTable() = default;
	virtual ~TruthTable() = default;

	void append_inputs(std::vector<T> input_vec);
	void append_outputs(std::vector<T> output_vec);

	const std::vector<T>& get_inputs_at(int index) const;
	const std::vector<T>& get_outputs_at(int index) const;

	const std::vector<std::vector<T>>& get_inputs() const;
	const std::vector<std::vector<T>>& get_outputs() const;

	bool is_compressed() const;
	void set_compressed(bool p_compressed);

	void clear();
	void print();
	void reset();
	int rows();

};

/**
 * @brief Appends an input row vector to the 2D input vector.
 *
 * @details Validates the input row vector by checking for nullptr and
 * emptiness.
 *
 * @param input_vec Input row vector
 */
template<class T>
void TruthTable<T>::append_inputs(std::vector<T> input_vec) {
	assert(&input_vec != nullptr && !input_vec.empty());
	this->inputs.push_back(input_vec);
}

/**
 * @brief Appends an output row vector to the 2D output vector.
 *
 * @details Validates the output row vector by checking for nullptr and
 * emptiness.
 *
 * @param output_vec Output row vector
 */
template<class T>
void TruthTable<T>::append_outputs(std::vector<T> output_vec) {
	assert(&output_vec != nullptr && !output_vec.empty());
	this->outputs.push_back(output_vec);
}

/**
 * @brief Returns the reference to an input row at a specific index of the 2D input vector.
 *
 * @details The index has to be in the interval 0 <= index <= max_index.
 *
 * @param index Index of the input row.
 *
 * @return Reference to an input row vector at the given index.
 */
template<class T>
const std::vector<T>& TruthTable<T>::get_inputs_at(int index) const {
	int max_index = this->inputs.size() - 1;
	assert((index >= 0) && (index <= max_index));
	return this->inputs.at(index);
}

/**
 * @brief Returns the reference to an output row at a specific index of the 2D output vector.
 *
 * @details  The index has to be in the interval 0 <= index <= max_index.
 *
 * @param index Index of the output row.
 *
 * @return Reference to an output row vector at the given index
 */
template<class T>
const std::vector<T>& TruthTable<T>::get_outputs_at(int index) const {
	int max_index = this->outputs.size() - 1;
	assert((index >= 0) && (index <= max_index));
	return this->outputs.at(index);
}

/**
 * @brief Returns a reference to the 2D output vector.
 *
 * @return 2D input vector reference
 */
template<class T>
const std::vector<std::vector<T>>& TruthTable<T>::get_inputs() const {
	return inputs;
}

/**
 * @brief Returns a reference to the 2D output vector.
 *
 * @return 2D output vector reference
 */
template<class T>
const std::vector<std::vector<T>>& TruthTable<T>::get_outputs() const {
	return outputs;
}

/**
 * @brief Clears the input and output vector..
 */
template<class T>
void TruthTable<T>::clear() {
	this->inputs.clear();
	this->outputs.clear();
}

/**
 * @brief Returns the number of rows of the table.
 *
 * @return Number of rows.
 */
template<class T>
int TruthTable<T>::rows() {
	return this->inputs.size();
}

/**
 * @brief Reset the table by clearing and resetting the state
 * of the compressed property.
 */
template<class T>
void TruthTable<T>::reset() {
	this->clear();
	this->compressed = false;
}

/**
 * @brief Print the table row-wise.
 *
 * @details Validates the dimensions of the input and output vectors before printing.
 */
template<class T>
void TruthTable<T>::print() {

	// Check whether the input vector contain any data
	if (this->inputs.size() == 0) {
		throw std::runtime_error("Input data of the truth table is empty!");
		return;
	}

	// Check whether the output vector contain any data
	if (this->outputs.size() == 0) {
		throw std::runtime_error("Output data of the truth table is empty!");
		return;
	}

	// Check whether the dimensions of the input and output data are equal
	if (this->outputs.size() != this->outputs.size()) {
		throw std::runtime_error("Number of output and input rows are not equal!");
		return;
	}

	// Determine the number of rows for the iteration
	int rows = this->rows();

	// Genetic variables to store the values of the current input and output
	T input;
	T output;

	int num_inputs = this->inputs.at(0).size();
	int num_outputs = this->outputs.at(0).size();

	// Iterate over the number of rows
	for (int i = 0; i < rows; i++) {

		// Print the data of the current input row vector
		for (int j = 0; j < num_inputs; j++) {
			input = this->inputs.at(i).at(j);
			std::cout << input << " ";
		}

		std::cout << "   ";

		// Print the data of the current output row vector
		for (int j = 0; j < num_outputs; j++) {
			output = this->outputs.at(i).at(j);
			std::cout << output << " ";
		}

		std::cout << std::endl;
	}
}

/**
 * @brief Returns the state of the compressed property.
 *
 * @return State of the compressed property.
 */
template<class T>
bool TruthTable<T>::is_compressed() const {
	return compressed;
}

/**
 * @brief Sets the state of the compressed property.
 *
 * @param p_compressed New state of the compressed property.
 */
template<class T>
void TruthTable<T>::set_compressed(bool p_compressed) {
	this->compressed = p_compressed;
}

#endif /* TRUTHTABLE_H_ */
