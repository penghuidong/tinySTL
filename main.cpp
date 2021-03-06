// test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DBinTree.h"
#include "DRBTree.h"
#include "functional.h"

using namespace DLib;
using namespace std;

int main()
{
    DBinTree<int> btree;
	DBinTreeNode<int>* n = NULL;

	btree.insert(1, NULL);
	
	n = btree.find(1);
	btree.insert(2, n);
	btree.insert(3, n);

	n = btree.find(2);
	btree.insert(4, n);
	btree.insert(5, n);

	n = btree.find(4);
	btree.insert(8, n);
	btree.insert(9, n);

	n = btree.find(5);
	btree.insert(10, n);

	n = btree.find(3);
	btree.insert(6, n);
	btree.insert(7, n);

	n = btree.find(6);
	btree.insert(11, n, DBinTree<int>::LEFT);

	DSharedPointer<DVector<int>> ret = btree.traver(DBinTree<int>::InOrder);
	cout << "traver result: " << endl;
	for (size_t i = 0; i<(*ret).length(); i++)
		cout << (*ret)[i] << " ";

	cout << "count = " << btree.count() << endl;
	cout << "height = " << btree.height() << endl;

	DSharedPointer<DBinTree<int>> sp = btree.remove(3);

	cout << "count = " << btree.count() << endl;
	cout << "height = " << btree.height() << endl;
	cout << "degree = " << btree.degree() << endl;

	int a[] = { 8, 9, 10, 11, 7 };
	for (size_t i = 0; i<5; i++)
	{
		DBinTreeNode<int>* node = btree.find(a[i]);
		while (node)
		{
			cout << node->nodeValue << " ";
			node = node->m_parent;
		}
		cout << endl;
	}



	cout << endl;

	DRBTree<int> rbtree;



	system("pause");
	return 0;
}

