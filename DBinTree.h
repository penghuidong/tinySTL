#ifndef DBINTREE_H
#define DBINTREE_H

#include "DException.h"
#include "DObject.h"
#include "DSingleLinkList.h"
#include "DSharedPointer.h"
#include "Config.h"
#include "DQueue.h"
#include "DVector.h"


namespace DLib
{

template <typename T>
class DBinTreeNode
{
public:
    T nodeValue;
    DBinTreeNode<T>* m_parent;
    DBinTreeNode<T>* rchild;
    DBinTreeNode<T>* lchild;

    bool newFlag;

    void* operator new(size_t size)
    {
        return DObject<>::operator new(size);
    }

    DBinTreeNode(const DBinTreeNode& rval) : m_parent(NULL), rchild(NULL), lchild(NULL), newFlag(false) {}

    DBinTreeNode& operator =(const DBinTreeNode& rval);

public:
    DBinTreeNode() : m_parent(NULL), rchild(NULL), lchild(NULL), newFlag(false) {}

    DBinTreeNode(const T& val) : nodeValue(val), m_parent(NULL), rchild(NULL), lchild(NULL), newFlag(false) {}

    static DBinTreeNode* newNode(const T& val)
    {
        DBinTreeNode* ret = NULL;
        ret = static_cast<DBinTreeNode*>(DObject<>::operator new(sizeof(DBinTreeNode)));
        if(ret!=NULL)
        {
            ::new (ret) DBinTreeNode(val);
            ret->newFlag = true;
        }
        return ret;
    }

    bool getNewFlag() const {return newFlag;}
};

template<typename T>
class DBinTree : public DObject<>
{
public:
    enum BTNodePos
    {
        ANY, LEFT, RIGHT
    };
    enum BTTraversal{
        PreOrder,
        InOrder,
        PostOrder,
        LevelOrder
    };
private:
	DBinTreeNode<T>* m_root;
	DQueue<DBinTreeNode<T>*> m_queue;

	//基于值的查找
	DBinTreeNode<T>* find_private(DBinTreeNode<T>* node, const T& val) const 
	{	
		DBinTreeNode<T>* ret = NULL;

		if(node!=NULL)
		{
            if(node->nodeValue==val)
				ret = node;
			else 
			{
				if(ret==NULL)
                    ret = find_private(node->lchild, val);
				if(ret==NULL)
                    ret = find_private(node->rchild, val);
			}
		}
		return ret;
	}

	DBinTreeNode<T>* find_private(DBinTreeNode<T>* node, DBinTreeNode<T>* obj) const 
	{	
		DBinTreeNode<T>* ret = NULL;
		if(node!=NULL)
		{
			if(node==obj)
				ret = node;
			else 
			{
				if(ret==NULL)
                    ret = find_private(node->lchild, obj);
				if(ret==NULL)
                    ret = find_private(node->rchild, obj);
			}
		}
		return ret;
    }
	
    bool insert_private(DBinTreeNode<T>* node, DBinTreeNode<T>* nodeparent, BTNodePos pos)
	{
		bool ret = true;

		if(pos==ANY)  //插入位置随意
		{
			if(nodeparent->lchild==NULL)
				nodeparent->lchild = node;
			else if(nodeparent->rchild==NULL)
				nodeparent->rchild = node;
			else 
				ret = false;
		}
		else if(pos==LEFT) 
		{
			if(nodeparent->lchild==NULL)
				nodeparent->lchild = node;
			else
				ret = false; //只能插入左边，但是左边不空，返回错误
		}
		else if(pos==RIGHT)
		{	
			if(nodeparent->rchild==NULL)
				nodeparent->rchild = node;
			else 
				ret = false;
		}
		else 
			ret = false;
		return ret;
	}

    void remove_private(DBinTreeNode<T>* node, DBinTree<T>*& ret)
    {
        ret = new DBinTree<T>();
        if(ret!=NULL)
        {
            if(root()==node)
                this->m_root = NULL;
            else
            {
                DBinTreeNode<T>* parent = node->m_parent;
                if(parent->lchild==node)
                    parent->lchild = NULL;
                else if(parent->rchild==node)
                    parent->rchild = NULL;
                node->m_parent = NULL;
            }
            ret->m_root = node;
        }
        else
        {
            throw_exception(outOfMemory, "out of memory");
        }
    }

    void free(DBinTreeNode<T>* node)
    {
        if(node!=NULL)
        {
            free(node->lchild);
            free(node->rchild);
            if(node->newFlag)
                DObject<>::operator delete(node);
        }
    }

    size_t count(DBinTreeNode<T>* node) const
    {
        size_t ret = 0;
        if(node==NULL) return 0;
        else ret = count(node->rchild) + count(node->lchild) +1;
        return ret;
    }

    size_t height(DBinTreeNode<T>* node) const
    {
        size_t ret = 0;
        if(node==NULL) return 0;
        else
        {
            size_t rh = height(node->rchild) + 1;
            size_t lh = height(node->lchild) + 1;
            ret = rh>lh ? rh : lh;
        }
        return ret;
    }

    size_t degree(DBinTreeNode<T>* node) const
    {
        size_t ret = 0;
        if(node!=NULL)
        {
            DBinTreeNode<T>* child[] ={node->lchild, node->rchild};
            ret = (!!node->lchild+!!node->rchild);
            for(int i=0 ;i<2 && (ret<2); i++)
            {
                size_t d = degree(child[i]);
                if(ret<d) ret = d;
            }
        }
        return ret;
    }

    void preOrderTraver(DBinTreeNode<T>* node, DQueue<DBinTreeNode<T>*>& queue)
    {
        if(node!=NULL)
        {
            queue.push(node);
            preOrderTraver(node->lchild, queue);
            preOrderTraver(node->rchild, queue);

        }
    }

    void inOrderTraver(DBinTreeNode<T>* node, DQueue<DBinTreeNode<T>*>& queue)
    {
        if(node!=NULL)
        {
            inOrderTraver(node->lchild, queue);
            queue.push(node);
            inOrderTraver(node->rchild, queue);
        }
    }

    void postOrderTraver(DBinTreeNode<T>* node, DQueue<DBinTreeNode<T>*>& queue)
    {
        if(node!=NULL)
        {
            postOrderTraver(node->lchild, queue);
            postOrderTraver(node->rchild, queue);
            queue.push(node);
        }
    }

    void levelOrderTraver(DBinTreeNode<T>* node, DQueue<DBinTreeNode<T>*>& queue)
    {
        if(node!=NULL)
        {
            DQueue<DBinTreeNode<T>* > tmp;
            tmp.push(node);
            while(tmp.length()>0)
            {
                DBinTreeNode<T>* n = tmp.front();
                if(n->lchild!=NULL) tmp.push(n->lchild);
                if(n->rchild!=NULL) tmp.push(n->rchild);
                tmp.pop();
                queue.push(n);
           }
        }
    }

    void traver(BTTraversal order, DQueue<DBinTreeNode<T>*>& queue)
    {
        switch (order)
        {
        case PreOrder:  preOrderTraver(root(), queue); break;
        case InOrder:   inOrderTraver(root(), queue); break;
        case PostOrder: postOrderTraver(root(), queue); break;
        case LevelOrder:levelOrderTraver(root(), queue); break;
        default:
            throw_exception(invalidParameter, "invalid paremeter"); break;
        }
    }

public:
    DBinTree() : m_root(NULL) {}

	bool insert(DBinTreeNode<T>* node)
	{
		return insert(node, ANY);
	}
	
    bool insert(DBinTreeNode<T>* node,BTNodePos pos)
	{
		bool ret = true;
		if(node!=NULL)
		{
			if(this->m_root==NULL)
			{
				node->m_parent = NULL;
				this->m_root = node;
			}
			else 
			{
				DBinTreeNode<T>* nodeparent = find(node->m_parent);
				if(nodeparent!=NULL)
					ret = insert_private(node, nodeparent, pos);
				else 
					throw_exception(invalidParameter, "invalid parameter");
			}
		}
		else
		{
			throw_exception(invalidParameter, "node cannot be NULL");
		}
		return ret;
	}

    bool insert(const T& value, DBinTreeNode<T>* parent)
    {
        return insert(value, parent, ANY);
    }

    bool insert(const T& val, DBinTreeNode<T>* parent, BTNodePos pos)
	{
        bool ret = true;
        DBinTreeNode<T>* node = DBinTreeNode<T>::newNode(val);
        if(node!=NULL)
        {
            node->m_parent = parent;
            ret = insert(node, pos);
            if(!ret) DObject<>::operator delete(node);
        }
        else
        {
            throw_exception(outOfMemory, "out ot memory");
        }

        return ret;
	}

	DSharedPointer<DBinTree<T>> remove(const T& val)
	{
        DBinTree<T>* ret = NULL;
        DBinTreeNode<T>* node = find(val);
        if(node==NULL)
            throw_exception(invalidParameter, "can not find value");
        else
            remove_private(node, ret);
        return ret;
	}

	DSharedPointer<DBinTree<T>> remove(DBinTreeNode<T>* node)
	{
        DBinTree<T>* ret = NULL;
        node = find(node);
        if(node==NULL)
            throw_exception(invalidParameter, "invalid parameter");
        else
            remove_private(node, ret);
        return ret;
	}

    DBinTreeNode<T>* find(const T& val) const {return find_private(root(), val);}

	DBinTreeNode<T>* find(DBinTreeNode<T>* node) const {return find_private(root(), node);}

    DBinTreeNode<T>* root() const { return m_root;}

    size_t degree() const { return degree(root());}

    size_t count() const {return count(root());}

    size_t height() const {return height(root());}

    void clear() {free(root());}

    DSharedPointer<DVector<T>> traver(BTTraversal order)
    {
        DVector<T>* ret = NULL;
        DQueue<DBinTreeNode<T>*> queue;

        traver(order, queue);
        ret = new DVector<T>(queue.length());
        if(ret!=NULL)
        {
            for(size_t i=0; i<ret->length(); i++,queue.pop())
                (*ret)[i] = queue.front()->nodeValue;
        }
        else
        {
            throw_exception(outOfMemory,"no memory to create return array");
        }
        return ret;
    }

    ~DBinTree() {clear();}
};

}


#endif // DBINTREE_H
