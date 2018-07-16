
#pragma once


#include <iostream>
using namespace std;

enum Colour
{
	RED,
	BLACK,
};


template<class K,class V>
struct RBTreeNode
{
	K _key;
	V _value;

	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	Colour _colour;

	RBTreeNode(const K& key, const K& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _colour(RED)
	{}
};


template<class K,class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			_root->_colour = BLACK;
			return true;
		}

		Node* parent = NULL;
		Node* cur = _root;

		while (cur)
		{
			if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return false;
			}
		}
		
		cur = new Node(key, value);//����ڵ�
		cur->_colour = RED;

		if (parent->_key < key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//��ƽ��
		while (parent && parent->_colour == RED)//�����׵���ɫΪ��ɫ��Ҫ������ת
		{
			Node* grandfather = parent->_parent;

			if (grandfather->_left == parent)//�������������
			{
				Node* uncle = grandfather->_right;
				if (uncle && uncle->_colour == RED)//������������Ϊ��
				{
					parent->_colour = uncle->_colour = BLACK;//���׺�������ɫ��Ϊ�ڣ�үү��Ϊ��
					grandfather->_colour = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else//���岻���ڻ�������Ϊ��
				{
					//��cur�Ǹ��׵��Һ��ӣ�Ϊ���ߣ���һ������˫��
					if (parent->_right == cur)
					{
						RotateL(parent);

						swap(cur, parent);//������֮����н�������֤�ı���ɫ��һ����parent��grandfather��
					}

					RotateR(grandfather);

					parent->_colour = BLACK;
					grandfather->_colour = RED;
				}
			}
			else//�������Һ���
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_colour == RED)//������Ϊ��
				{
					uncle->_colour = parent->_colour = BLACK;
					grandfather->_colour = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else//���岻���ڻ���Ϊ��
				{
					if (parent->_left == cur)
					{
						RotateR(parent);

						swap(cur, parent);
					}

					RotateL(grandfather);
					parent->_colour = BLACK;
					grandfather->_colour = RED;
				}
			}
		}
		_root->_colour = BLACK;
		return true;
	}

	/*
	1.үү�����ף�������ͬһ�࣬���岻���ڻ���Ϊ�ڣ������ҵ���
	     ��   ��gandfa         �� parent                 ��
		/ \    ����           /  \                      /  \
	   ��  ��  ---->    cur ��    �� grandfa    ---->  ��   ��            parent  = ��
	  /                            \                          \           gandfather  = ��
     ��                             �� uncle                   ��
  */

	/*
	2.үү�����ף���ͬһ�࣬�����ڸ��׵��Ҳ࣬��һ���ߣ����岻���ڻ���Ϊ�ڣ����Ը��׽�������������үү�����ҵ���
	     ��       ��parent       ��                    ��
	    /  \      ������        /  \     ������       /  \
      ��    ��    ------>      ��  ��    ------>     ��   ��  -----> ����ͼ���иı���ɫ
	   \                      /                            \
	   ��                    ��                             ��
	*/


	void RotateL(Node* parent)//�����Ǻ죬��үү�ұߣ��������ұߣ����߸��׺죬��үү��ߣ��������ұߣ����Ը���Ϊ׼����������үү��������
	{
		Node* subR = parent->_right;//�Һ���
		Node* subRL = subR->_left;//�Һ��ӵ�����
		Node* ppNode = parent->_parent;//���׵ĸ���

		parent->_right = subRL;//���׵��Һ���ָ���Һ��ӵ�����
		if (subRL != NULL)
			subRL->_parent = parent;

		subR->_left = parent;//�Һ��ӵ�����ָ����
		parent->_parent = subR;
		if (parent == _root)//Ϊ����subR���Ǹ�
		{
			_root = subR;
			_root->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)//���parent�Ǹ��׵����ӣ����׵�����ָ��SubR
			{
				ppNode->_left = subR;
			}
			else if (ppNode->_right == parent)//���parent�Ǹ��׵��Һ��ӣ����׵��Һ���ָ��SubR
			{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
	}
	/*
	1.үү�����ף�������ͬһ�࣬���岻���ڻ���Ϊ�ڣ���������
		 ��   ��gandfa          �� parent                 ��
	    / \    ����            /  \        ����ɫ        /  \             parent = ��
	  ��   ��    ---->grandfa ��   ��cur   ---->        ��   ��           grandfather = ��
	        \                /                         /      
	        ��         uncle��                        ��
	*/
	
	/*
	2.үү�����ף���ͬһ�࣬�����ڸ��׵��Ҳ࣬��һ���ߣ����岻���ڻ���Ϊ�ڣ����Ը��׽�������������үү�����ҵ���
	   ��       ��parent       ��                   ��
	  /  \      ������        /  \    ������       /  \
	 ��   ��    ------>      ��  ��  ------>      ��  ��-----> ����ͼ���иı���ɫ
		 /	                       \             /     
	    ��                         ��          ��
	*/

	void RotateR(Node* parent)//�����Ǻ죬��үү��ߣ���������ߣ����߸��׺죬��үү�ұߣ���������ߣ����Ը���Ϊ׼����������үү��������
	{
		Node* subL = parent->_left;//���׵�����
		Node* subLR = subL->_right;//���ӵ��Һ���
		Node* ppNode = parent->_parent;//���׵ĸ���

		parent->_left = subLR;//���׵�����ָ�����ӵ��Һ���
		if (subLR != NULL)
			subLR->_parent = parent;

		subL->_right = parent;//���ӵ��Һ���ָ����		
		parent->_parent = subL;
		if (_root == parent)//���ڵ�
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (ppNode->_left == parent)//���и��ף��Ǹ��׵�����
			{
				ppNode->_left = subL;
			}
			else//���и��ף��Ǹ��׵��Һ���
			{
				ppNode->_right = subL;
			}
			subL->_parent = ppNode;
		}
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}


	bool _IsBalance(Node* root, int& height)
	{
		if (root == NULL)
		{
			height = 0;
			return true;
		}

		int leftHeight = 0;
		int rightHeight = 0;
		if (_IsBalance(root->_left, leftHeight)
			&& _IsBalance(root->_right, rightHeight))
		{
			height = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
			return abs(leftHeight - rightHeight) < 2;
		}
		else
		{
			return false;
		}
	}

	bool IsBalance()
	{
		int height = 0;
		return _IsBalance(_root, height);
	}
protected:
	Node* _root;
};


void TestRBTree()
{
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	RBTree<int, int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(a[i], i);
		cout << a[i] << ":" << t.IsBalance() << endl;

	}
	t.InOrder();
	cout << t.IsBalance() << endl;

}