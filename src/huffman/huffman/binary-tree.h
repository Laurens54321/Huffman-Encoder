#ifndef BINARYTREE_H

#include "memory"
#include "typeinfo"

namespace data {
	template<typename T>
	class Node {
	public:
		Node() {}
	};

	template<typename T>
	class Branch: public Node<T> {
	private:
		std::unique_ptr<Node<T>> _left_child;
		std::unique_ptr<Node<T>> _right_child;
	public:
		Branch(std::unique_ptr<Node<T>> left_child, std::unique_ptr<Node<T>>_right_child)
			: _left_child(std::move(left_child)), _right_child(std::move(_right_child)) { }

		std::unique_ptr<Node<T>> getLeftChild() const { return _left_child; }
		std::unique_ptr<Node<T>> getRightChild() const { return _right_child; }
	};

	template<typename T>
	class Leaf : public Node<T> {
	private:
		T _value;

	public:
		Leaf(T value) :_value(value) {}
		const T& getValue(){ return _value; }
	};

	
	template<typename IN, typename OUT> 
	std::unique_ptr<Node<OUT>> map(const Node<IN>& tree, std::function<OUT(const IN&)> function)
	{
		const Leaf<IN>* L;
		const Brtanch<IN>* B;
		if ((B = static_cast<const Leaf<IN>*>(&tree)) != nullptr) {
			Leaf<OUT> leaf(function(L->getValue()));
			return std::make_unique<Node<OUT>>(leaf);
		}
		else {
			B = static_cast<const Branch<IN>*>(&tree);
			const auto new_left_child = map<IN, OUT>(b->getLeftChild(), function).get();
			const auto new_right_child = map<IN, OUT>(b->getRightChild(), function).get();
			Branch<OUT> branch(*new_left_child, *new_right_child);
			return std::make_unique<Node<OUT>>(branch);
		}

		/*

		if (typeid(tree) == typeid(Leaf)) {
			Leaf f = Leaf(function(tree.getValue()))
		}
		/*

		if tree is leaf
			create new leaf with value = function(old_value)
		else tree is branch
			new_left_child = apply map to left child
			new_right_child = apply map to right child
			create new branch with new_left_child and new_right_child
		*/
	}

		


}


#endif // !BINARYTREE_H

