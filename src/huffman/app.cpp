

#include "bit-grouper.h"
#include "encoding.h"
#include "eof-encoding.h"
#include "huffman-encoding.h"
#include "encoding-combiner.h"
#include "io/files.h"
#ifndef TEST_BUILD

/*
*   This file is only active in non-test builds (i.e., builds where TEST_BUILD is not defined).
*/


encoding::Encoding<256, 256> getEncoding(char* name)
{
    if (name == "huffman")
    {
        std::cout << "Huffman is running" << std::endl;
    }



    const auto eof = encoding::eof_encoding<256>();
    const auto huffman = encoding::huffman::huffman_encoding<257>();
    const auto grouper = encoding::bit_grouper<8>();

    const auto combined = eof | huffman | grouper;
    return combined;




    assert(false);
}

bool getEncodeDecode(char* text)
{
    if (text == "decode") return true;
    return false;
}

/*
* This is the main entry point of your application.
* Note that it will only work in non-test builds.
*/
int main(int argc, char* argv[], char* envp[])
{
#ifdef NDEBUG
    std::cout << "You are running the release build" << std::endl;
#else
    std::cout << "You are running the debug build" << std::endl;
#endif

    bool numberLines = true;    
    for (int i = 0; argv[i] != NULL; ++i)
    {
        if (numberLines)
            std::cout << i << ": "; // Prefix with numbers if /n specified
        std::cout << argv[i] << "\n";
    }

    const auto encoding = getEncoding(argv[1]);
    const bool decode = getEncodeDecode(argv[2]);
    const auto input = io::create_file_data_source(argv[3]);
    const auto output = io::create_file_data_destination(argv[4]);
    
    if (!decode)
        encoding::encode(input, output, encoding);
    else
        encoding::decode(input, output, encoding);
}



#endif