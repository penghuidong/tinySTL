#pragma once

#include "DObject.h"
#include "DException.h"
#include "functional.h"

namespace DLib
{
	typedef  bool nodeColor;
	const nodeColor black = false;
	const nodeColor red = true;

template <typename T>
class DRBTreeNode
{
public:
	DRBTreeNode<T>* parent;
	DRBTreeNode<T>* left;
	DRBTreeNode<T>* right;
	T node;
	nodeColor color;
private:
	DRBTreeNode() : parent(NULL), left(NULL), right(NULL) {}
public:
	static DRBTreeNode<T>* newNode()
	{
		return static_cast<DRBTreeNode<T>*>(DObject<>::operator new(sizeof(DRBTreeNode)));
	}
};

template <typename key, typename value, typename keyOfValue, typename Compare=less<T>>
class DRBTree
{
private:
	DRBTreeNode<T>* m_root;
	DRBTreeNode<T>* header;
	size_t countNode;
	Compare comfunc;

protected:
	

	bool& color(DRBTreeNode<T>* node) { return node->color; }

	DRBTreeNode<T>*& parent(DRBTreeNode<T>* node) { return node->parent; }

	DRBTreeNode<T>*& left(DRBTreeNode<T>* node) { return node->left; }

	DRBTreeNode<T>*& right(DRBTreeNode<T>* node) { return node->right; }

private:
	void init()
	{
		header = DRBTreeNode<T>::newNode();
		if (header != NULL)
		{
			header->color = red;
			m_root = NULL;
			header->left = header;
			header->right = header;
		}
	}

public:
	DRBTree() : m_root(NULL), header(NULL), countNode(0) { init();}

	DRBTreeNode<T>* root() const { return m_root; }

	DRBTreeNode<T>* leftmost() const { return header->left; }

	DRBTreeNode<T>* rightmost() const { return header->right; }

	//insert
	DRBTreeNode<T>* insertOnlyOne(const value& val);
};


template<typename key, typename value, typename keyOfValue, typename Compare = less<T>>
DRBTreeNode<key>*
DRBTree<key, value, keyOfValue, Compare>::insertOnlyOne(const value& val)
{

}


}

