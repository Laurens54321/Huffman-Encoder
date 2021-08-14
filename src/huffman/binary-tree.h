#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <cassert>

#include "memory"
#include "typeinfo"
#include "functional"

namespace data {
	template<typename T>
	class Node {
	public:
		virtual ~Node() {}
		virtual bool isLeaf() const { return false; }
	};

	template<typename T>
	class Branch: public Node<T> {
	private:
		std::unique_ptr<Node<T>> _left_child;
		std::unique_ptr<Node<T>> _right_child;
	public:
		Branch(std::unique_ptr<Node<T>> left_child, std::unique_ptr<Node<T>>right_child)
			: _left_child(std::move(left_child)), _right_child(std::move(right_child)) { }

		const Node<T>& getLeftChild() const { return *_left_child; }
		const Node<T>& getRightChild() const { return *_right_child; }

		bool isLeaf() const override  { return false; }
	};

	template<typename T>
	class Leaf : public Node<T> {
	private:
		T _value;
	public:
		Leaf(T value) :_value(value) {}
		const T& getValue() const { return _value; }

		bool isLeaf() const override { return true; }
	};


	template<typename _IN, typename _OUT> 
	std::unique_ptr<Node<_OUT>> map(const Node<_IN>& tree, std::function<_OUT(const _IN&)> function)
	{
		if (tree.isLeaf()) {
			const auto& L = static_cast<const Leaf<_IN>&>(tree);
			return std::make_unique<Leaf<_OUT>>(function(L.getValue()));
		}
		else {
			const auto& B = static_cast<const Branch<_IN>&>(tree);
			auto new_left_child = map<_IN, _OUT>(B.getLeftChild(), function);
			auto new_right_child = map<_IN, _OUT>(B.getRightChild(), function);
			return std::make_unique<Branch<_OUT>>(std::move(new_left_child), std::move(new_right_child));
		}
	}
	
	

		


}


#endif // !BINARYTREE_H

