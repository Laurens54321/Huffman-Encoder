#ifndef HUFFMAN_ENCODING_H
#define HUFFMAN_ENCODING_H

#include "binary-tree.h"
#include "encoding.h"
#include "frequency-table.h"
#include "util.h"



std::shared_ptr<encoding::EncodingImplementation> create_huffman_implementation(u64 domain_size);
u64 get_weight(const data::Node<std::pair<Datum, unsigned>>& node);
std::unique_ptr<data::Node<std::pair<Datum, unsigned>>> getbuild_tree(const std::shared_ptr<data::FrequencyTable<Datum>>& frequency_table);
std::map<Datum, std::vector<Datum>> getbuild_codes(data::Node<std::pair<Datum, unsigned>>& root);
namespace encoding
{
	namespace huffman
	{
		template<u64 N>
		encoding::Encoding<N, 2> huffman_encoding()
		{
			auto encoder = Encoding<N, 2>(create_huffman_implementation(N));
			return encoder;
		}
	}
}



#endif
