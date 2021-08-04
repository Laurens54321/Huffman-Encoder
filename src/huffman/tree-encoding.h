#ifndef TREEENCODING_H
#define TREEENCODING_H
#include "binary-tree.h"
#include "util.h"
#include "io/streams.h"

namespace encoding
{
	namespace huffman
	{
		void encode_tree(const data::Node<Datum> root, Datum size, io::OutputStream& outputStream);
		bool test_encode(const data::Node<Datum> root);
	}

}


#endif