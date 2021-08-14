#ifdef TEST_BUILD

#include "binary-io.h"
#include "bit-grouper.h"
#include "catch.hpp"
#include "encoding.h"
#include "io/memory-buffer.h"


void groupBits(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, uint8_t result) {
    io::MemoryBuffer<2> bufferin;
    bufferin.data()->push_back(bit0);
    bufferin.data()->push_back(bit1);
    bufferin.data()->push_back(bit2);
    bufferin.data()->push_back(bit3);
    io::MemoryBuffer<16> bufferout;
    const auto test = encoding::bit_grouper<4>();
    encoding::encode(bufferin.source(), bufferout.destination(), test);
    REQUIRE(bufferout.data().get()->at(0) == result);           //RESULT
}

TEST_CASE("BIT GROUPER TEST") {
    groupBits(1, 0, 0, 1, 9);
    groupBits(1, 1, 0, 1, 13);
    groupBits(1, 1, 0, 0, 12);
    groupBits(0, 1, 1, 0, 6);
    groupBits(1, 1, 1, 1, 15);
    groupBits(1, 0, 1, 1, 11);
}

TEST_CASE("Bit grouper test 2 ")
{
    io::MemoryBuffer<2> _in;
    io::MemoryBuffer<1 << 2> _out;

    _in.data().reset();
    _in.data()->push_back(1);
    _in.data()->push_back(0);
    _in.data()->push_back(0);
    _in.data()->push_back(0);

    encoding::encode<2, 1 << 2>(_in.source(), _out.destination(), encoding::bit_grouper<2>());

    REQUIRE(_out.data()->front() == 2);


}



#endif
