#ifndef BINARYTREE_H
#define BINARYTREE_H

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
		Branch(std::unique_ptr<Node<T>> left_child, std::unique_ptr<Node<T>>_right_child)
			: _left_child(std::move(left_child)), _right_child(std::move(_right_child)) { }

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
		//Leaf(Node<T> node) { }
		const T& getValue() const { return _value; }

		bool isLeaf() const override { return true; }
	};

	
	template<typename _IN, typename _OUT> 
	std::unique_ptr<Node<_OUT>> map(const Node<_IN>& tree, std::function<_OUT(const _IN&)> function)
	{
		const Leaf<_IN>* L;
		const Branch<_IN>* B;
		if ((B = static_cast<const Leaf<_IN>*>(&tree)) != nullptr) {
			Leaf<_OUT> leaf(function(L->getValue()));
			return std::make_unique<Node<_OUT>>(leaf);
		}
		else {
			B = static_cast<const Branch<_IN>*>(&tree);
			const auto new_left_child = map<_IN, _OUT>(B->getLeftChild(), function).get();
			const auto new_right_child = map<_IN, _OUT>(B->getRightChild(), function).get();
			Branch<_OUT> branch(*new_left_child, *new_right_child);
			return std::make_unique<Node<_OUT>>(branch);
		}
	}

		


}


#endif // !BINARYTREE_H

