#include <io/streams.h>
#ifndef BINARYIO_H

#include "util.h"

namespace io {
    	u64 read_bits(unsigned nbits, io::InputStream& input);
    void write_bits(u64 value, unsigned nbits, io::OutputStream& output);

}



#endif // !BINARY_IO