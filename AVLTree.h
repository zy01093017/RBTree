#pragma once

#include <iostream>
using namespace std;


#include <assert.h>

template<class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	int _bf;

	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _bf(0)
	{}
};

template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V>   Node;
	
public:
	AVLTree()
		:_root(NULL)
	{}

	~AVLTree()
	{

	}
	bool Insert(const K& key, const V& value)
	{
		//1.����
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		//2.���ǿ���
		Node* parent = NULL;
		Node* cur = _root;

		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else//��Ȳ�����
			{
				return false;
			}		 
		}
		//�ߵ��ò����λ��
		cur = new Node(key, value);
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

		//����ƽ�����ӣ��Ӳ���ĵط����ϵ���
		//��ǰ��cur
		while (parent)
		{
			if (parent->_left == cur)//���Ӽ���ƽ������
			{
				--parent->_bf;
			}
			else//�Һ��ӼӼ�ƽ������
			{
				++parent->_bf;
			}

			if (parent->_bf == 1 || parent->_bf == -1)//��parent
			{
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 0)
			{
				break;
			}
			else //�����׵�ƽ��������2����-2ʱ������ת
			{
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1)//���������2��������1�����������
					{
						RotateL(parent);
					}
					else//������2��������-1,��������˫��
					{
						RotateRL(parent);
					}
				}
				else//������-2�����
				{
					if (cur->_bf == -1)//������-2��������-1�������ҵ���
					{
						RotateR(parent);
					}
					else//������-2��������1����������˫��
					{
						RotateLR(parent);
					}
				}
				break;
			}			
		}
		return true;
	}

	//     40 parent
	//    /  \  
	//  30    50 subR
	//       /  \
    //subRL 35  60
	//            \
	//             70
	void RotateL(Node* parent)//ƽ������Ϊ2���Һ���Ϊ1
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
		parent->_bf = subR->_bf = 0;//ƽ�����Ӷ�Ϊ0
	}

	//       70 parent
	//      /  \
    //subL 60  80
	//    /  \
	//   50  65 SubLR
	//  /
	// 40

	void RotateR(Node* parent)//����ƽ��������-2��������-1
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
		subL->_bf = parent->_bf = 0;
	}

	/*1.�����ڵ� 
	
panent	50                50                45
	    /     ������      /     ������      / \
subR  40      --->      45      --->       40  50
	   \                /
subRL  45              40
	*/

	/*2.����ڵ���subLR������
	
	  50                  50                       45
	 /  \                / \                      / \
   40    60   ������    45  60      ������ bf=0  40   50   bf=1
   / \        --->      /           ----->      / \     \
  30  45               40                      30  43    60
     /                / \
	 43              30  43
*/

	/*3.����ڵ���subLR���Һ���

	   50                  50                       45
	  /  \                / \                      /  \
	40    60   ������    45  60      ������ bf=-1 40   50   bf=0
	/ \        --->     / \          ----->      /     / \
   30  45              40  48                  30     48  60
        \             / 
         48           30  
*/

	void RotateLR(Node* parent)//����˫����������-2��������1
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;
		//����subL��������
		RotateL(parent->_left);
		//����parent��������
		RotateR(parent);

		//��ʱƽ�����ӻ���иı�
		if (bf == 0)//��ʾsubLR���ǵ�ǰ�ڵ�
		{
			parent->_bf = subL->_bf = subLR->_bf = 0;
		}
		else if (bf == -1)
		{
			subLR->_bf = 0;
			subL->_bf = 0;
			parent->_bf = 1;
		}
		else if (bf == 1)
		{
			subLR->_bf = 0;
			subL->_bf = -1;
			parent->_bf = 0;
		}
		else
		{
			assert(false);//����
		}
	}
	/*1.�����ڵ�
	   30                   30                            35
		 \     ������         \          ������          /  \
		 40    ---->          35         ----->        30   40
		/                       \
	  35                        40
	*/

	/*2.����ڵ���subRL������
	   
	   30 parent                                30                                          35 subRL  bf = 0
	  / \            ��subR��������            /  \              ��parent��������          / \
    20   40  subR    ---->                   20    35  SubRL    -------->parent bf=0     30   40 subR bf=1
	     / \                                      / \                                    / \   \
bf=-1   35  50                                   33  40 subR                            20  33  50
	   /                                              \
	  33                                               50
	*/



	/*3.����ڵ���subRL���Һ���

		30 parent                                 30                                          35 subRL  bf = 0
		/ \            ��subR��������            /  \              ��parent��������          / \
	   20 40  subR    ---->                     20  35  SubRL    -------->     parent bf=-1 30 40 subR bf=0
	      / \                                         \                                    /   / \
bf=-1    35  50                                       40 subR                            20   37  50
	      \                                          /  \
	      37                                        37  50
*/
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		//����subL��������
		RotateR(parent->_right);
		//����parent��������
		RotateL(parent);

		if (bf == 0)//��ǰ�ڵ���ǲ���Ľڵ�
		{
			subR->_bf = subRL->_bf = parent->_bf = 0;
		}
		else if (bf == -1)
		{
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
		else if (bf == 1)
		{
			subR->_bf = 0;
			parent->_bf = -1;
			subRL->_bf = 0;
		}
		else
		{
			assert(false);
		}
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

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
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
			if (rightHeight - leftHeight != root->_bf)
			{
				cout << "ƽ�������쳣" << root->_key << endl;
				return false;
			}

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


void TestAVLtree()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };//{16, 3, 7, 11, 9, 26, 18, 14, 15};
	AVLTree<int, int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
	{
		t.Insert(a[i], i);
		cout << a[i] << ":" << t.IsBalance() << endl;
	}

	t.InOrder();
	cout << t.IsBalance() << endl;
}