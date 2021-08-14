#include "catch.hpp"
#include "encoding.h"
#include "eof-encoding.h"
#include "io/memory-buffer.h"
#ifdef TEST_BUILD







void check(u64 n, unsigned nbits)
{
    io::MemoryBuffer<2> buffer;
    auto input = buffer.source()->create_input_stream();
    auto output = buffer.destination()->create_output_stream();
    io::write_bits(n, nbits, *output);
    auto result = io::read_bits(nbits, *input);

    REQUIRE(n == result);
}

TEST_CASE("inputoutputstream")
{
    check(55, 6);
    check(63, 6);
    check(111, 7);
    check(127, 7);
    check(2, 2);
}

TEST_CASE("Memory buffer stream")
{
    auto original_data = std::make_shared<std::vector<Datum>>();
    original_data->push_back(0);
    original_data->push_back(1);
    original_data->push_back(2);
    original_data->push_back(3);

    auto input_stream = io::create_memory_input_stream(original_data);

    auto data_receiver = std::make_shared<std::vector<Datum>>();
    auto output_stream = io::create_memory_output_stream(data_receiver);

	
}

TEST_CASE("EOF Encoder")
{
    io::MemoryBuffer<10> inbuffer;
    auto data = inbuffer.destination()->create_output_stream();
    data->write(1);
    data->write(2);
    data->write(3);
    data->write(4);
    data->write(5);
    data->write(6);
    data->write(7);
    data->write(8);
    data->write(9);
    data->write(10);




    io::MemoryBuffer<11> outbuffer;

    auto source = inbuffer.source();
    auto destination = outbuffer.destination();

    auto encoding = encoding::eof_encoding<10>();

    encoding::encode<10, 11>(source, destination, encoding);
}


#endif
