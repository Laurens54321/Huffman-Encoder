#ifndef ENCODING_H
#define ENCODING_H


#include <binary-io.h>
#include <iostream>
#include <ostream>

#include "memory"
#include "io/data-endpoints.h"


namespace encoding
{
	class EncodingImplementation{
	public:
		// ~EncodingImplementation();
	
		virtual void encode(io::InputStream& inputStream, io::OutputStream& outputStream) { }
		virtual void decode(io::InputStream& inputStream, io::OutputStream& outputStream) { }
	};

	template<u64 IN, u64 OUT>
	class Encoding {
	private:
		std::shared_ptr<EncodingImplementation> _encodingImplementation;
	public:
		Encoding(std::shared_ptr<EncodingImplementation> encodingImplementation) : _encodingImplementation(encodingImplementation) {}

		std::shared_ptr<EncodingImplementation> operator -> ()
		{
			return _encodingImplementation;
		}

		const std::shared_ptr<EncodingImplementation> operator -> () const
		{
			return _encodingImplementation;
		}
	};

	template<u64 IN, u64 OUT>
	void encode(io::DataSource<IN> data_source, io::DataDestination<OUT> data_destination, Encoding<IN, OUT> encoding)
	{
		auto input_stream = data_source->create_input_stream();
		auto output_stream = data_destination->create_output_stream();
		
		encoding->encode(*input_stream, *output_stream);
	}

	template<u64 IN, u64 OUT>
	void decode(io::DataSource<IN> data_source, io::DataDestination<OUT> data_destination, Encoding<OUT, IN> decoding)
	{
		io::InputStream& input_stream = data_source->create_input_stream();
		io::OutputStream& output_stream = data_destination->create_output_stream();

		decoding->decode(input_stream, output_stream);
	}
}



#endif



