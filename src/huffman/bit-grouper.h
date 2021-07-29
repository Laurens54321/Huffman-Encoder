#ifndef BITGROUPER_H
#define BITGROUPER_H
#include "encoding.h"
#include "util.h"

std::shared_ptr<encoding::EncodingImplementation> create_bit_grouper_implementation(u64 group_size);

namespace encoding
{
	template<unsigned GROUP_SIZE>
	Encoding<2, 1 << GROUP_SIZE> bit_grouper()
	{
		return create_bit_grouper_implementation(GROUP_SIZE);
	}
}




#endif
