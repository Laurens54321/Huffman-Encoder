#include <algorithm>
#include <cassert>
#include <encoding.h>
#include <frequency-table.h>
#include <numeric>
#include "encoding.h"
#include "eof-encoding.h"
#include "util.h"
#include "binary-tree.h"
#include "tree-encoding.h"


namespace 
{
		/*
	int weight(data::Node<std::pair<Datum, unsigned>>& tree)
	{
		assert(&tree != nullptr);

		if (tree.isLeaf())
		{
			auto& L = static_cast<const data::Leaf<std::pair<Datum, unsigned>>&>(tree);
			assert(&tree != nullptr);
			assert(L.getValue().second < 100);
			return L.getValue().second;
		}

		int total_weight = 0;
		auto& B = static_cast<const data::Branch<std::pair<Datum, unsigned>>&>(tree);

		auto& left = B.getLeftChild();
		if (&left != nullptr)
			total_weight += weight(left);

		auto& right = B.getRightChild();
		if (&right != nullptr)
			total_weight += weight(right);

		assert(total_weight < 100);
		return total_weight;
	}
	*/

	u64 weight(const data::Node<std::pair<Datum, unsigned>>& node)      //Szymon's weight functie, only use for emergencies
	{
		if (node.isLeaf())
		{
			auto& current_leaf = static_cast<const data::Leaf<std::pair<Datum, unsigned>>&>(node);
			const auto weight = current_leaf.getValue().second;
			return weight;
		}

		// Get children
		auto& branch = static_cast<const data::Branch<std::pair<Datum, unsigned>>&>(node);
		auto& left_child = branch.getLeftChild();
		auto& right_child = branch.getRightChild();

		// Calculate weight & return
		const auto left_weight = weight(left_child);
		const auto right_weight = weight(right_child);
		const auto weight_result = left_weight + right_weight;

		return weight_result;
	}


	struct {
		bool operator()(const std::unique_ptr<data::Node<std::pair<Datum, unsigned>>>& a, const std::unique_ptr<data::Node<std::pair<Datum, unsigned>>>& b) const { return weight(*a) > weight(*b); }
	} customGreater;

	std::vector<Datum> copy_to_vector(io::InputStream& inputStream) {
		std::vector<Datum> data;
		while (!inputStream.end_reached()) {
			data.push_back(inputStream.read());
		}
		return data;
	}

	/*

	void addNodeToList(std::vector<data::Node<std::pair<Datum, unsigned>>>* nodes, data::Node<std::pair<Datum, unsigned>>* node)
	{
		nodes->reserve(1);
		for (int i = 0; i < nodes->size(); i++)
		{
			if (weight((nodes->at(i))) < weight(*node)){
				nodes->insert(nodes->begin() + i, *node);
				break;
			}
		}
	}

	*/

	Datum getNode(data::Leaf<std::pair<Datum, unsigned>> node)
	{
		return node.getValue().first;
	}

	std::unique_ptr<data::Node<std::pair<Datum, unsigned>>> build_tree(const std::shared_ptr<data::FrequencyTable<Datum>>& frequency_table) {


		std::vector<std::pair<Datum, unsigned>> pairs = frequency_table->pairs();

		std::vector<std::unique_ptr<data::Node<std::pair<Datum, unsigned>>>> nodeList;

		nodeList.reserve(pairs.size() - 1);
		for (auto pair : pairs)
		{
			nodeList.push_back(std::make_unique<data::Leaf<std::pair<Datum, unsigned>>>(std::pair<Datum, unsigned>(pair.first, pair.second)));
		}

		assert(pairs.size() == nodeList.size());

		while (nodeList.size() > 1)
		{
			std::sort(nodeList.begin(), nodeList.end(), customGreater);
			auto rightleaf = std::move(nodeList.back());
			nodeList.pop_back();
			assert(rightleaf != nullptr);

			auto leftleaf = std::move(nodeList.back());
			nodeList.pop_back();
			assert(leftleaf != nullptr);

			auto newBranch = std::make_unique<data::Branch<std::pair<Datum, unsigned>>>(std::move(leftleaf), std::move(rightleaf));
			nodeList.push_back(std::move(newBranch));
		}

		assert(nodeList.size() == 1);
		auto root = std::move(nodeList.at(0));
		
		//auto newroot = std::make_unique<data::Node<Datum>>();
		return root;
	}

	std::vector<Datum> addBit(std::vector<Datum> bits, Datum bit)
	{
		bits.push_back(bit);
		return bits;
	}

	void build_vector(const data::Node<Datum>& node, std::vector<Datum> bits, std::map<Datum, std::vector<Datum>>& codes, unsigned depth)
	{
		if (node.isLeaf())
		{
			auto& current_leaf = static_cast<const data::Leaf<Datum>&>(node);
			assert(&current_leaf != nullptr);
			std::vector<Datum> code(bits.end() - depth, bits.end());
			
			codes.insert(std::pair<Datum, std::vector<Datum>>(current_leaf.getValue(), bits));
			return;
		}
		else
		{
			const auto& current_branch = static_cast<const data::Branch<Datum>&>(node);
			assert(&current_branch != nullptr);
			unsigned new_depth = depth + 1;
			if (&current_branch.getLeftChild() != nullptr)
				build_vector(current_branch.getLeftChild(), addBit(bits, 0), codes, new_depth);
			if (&current_branch.getRightChild() != nullptr)
				build_vector(current_branch.getRightChild(), addBit(bits, 1), codes, new_depth);
		}
	}

	std::map<Datum, std::vector<Datum>> build_codes(const data::Node<Datum>& root)
	{
		std::map<Datum, std::vector<Datum>> codes;
		std::vector<Datum> bits;
		build_vector(root, bits , codes, 0);
		assert(!codes.empty());
		assert(codes.size() < 1000);
		return codes;
	}

	void encode_data(std::map<Datum, std::vector<Datum>>& codes, const std::vector<Datum>& data, io::OutputStream& outputStream)
	{
		for (auto value : data)
		{
			auto bits = codes[value];
			//assert(bits.size() > 1);
			for (auto bit : bits)
			{
				outputStream.write(bit);
			}
		}
	}

	Datum decode_single_datum(io::InputStream& input, const data::Node<Datum>& root) 
	{
		assert(!root.isLeaf());
		auto current = &root;
		while (true)
		{
			if (input.end_reached())
			{
				return 0;
			}
			const auto value = input.read();
			auto& branch = static_cast<const data::Branch<Datum>&>(*current);
			if (value == 0)
			{
				current = &branch.getLeftChild();
			}
			else
			{
				current = &branch.getRightChild();
			}
			if (current->isLeaf())
			{
				auto& leaf = static_cast<const data::Leaf<Datum>&>(*current);
				return leaf.getValue();
			}
		}
	}

	void decode_bits(io::InputStream& input_stream, io::OutputStream& output_stream, data::Node<Datum>& root)
	{
		int i = 0;
		while (!input_stream.end_reached())
		{
			Datum data = decode_single_datum(input_stream, root);
			output_stream.write(data);
			if (data == 0) break;
			i++;
		}
		assert(i > 0);
	}


	class HuffmanEncodingImplementation : public encoding::EncodingImplementation {
	private:
		const Datum _domain_size;

	public:
		HuffmanEncodingImplementation(const Datum domain_size) : EncodingImplementation(), _domain_size(domain_size) {}


		void encode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{

			const auto data = copy_to_vector(inputStream);
			const auto frequencyTable = std::make_shared<data::FrequencyTable<Datum>>();
			for (auto datum : data)
			{
				frequencyTable->increment(datum);
			}


			const auto root = build_tree(frequencyTable);
			auto newroot = data::map<std::pair<Datum, unsigned>, Datum>(*root, [](std::pair<Datum, unsigned> pair) { return pair.first; });
			int actualbits = bits_needed(_domain_size);
			encoding::huffman::encode_tree(*newroot, actualbits, outputStream);
			
			auto codes = build_codes(*newroot);
			encode_data(codes , data, outputStream);

		}

		void decode(io::InputStream& inputStream, io::OutputStream& outputStream) override
		{
			auto root = encoding::huffman::decode_tree(bits_needed(_domain_size), inputStream);
			assert(!root->isLeaf());
			decode_bits(inputStream, outputStream, *root);
		}
	};
}
	
	


std::shared_ptr<encoding::EncodingImplementation> create_huffman_implementation(u64 domain_size)
{
	return std::make_shared<HuffmanEncodingImplementation>(domain_size);
}

u64 get_weight(const data::Node<std::pair<Datum, unsigned>>& node)
{
	return weight(node);
}

std::unique_ptr<data::Node<std::pair<Datum, unsigned>>> getbuild_tree(const std::shared_ptr<data::FrequencyTable<Datum>>& frequency_table)
{
	return build_tree(frequency_table);
}

std::map<Datum, std::vector<Datum>> getbuild_codes(data::Node<std::pair<Datum, unsigned>>& root)
{
	auto newroot = data::map<std::pair<Datum, unsigned>, Datum>(root, [](std::pair<Datum, unsigned> pair) { return pair.first; });
	return build_codes(*newroot);
}
