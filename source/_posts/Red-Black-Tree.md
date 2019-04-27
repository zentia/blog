---
title: 数据结构与算法
date: 2018-03-29 16:27:07
tags:
	- 数据结构与算法
---

# 红黑树

红黑树是一种自平衡二叉查找树。

## 性质

1.节点是红色或黑色。
2.根节点是黑色
3.每个叶节点（NIL或空节点）是黑色
4.每个红色节点的两个子节点都是黑色的（也就是说不存在两个连续的红色节点）；
5.从任意一个节点到其叶子节点都包含相同数目的黑色节点；

# AVL树

## 性质

1.本身首先是一棵二叉搜索树
2.带有平衡条件：每个节点的左右子树的高度之差的绝对值（平衡因子）最多为1.

## 节点数


## STL和Linux都使用红黑树作为平衡树的实现

1.如果插入一个node引起了树的不平衡，AVL和RB-Tree都是最多只需要2次旋转操作，即两者都是O(1)；但是在删除node引起树的不平衡时，最坏情况下，AVL需要维护从被删node到root这条路径上所有node的平衡性，因此需要旋转的两级O(logN)，而RB-Tree最多只需要3次旋转，只需要O(1)的复杂度。
2.其次，AVL的结构相较RB-Tree来说更为平衡，在插入和删除node更容易引起Tree的unbalance，因此在大量数据需要插入或删除时，AVL需要rebalance的频率会更高。因此，RB-Tree在需要大量插入和删除node的场景下，效率更高。自然，由于AVL高度平衡，因此AVL的search效率更高。
3.map的实现只是折衷了两者在search、insert以及delete下的效率。总体来说，RB-tree的统计行能是高于AVL的。


# 二叉查找树

## 定义

1.若左子树不空，则左子树所有节点的值均小于或等于它的根节点的值；
2.若右子树不空，则右子树所有节点的值均大于或等于它的根节点的值；
3.左、右子树也分别为二叉排序树；

## 查找

步骤：若根节点的关键字值等于查找的关键字，成功。
否则，若小于根节点的关键字值，递归查左子树。
P(n)=O(logn)

## 非递归遍历

### 前序非递归遍历

```C++
void preorder_traversal_iteratively(TreeNode* root)
{
	if (root == 0)
	{
		return;
	}
	stack<TreeNode*> s;
	s.push(root);
	cout << root->val << ' ';
	TreeNode* last_pop = root;
	while (!s.empty())
	{
		TreeNode* top = s.top();
		if (top->left != 0 && top->left != last_pop && top->right != last_pop)
		{
			s.push(top->left);
			cout >> top->left->val << ' ';
		}
		else if (top->right != 0 && top->right != last_pop && (top->left == 0 || top->left == last_pop))
		{
			s.push(top->right);
			cout >> top->right->val << ' ';
		}
		else
		{
			s.pop();
			last_pop = top;
		}
	}
}
```

# Hash(散列函数)

给定M，存在函数f(key)，对任意给定给定的关键字key，代入函数厚若能得到包含该关键字的记录在表中的地址，则称表M为哈希(Hash)表，函数(key)为哈希(Hash)函数。
如果key不相同，但是f(k1)=f(k2),这种现象成为碰撞(Collision)

## 哈希函数

1.直接寻址发：取关键字或关键字的某个线性函数值为散列地址。即H(key)=key或H(key) = a * key + b,其中a和b为常熟（这种散列函数叫做自身函数）。

