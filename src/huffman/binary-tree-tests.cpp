#include "huffman-encoding.h"
#include "io/memory-buffer.h"
#ifdef TEST_BUILD

#include "util.h"
#include "catch.hpp"
#include <tree-encoding.h>


TEST_CASE("Branch has 2 leaves that compiles and saves value")
{
    auto left = std::make_unique<data::Leaf<char>>('x');
    auto right = std::make_unique<data::Leaf<char>>('y');

    data::Branch<char> root(std::move(left), std::move(right));
    auto& left_branch = root.getLeftChild();
    auto& right_branch = root.getRightChild();

    auto& leftActual = static_cast<const data::Leaf<char>&> (left_branch);  
    auto& rightActual = static_cast<const data::Leaf<char>&> (right_branch);

    REQUIRE(leftActual.getValue() == 'x');  
    REQUIRE(rightActual.getValue() == 'y');
}

TEST_CASE("Binary Tree testsing") {
    data::Leaf<int> myleaf(9);
    REQUIRE(myleaf.getValue() == 9);
    data::Node<int>& mynode = myleaf;
    auto newLeaf = static_cast<data::Leaf<int>&>(mynode);
    REQUIRE(newLeaf.getValue() == 9);
}

TEST_CASE("Tree encoding and decoding")
{
    auto freqTable = std::make_shared<data::FrequencyTable<Datum>>();
    freqTable->increment(6);
    freqTable->increment(6);
    freqTable->increment(6);
    freqTable->increment(6);


    freqTable->increment(2);
    freqTable->increment(2);


    auto root = std::move(getbuild_tree(freqTable));
    auto newroot = data::map<std::pair<Datum, unsigned>, Datum>(*root, [](std::pair<Datum, unsigned> pair) { return pair.first; });
    io::MemoryBuffer<8> temp;
    io::MemoryBuffer<8> out;
	
    encoding::huffman::encode_tree(*newroot, 3, *temp.destination()->create_output_stream());
    //encoding::huffman::encode_tree(*newroot, 3, *temp.destination()->create_output_stream());
    auto decodedroot = encoding::huffman::decode_tree(3, *temp.source()->create_input_stream());
    REQUIRE(!decodedroot->isLeaf());
	auto branch = static_cast<data::Branch<Datum>*>(decodedroot.get());
    const auto& leftleaf = static_cast<const data::Leaf<Datum>&>(branch->getLeftChild());
    const auto& rightleaf = static_cast<const data::Leaf<Datum>&>(branch->getRightChild());
	REQUIRE(leftleaf.isLeaf());
    REQUIRE(rightleaf.isLeaf());
    REQUIRE(leftleaf.getValue() == 6);
    REQUIRE(rightleaf.getValue() == 2);	
}



#endif