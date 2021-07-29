#ifndef ENCODINGCOMBINER_H
#define ENCODINGCOMBINER_H
#include "util.h"
#include "encoding.h"
#include "MemoryBuffer.h"
#include "io/memory-buffer.h"

namespace encoding
{
	template<u64 N1, u64 N2, u64 N3>
	class EncodingCombinerImplementation : public encoding::EncodingImplementation
	{
	private:
		Encoding<N1, N2> E1;
		Encoding<N2, N3> E2;
	public:
		EncodingCombinerImplementation(Encoding<N1, N2>E1, Encoding<N2, N3> E2): E1(E1), E2(E2) { }

		void encode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			auto temp_buffer = io::MemoryBuffer<N1, u64>();
			auto ouputbuffer = temp_buffer.destination()->create_output_stream();
			auto inbuffer = temp_buffer.source()->create_input_stream();
			E1->encode(inputStream, *ouputbuffer);
			E2->encode(*inbuffer, outputStream);
		}

		void decode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			
		}
	};

	template<u64 N1, u64 N2, u64 N3>
	std::shared_ptr<encoding::Encoding<N1, N3>> operator | (Encoding<N1, N2>E1, Encoding<N2, N3> E2)
	{
		auto encodercombiner = std::make_shared<EncodingCombinerImplementation<N1, N2, N3>>(E1, E2);
		auto encoder = std::make_shared<Encoding<N1, N3>>(encodercombiner);
		return std::shared_ptr<Encoding<N1, N3>>(encoder);
	}
}

#endif
