#ifndef TREEENCODING_H
#define TREEENCODING_H
#include "binary-tree.h"
#include "util.h"
#include "io/streams.h"

namespace encoding
{
	namespace huffman
	{
		void encode_tree(const data::Node<Datum>& root, Datum size, io::OutputStream& outputStream);
		std::unique_ptr<data::Node<Datum>> decode_tree(Datum nbits, io::InputStream& inputStream);
	}

}


#endif