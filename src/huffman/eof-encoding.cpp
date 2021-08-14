#include "encoding.h"
#include "eof-encoding.h"

#include <cassert>

#include "util.h"



namespace {
	class EofEncodingImplementation : public encoding::EncodingImplementation
	{
	private:
		const u64 _domain_size;
	public:
		EofEncodingImplementation(const u64 domain_size) : EncodingImplementation(), _domain_size(domain_size) { }

		void encode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			while(!inputStream.end_reached())
			{
				outputStream.write(inputStream.read());
			}
			outputStream.write(_domain_size + 1);
		}

		void decode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			assert(!inputStream.end_reached());
			while (!inputStream.end_reached())
			{
				Datum input = inputStream.read();
				if (input == _domain_size + 1) break;
				outputStream.write(input);
			}
		}
	};
}

std::shared_ptr<encoding::EncodingImplementation> create_eof_implementation(u64 domain_size)
{
	return std::make_shared<EofEncodingImplementation>(domain_size);
}

