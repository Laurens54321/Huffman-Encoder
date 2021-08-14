 #include <string>
#ifndef BINARYIO_CPP
#define BINARYIO_CPP
#include "binary-io.h"


namespace io {	
	u64 read_bits(unsigned nbits, io::InputStream& input) {
		std::string num;
		for (u64 i = 0; i < nbits; i++)
		{
			if (input.end_reached())
				num += std::to_string(0);
			else
				num += std::to_string(input.read());
		}
		int value = 0;
		int base = 1;

		const auto len = num.length();

		for (size_t i = len; i-- > 0; )
		{
			if (num[i] == '1')
				value += base;
			base = base * 2;
		}
		return value;
	}
	
	void write_bits(u64 value, unsigned nbits, io::OutputStream& output)
	{
		for (u64 j = nbits; j-- > 0; )
		{
			const auto k = value >> j;
			if (k & 1)
				output.write(1);
			else
				output.write(0);
		}
	}

	
}

#endif // !BINARYIO_CPP
