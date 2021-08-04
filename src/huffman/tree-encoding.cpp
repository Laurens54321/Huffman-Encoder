#include "binary-io.h"
#include "binary-tree.h"
#include "util.h"
#include "io/streams.h"

namespace encoding
{
	namespace huffman	
	{
		void encode_tree(const data::Node<Datum>& root, Datum size, io::OutputStream& outputStream)
		{
			
			if (root.isLeaf())
			{
				const data::Leaf<Datum>& leaf = dynamic_cast<const data::Leaf<Datum>&>(root);
				outputStream.write(0);
				io::write_bits(leaf.getValue(), 1, outputStream);
			}
			else
			{
				outputStream.write(1);
				auto& branch = static_cast<const data::Branch<Datum>&>(root);
				encode_tree(branch.getLeftChild(), size, outputStream);
				encode_tree(branch.getRightChild(), size, outputStream);
			}
			
		}

		std::unique_ptr<data::Node<Datum>> decode_tree(Datum nbits, io::InputStream& inputStream)
		{
			if (inputStream.read() == 0)
			{
				Datum data = io::read_bits(nbits, inputStream);
				return std::make_unique<data::Leaf<Datum>>(data);
			}
			else
			{
				std::unique_ptr<data::Node<Datum>> leftNode = decode_tree(nbits, inputStream);
				std::unique_ptr<data::Node<Datum>> rightNode = decode_tree(nbits, inputStream);
				return std::move(std::make_unique<data::Branch<Datum>>(std::move(leftNode), std::move(rightNode)));
			}			
		}	
	}
}
