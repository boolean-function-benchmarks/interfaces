/*
 * TruthTable.h
 *
 *  Created on: 20.12.2022
 *      Author: roman
 */

#ifndef TRUTHTABLE_H_
#define TRUTHTABLE_H_

#include <vector>

template<class T>
class TruthTable {
private:
	std::vector<std::vector<T>> inputs;
	std::vector<std::vector<T>> outputs;
public:
	TruthTable();
	virtual ~TruthTable();
};

#endif /* TRUTHTABLE_H_ */
