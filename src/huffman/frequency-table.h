#ifndef FREQUENCYTABLE_H
#define FREQUENCYTABLE_H
#include <iomanip>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <vector>

namespace data {
	template<typename T>
	class FrequencyTable {
		std::map<T, unsigned> freqTable;



	public:
		void increment(const T& x)
		{
			++freqTable[x];
		}

		int operator[](const T& index)
		{
			unsigned result = 0;
			if (freqTable.contains(index)) {
				result = freqTable[index];
			}
			return result;
		}

		std::vector<T> values() {
			std::vector<T> keys;
			for (auto const& it: freqTable) {
				keys.push_back(it.first);
			}
			return keys;
		}

		std::vector<std::pair<T, unsigned>> pairs()
		{
			std::vector<std::pair<T, unsigned>> pairs;
			for (auto const& it : freqTable) {
				pairs.push_back(std::pair<T, unsigned>(it.first, it.second));
			}
			return pairs;
		}
	};

	template<typename T>
	FrequencyTable<T> count_frequencies(std::vector<T> range) {
		FrequencyTable<T> freqTable;
		for (T item : range) {
			freqTable.increment(item);
		}
		return freqTable;
	};

}

#endif