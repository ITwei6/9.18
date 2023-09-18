#pragma once
using namespace std;
#include <vector>
#include<iostream>







//哈希桶

//用的是一个指针数组，里面都是存的指针


//哈希结点
template <class K,class V>
struct HashNode
{
	pair<K, V> _kv;
	//存储的数据
	HashNode<K, V>* _next;
	//指向下一个结点的指针

	HashNode(const pair<K,V> kv)
		:_kv(kv)
		,_next(nullptr)
	{}
};

template <class K>
struct defaulthashfunc
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
		//将key强行类型转化
		//这样作的意义：可以负数也可以进行模了
	}
};


//要写一个仿函数？  因为不是所有的数据类型都可以模的
// 一般整数是可以模的，string类型是无法模的
// 所以我们要写一个仿函数来达到传的数据可以模
//这样也就是增加了哈希表的一个模板参数l
template<class K,class V>
//哈希表
class Hash_table
{
	typedef HashNode<K, V> Node;


	//哈希需要将写析构函数，虽然自定义类型vector会自动调用默认析构，但它里面的成员是内置类型，没有默认构造，
	//所以需要我们自己析构每个结点
public:
	~Hash_table()
	{
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* next = cur->_next;
				//要先保存起来
				delete cur;
				cur = next;
			}
			_table[i] = nullptr;
		}
	}
	Hash_table()
	{
		//构造
		_table.resize(10, nullptr);
		//首先开出十个空间，每个空间值为空
	}

	bool insert(const pair<K, V> _kv)
	{

		//在插入之前，确认一下表里是否已经有了这个值，如果有了就不用插入了
		if (find(_kv.first))
		{
			return false;
		}

		//我们自己控制扩容逻辑，虽然vector会自己扩容，但我们要控制。因为扩容完，有的key会冲突有的值又不冲突了。
	   //如果不扩容，那么冲突多了就根单链表一样了
		//当负载因子大约等于1时就要扩容，平均每个桶里有一个数据

		if (n == _table.size())
		{
			//异地扩容，重新开空间
			size_t newSize = _table.size() * 2;
			vector<Node*> newtable;
			newtable.resize(newSize, nullptr);

			//不能再复用下面的方法，这样不好，因为就又重开空间，然后又要释放，
			//我们应该将原来的结点拿过来使用
			//所以我们遍历旧表，将旧表的结点拿过来，签到新表上
			for (size_t i = 0; i < _table.size(); i++)
			{
				//扩容后，空间size变大了，有的数据就可能会存到不同的桶里了
				//拿下来的结点要重新计算放进哪个位置
				Node* cur = _table[i];
				//cur后面可能还有链接的结点
				while (cur)
				{
					size_t hashi = cur->_kv.first % newtable.size();
					

					Node* next = cur->_next;
					//头插到新表
					cur->_next = newtable[hashi];
					//头插 这个结点的 接到插入结点的前面对吧
					//那么next就接到newtavle[i]
					newtable[hashi] = cur;

					//往后走接着拿走
					cur = next;
				}

				//当前桶里的结点被拿光后，就置为空
				_table[i] = nullptr;


			}
			//这个新表就是我们想要的表，那么我们利用vector、的交换，让旧表和新表交换

			_table.swap(newtable);
		}



		size_t hashi = _kv.first % _table.size();

		Node* newnode = new Node(_kv);
		newnode->_next = _table[hashi];
		_table[hashi] = newnode;
		//将新结点头插到哈希桶里
		++n;
		return true;
	}
	Node* find(const K& key)
	{
		size_t hashi = key % _table.size();

		Node* cur = _table[hashi];
		
		while (cur)
		{
			if (cur->_kv.first == key)
				return cur;
			else
				cur = cur->_next;
		}
		return nullptr;
	}
	void Print()
	{
		for (int i = 0; i < _table.size(); i++)
		{
			printf("[%d]", i);

			Node* cur = _table[i];
			while (cur)
			{
				cout << cur->_kv.first << " ";
				cur = cur->_next;
			}
			cout << endl;
			}

	}
	bool erase(const K& key)
	{

		//可以复用find吗？先用find找到key然后删除key呢？4
		//不可以，因为删除一个结点需要找到这个结点的前面和后面的位置，但这里只有key的位置，所以不能直接复用find，但是复用其中的步骤
		size_t hashi = key % _table.size();

		Node* cur = _table[hashi];
		Node* prev = nullptr;
		while (cur)
		{
			if (cur->_kv.first == key)//找到要删除的结点后
			{
				
				//将前面的结点的指针指向后面的前面

				//还有一种可能cur就是桶里的第一个，那么就是头删了，prev就是nullptr
				if (prev == nullptr)
				{
					_table[hashi] = cur->_next;
				}
				else
				{
					prev->_next = cur->_next;
				}
			
				delete cur;
				return true;
			}
			else
			{
				prev = cur;
				//每次先记录一下前面的结点位置
				cur = cur->_next;
			}
				
		}
		return false;

	}
private:
	//底层封装的是一个指针数组
	//数组里的指针指向的都是哈希结点
	vector<Node*> _table;
	//底层还封装一个大小n表示实际表中的数据个数
	size_t n=0;///用来计算负载因子
};
