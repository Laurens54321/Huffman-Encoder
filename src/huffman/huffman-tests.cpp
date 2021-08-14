#include "binary-tree.h"
#include "catch.hpp"
#include "frequency-table.h"
#include "huffman-encoding.h"
#include "util.h"
#ifdef TEST_BUILD


TEST_CASE("Build tree test 1")
{
    auto freqTable = std::make_shared<data::FrequencyTable<Datum>>();
    freqTable->increment(1);
    freqTable->increment(1);
    freqTable->increment(1);
    freqTable->increment(1);


    freqTable->increment(2);
    freqTable->increment(2);


    auto root = std::move(getbuild_tree(freqTable));

    REQUIRE(!root->isLeaf());
    auto& rootBranch = static_cast<const data::Branch<std::pair<Datum, unsigned>>&>(*root);
    REQUIRE(rootBranch.getLeftChild().isLeaf());
    REQUIRE(rootBranch.getRightChild().isLeaf());
    auto& leftLeaf = static_cast<const data::Leaf<std::pair<Datum, unsigned>>&>(rootBranch.getLeftChild());
    auto& rightLeaf = static_cast<const data::Leaf<std::pair<Datum, unsigned>>&>(rootBranch.getRightChild());
    //REQUIRE(&leftLeaf.getValue().first)
    REQUIRE(leftLeaf.getValue().first == 1);
    REQUIRE(leftLeaf.getValue().second == 4);
    REQUIRE(rightLeaf.getValue().first == 2);
    REQUIRE(rightLeaf.getValue().second == 2);
}

TEST_CASE("Build tree test 2")
{
    auto freqTable = std::make_shared<data::FrequencyTable<Datum>>();
    freqTable->increment(0);
    freqTable->increment(0);
    freqTable->increment(0);
    freqTable->increment(0);


    freqTable->increment(2);
    freqTable->increment(2);

    freqTable->increment(3);

    auto root = std::move(getbuild_tree(freqTable));

    auto& branch1 = dynamic_cast<const data::Branch<std::pair<Datum, unsigned>>&>(*root);

}

TEST_CASE("building tree test 3")  //Test Baljit
{
    //std::vector<Datum> input = { 10, 0, 110, 0, 10, 0, 0 };
    std::vector<Datum> input = { 'B', 'A', 'C', 'A', 'B', 'A', 'A' };
    auto ft = std::make_shared<data::FrequencyTable<Datum>>(data::count_frequencies(input));

    //REQUIRE(*ft['B'] == 2);
    //REQUIRE(ft['A'] == 4);
    //REQUIRE(ft['C'] == 1);
    auto tree = std::move(getbuild_tree(ft));

    /*
    list = L(0, 4) , L(10, 2), L(110, 1)
    min -> Branch1 ( L(110, 1), L(10, 2) )

    list = L(0, 4)-w4 ,  Branch1 - w3
    min -> Root (Branch1 , L(0,4))

    //tree
                Root-w7
                /      \
        Branch1-w3    L1(0,4)
        /       \
     L2(110, 1)  L3(10, 2)

    */

    const data::Branch<std::pair<Datum, unsigned int>>& tree_root = dynamic_cast<const data::Branch<std::pair<Datum, unsigned int>>&>(*(tree.get()));
    REQUIRE(get_weight(tree_root) == 7);

    auto branch1 = dynamic_cast<const data::Branch<std::pair<Datum, unsigned int>>*>(&(tree_root).getRightChild());
    assert(&branch1 != nullptr);
    REQUIRE(get_weight(*branch1) == 3);

    auto L1 = dynamic_cast<const data::Leaf<std::pair<Datum, unsigned int>>*>(&(tree_root).getLeftChild());
    assert(&L1 != nullptr);
    REQUIRE(L1->getValue().first == 65);
    REQUIRE(L1->getValue().second == 4);

    auto L2 = dynamic_cast<const data::Leaf<std::pair<Datum, unsigned int>>*>(&(branch1->getRightChild()));
    assert(&L2 != nullptr);
    REQUIRE(L2->getValue().first == 67);
    REQUIRE(L2->getValue().second == 1);

    auto L3 = dynamic_cast<const data::Leaf<std::pair<Datum, unsigned int>>*>(&(branch1->getLeftChild()));
    assert(&L3 != nullptr);
    REQUIRE(L3->getValue().first == 66);
    REQUIRE(L3->getValue().second == 2);
}


TEST_CASE("Build codes test 1")
{
    auto freqTable = std::make_shared<data::FrequencyTable<Datum>>();
    freqTable->increment(1);
    freqTable->increment(1);
    freqTable->increment(1);
    freqTable->increment(1);


    freqTable->increment(2);
    freqTable->increment(2);


    auto root = std::move(getbuild_tree(freqTable));

    REQUIRE(!root->isLeaf());
    //auto& rootBranch = static_cast<data::Branch<Datum>&>(root);


    auto codes = getbuild_codes(*root);
    REQUIRE(codes.size() == 2);
}

#endif
