#include "encoding.h"

namespace
{
	class BitGrouperImplementation : public encoding::EncodingImplementation
	{
	private:
		u64 _group_size;

	public:
		BitGrouperImplementation(u64 group_size): _group_size(group_size) { }

		void encode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			while (!inputStream.end_reached())
			{
				u64 data = io::read_bits(_group_size, inputStream);
				outputStream.write(data);
			}
		}

		void decode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			while (!inputStream.end_reached())
			{
				u64 data = inputStream.read();
				io::write_bits(data, _group_size, outputStream);
			}
		}
	};

}

std::shared_ptr<encoding::EncodingImplementation> create_bit_grouper_implementation(u64 group_size)
{
	return std::make_shared<BitGrouperImplementation>(group_size);
}
