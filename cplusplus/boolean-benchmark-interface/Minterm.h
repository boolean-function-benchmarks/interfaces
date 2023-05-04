/*
 *
 */
#ifndef MINTERM_H_
#define MINTERM_H_

#include <vector>

class Minterm {
private:
	std::vector<char> *term;
	int output_index;
	int num_inputs;
public:
	Minterm(int p_num_inputs, int p_output_index);
	virtual ~Minterm();
	void set_term(const std::vector<char> *p_term);
	int get_output_index() const;
	bool match(const std::vector<char> *p_term);
};

Minterm::Minterm(int p_num_inputs, int p_output_index) {
	this->num_inputs = p_num_inputs;
	this->output_index = p_output_index;
	this->term = new std::vector<char>(this->num_inputs);
}

Minterm::~Minterm() {
	delete this->term;
}

void Minterm::set_term(const std::vector<char> *p_term) {
	for (int i = 0; i < this->num_inputs; i++) {
		this->term->at(i) = p_term->at(i);
	}
}

int Minterm::get_output_index() const {
	return this->output_index;
}

bool Minterm::match(const std::vector<char> *p_term) {
	bool state = true;
	for (int i = 0; i < this->num_inputs; i++) {
		if (this->term->at(i) != '-') {
			if (this->term->at(i) != p_term->at(i)) {
				state = false;
			}
		}
	}

	if (state == true) {
		for (int i = 0; i < this->num_inputs; i++) {
			std::cout << this->term->at(i);
		}

		std::cout << "   ";

		for (int i = 0; i < this->num_inputs; i++) {
			std::cout << p_term->at(i);
		}

		std::cout << "   " << this->output_index;

		std::cout<< std::endl;
	} else {

	}

	return state;
}

#endif /* MINTERM_H_ */
