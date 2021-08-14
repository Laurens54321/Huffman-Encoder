#ifdef TEST_BUILD

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
#include "tree-encoding.h"
#include "huffman-encoding.h"
#include <binary-tree.h>




TEST_CASE("EOF 2 buffers | 3 values in | should end up at 4")
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

TEST_CASE("Combiner test")
{
    encoding::Encoding<2, 8> grouper = encoding::bit_grouper<3>();
    encoding::Encoding<8, 9> eof = encoding::eof_encoding<8>();


    auto combined = grouper | eof;

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


    encoding::encode(bufferin.source(), bufferout.destination(), combined);
    //REQUIRE(bufferout.data()->size() == 1);
    auto c = io::read_bits(9, *bufferout.source()->create_input_stream());
    //REQUIRE(c == 11);
}

TEST_CASE("Tree encoding test") {
    auto leftleaf = std::make_unique<data::Leaf<Datum>>(01);
    auto middleleaf = std::make_unique<data::Leaf<Datum>>(11);
    auto rightleaf = std::make_unique<data::Leaf<Datum>>(10);
    auto rightBranch = std::make_unique<data::Branch<Datum>>(std::move(middleleaf), std::move(rightleaf));
    const auto root = data::Branch<Datum>(std::move(leftleaf), std::move(rightBranch));
    REQUIRE(!root.isLeaf());
    auto memorybuffer = io::MemoryBuffer<2, Datum>();
    encoding::huffman::encode_tree(root, 2, *memorybuffer.destination()->create_output_stream());
    auto outputnode = encoding::huffman::decode_tree(2, *memorybuffer.source()->create_input_stream());
    //auto branch = std::dynamic_pointer_cast<data::Branch<Datum>, data::Node<Datum>>(outputnode);
    //REQUIRE(!branch->isLeaf());
}

TEST_CASE("Huffman Encoder test")
{

    auto huffman = encoding::huffman::huffman_encoding<8>();

    io::MemoryBuffer<8> bufferin;
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(0);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(0);
    bufferin.data()->push_back(1);
    bufferin.data()->push_back(1);
    io::MemoryBuffer<2> bufferout;


    encoding::encode(bufferin.source(), bufferout.destination(), huffman);
	
    auto c = io::read_bits(8, *bufferout.source()->create_input_stream());
    //REQUIRE(c == 11);
}

void EncodeHuffman(std::string text)
{
    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;

    io::MemoryBuffer<256> original;
    std::string ha = text;
    for (auto letter : ha)
    {
        original.data()->push_back(letter);
    }
    io::MemoryBuffer<256> temp;
    io::MemoryBuffer<256> decomp;

    encoding::encode(original.source(), temp.destination(), combined);

    REQUIRE(*original.data() != *temp.data());
}

void EncodeDecodeHuffman(std::string text)
{
    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;

    io::MemoryBuffer<256> original;
    std::string ha = text;
    for (auto letter : ha)
    {
        original.data()->push_back(letter);
    }
    io::MemoryBuffer<256> temp;
    io::MemoryBuffer<256> decomp;

    encoding::encode(original.source(), temp.destination(), combined);

    encoding::decode(temp.source(), decomp.destination(), combined);

    REQUIRE(*original.data() == *decomp.data());
}


TEST_CASE("Encode with huffman") 
{
    EncodeHuffman("aaaaaaaaaaaadddddddddddddjjjjjjj");
    EncodeHuffman("mqsldkjfmslqdkjfosikdfjoekkkkddd");
    EncodeHuffman("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
}

TEST_CASE("Encode Default tree")
{
    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;

    io::MemoryBuffer<256> original;
    std::string ha = "AAAABCB";
    for (auto letter : ha)
    {
        original.data()->push_back(letter);
    }
    io::MemoryBuffer<256> temp;

    encoding::encode(original.source(), temp.destination(), combined);

    std::string binary = "10001 1000 1100 0110 0100 0011 0011";

	
	auto data = temp.source()->create_input_stream();
    REQUIRE(*original.data() != *temp.data());
}

TEST_CASE("Encode & decode with huffman") 
{
    EncodeDecodeHuffman("QMLSDKFJQMLSKDJFQSMDLKFJ");
    EncodeDecodeHuffman("aaaaaaaaaaaaadfddddddddddddd");
    EncodeDecodeHuffman("fsfdddddddfeerggffffffff");
    EncodeDecodeHuffman("dqsd0595123qsdddddddddssseeee");
    EncodeDecodeHuffman("sqeeeeeeeeeeefffffffffffff");
    EncodeDecodeHuffman("ssdf zefzerzerds dsf ze ");
    EncodeDecodeHuffman("Als deze test passt ben ik heel blij");
	
}





#endif
