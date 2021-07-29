#include "encoding.h"
#ifdef TEST_BUILD
#define CATCH_CONFIG_MAIN
/*
* This file is only active in testing build.
* Its purpose is to initialize the testing framework.
* 
* You can safely ignore this file; no changes will have to be made to it.
* Make sure this file remains included in the project though: without it, tests will not be able to run.
*/


#include "catch.hpp"
#include "iostream"
#include "util.h"
#include "frequency-table.h"
#include "binary-tree.h"
#include "io/memory-buffer.h"
#include "io/streams.h"
#include "eof-encoding.h"
#include "bit-grouper.h"
#include "encoding.h"
#include "encoding-combiner.h"

TEST_CASE("Passing test")
{
    REQUIRE(1 == 1);
}

TEST_CASE("Bits_Needed test")
{
    REQUIRE(bits_needed(2) == 1);
    REQUIRE(bits_needed(3) == 2);
    REQUIRE(bits_needed(4) == 2);
    REQUIRE(bits_needed(5) == 3);
    REQUIRE(bits_needed(6) == 3);
    REQUIRE(bits_needed(7) == 3);
    REQUIRE(bits_needed(8) == 3);
    REQUIRE(bits_needed(9) == 4);
    REQUIRE(bits_needed(128) == 7);
    REQUIRE(bits_needed(129) == 8);
    REQUIRE(bits_needed(1000000) == 20);


}

TEST_CASE("Bytes_needed test")
{
    REQUIRE(bytes_needed(2) == 1);
    REQUIRE(bytes_needed(3) == 1);
    REQUIRE(bytes_needed(128) == 1);
    REQUIRE(bytes_needed(129) == 1);
    REQUIRE(bytes_needed(255) == 1);
    REQUIRE(bytes_needed(256) == 1);
    REQUIRE(bytes_needed(1000000) == 3);
}

TEST_CASE("FrequencyTable testing") {
    data::FrequencyTable<std::string> myfreq;
    myfreq.increment("a");
    REQUIRE(myfreq["a"] == 1);
    myfreq.increment("a");
    REQUIRE(myfreq["a"] == 2);
    REQUIRE(myfreq["b"] == 0);
    myfreq.increment("b");
    REQUIRE(myfreq["b"] == 1);

    REQUIRE(myfreq.values().size() == 2);
    std::cout << myfreq.values().size();


    data::FrequencyTable<std::string> newfreq;
    //data::count_frequencies<std::string>(std::vector({ "a", "b" }));
    newfreq.increment("a");
    REQUIRE(newfreq["a"] == 1);
}

TEST_CASE("Binary Tree testsing") {
    data::Leaf<int> myleaf(0);
    REQUIRE(myleaf.getValue() == 0);


}

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

TEST_CASE("Memory buffer")
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

TEST_CASE("2 buffers | 3 values in | should end up at 4")
{
	io::MemoryBuffer<20> buffer1_20_encode;
	buffer1_20_encode.data()->push_back(0);
	buffer1_20_encode.data()->push_back(1);
	buffer1_20_encode.data()->push_back(2);
	io::MemoryBuffer<21> buffer2_21_encode;
	const auto test = encoding::eof_encoding<20>();
    encoding::encode(buffer1_20_encode.source(), buffer2_21_encode.destination(), test);
	REQUIRE(buffer1_20_encode.data()->size() == buffer2_21_encode.data()->size() - 1);
}

TEST_CASE("2 buffers | 4 bits in | should end up at 11")
{
    io::MemoryBuffer<2> bufferin;
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(0);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    io::MemoryBuffer<16> bufferout;
    const auto test = encoding::bit_grouper<4>();
    encoding::encode(bufferin.source(), bufferout.destination(), test);
    //REQUIRE(bufferout.data()->size() == 1);
    auto c = io::read_bits(4,*bufferout.source()->create_input_stream());
    REQUIRE(c == 11);
	
}

TEST_CASE("Combiner test")
{
    encoding::Encoding<2, 8> grouper = encoding::bit_grouper<3>();
    encoding::Encoding<8, 9> eof = encoding::eof_encoding<8>();
	

    auto combined = grouper | eof ;

    io::MemoryBuffer<2> bufferin;
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(0);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(0);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    io::MemoryBuffer<9> bufferout;

	
    encoding::encode(bufferin.source(), bufferout.destination(), *combined);
    //REQUIRE(bufferout.data()->size() == 1);
    auto c = io::read_bits(9, *bufferout.source()->create_input_stream());
    REQUIRE(c == 11);
}



#endif
