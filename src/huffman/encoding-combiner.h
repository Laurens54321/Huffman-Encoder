#ifndef ENCODINGCOMBINER_H
#define ENCODINGCOMBINER_H
#include "util.h"
#include "encoding.h"
#include "io/memory-buffer.h"

namespace encoding
{
	template<u64 N1, u64 N2, u64 N3>
	class EncodingCombinerImplementation : public encoding::EncodingImplementation
	{
	private:
		const Encoding<N1, N2> E1;
		const Encoding<N2, N3> E2;
	public:
		EncodingCombinerImplementation(const Encoding<N1, N2>E1, const Encoding<N2, N3> E2): E1(E1), E2(E2) { }

		void encode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			auto temp_buffer = io::MemoryBuffer<N1, u64>();
			auto outputbuffer = temp_buffer.destination()->create_output_stream();
			auto inbuffer = temp_buffer.source()->create_input_stream();
			E1->encode(inputStream, *outputbuffer);
			E2->encode(*inbuffer, outputStream);
		}

		void decode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			auto temp_buffer = io::MemoryBuffer<N1, u64>();
			auto outputbuffer = temp_buffer.destination()->create_output_stream();
			auto inbuffer = temp_buffer.source()->create_input_stream();
			E2->decode(inputStream, *outputbuffer);
			E1->decode(*inbuffer, outputStream);
			
		}
	};

	template<u64 N1, u64 N2, u64 N3>
	encoding::Encoding<N1, N3> operator | (const Encoding<N1, N2>E1, const Encoding<N2, N3> E2)
	{
		auto encodercombiner = std::make_shared<EncodingCombinerImplementation<N1, N2, N3>>(E1, E2);
		//auto encoder = std::make_shared<Encoding<N1, N3>>();
		return Encoding<N1, N3>(encodercombiner);
	}
}

#endif
