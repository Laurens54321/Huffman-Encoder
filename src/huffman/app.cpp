#ifndef TEST_BUILD

#include "bit-grouper.h"
#include "encoding.h"
#include "eof-encoding.h"
#include "huffman-encoding.h"
#include "encoding-combiner.h"
#include "io/files.h"


/*
*   This file is only active in non-test builds (i.e., builds where TEST_BUILD is not defined).
*/


void compress(const std::string& file1, const std::string& file2)
{
    std::cout << "Compressing..." << std::endl;
    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;

    encode(io::create_file_data_source(file1), io::create_file_data_destination(file2), combined);
}

void decompress(const std::string& file1, const std::string& file2)
{
    std::cout << "Decompressing..." << std::endl;
    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;

    encoding::decode(io::create_file_data_source(file1), io::create_file_data_destination(file2), combined);
}

int main(const int argc, char* argv[])
{
    for (int i = 0; argv[i] != NULL; ++i)
    {
        
        std::cout << i << ": " << argv[i] << "\n";
    }


	
    std::string WrongArgument = "Wrong arguments. (method, input.type, output.type)";
#ifdef NDEBUG
    if (argc != 4)
    {
        std::cout << WrongArgument << std::endl;
        return -1;
    }
    else
    {
        const std::string function(argv[1]);
        const std::string file1(argv[2]);
        const std::string file2(argv[3]);
        if (function == "compress")
        {
            compress(file1, file2);
            std::cout << "File " + file1 + " compressed successfully to " + file2 + "!" << std::endl;
            return 0;
        }
        else if (function == "decompress")
        {
            decompress(file1, file2);
            std::cout << "File " + file1 + " decompressed successfully to " + file2 + "!" << std::endl;
            return 0;
        }
        else if (function == "multi") {
            auto tempfile = "temp.hf";
        	compress(file1, tempfile);
            std::cout << "File " + file1 + " compressed successfully to " + tempfile + "!" << std::endl;
            decompress(tempfile, file2);
            std::cout << "File " << tempfile << " decompressed successfully to " << file2 + "!" << std::endl;
        }
        else
        {
            std::cout << WrongArgument << std::endl;
            return -1;
        }
    }
}
#else
#endif

#endif
