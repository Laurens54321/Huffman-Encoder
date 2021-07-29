

#ifndef BINARYIO_CPP
#define BINARYIO_CPP
#include "binary-io.h"


namespace io {	
	u64 read_bits(unsigned nbits, io::InputStream& input) {
		 u64 returnBits = input.read() << nbits;
		 return returnBits;
	}
	
	void write_bits(u64 value, unsigned nbits, io::OutputStream& output)
	{
		output.write(value & (1 << nbits) >> nbits);
	}

	
}

#endif // !BINARYIO_CPP
