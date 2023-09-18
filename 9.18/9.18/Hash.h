#pragma once
using namespace std;
#include <vector>
#include<iostream>







//��ϣͰ

//�õ���һ��ָ�����飬���涼�Ǵ��ָ��


//��ϣ���
template <class K,class V>
struct HashNode
{
	pair<K, V> _kv;
	//�洢������
	HashNode<K, V>* _next;
	//ָ����һ������ָ��

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
		//��keyǿ������ת��
		//�����������壺���Ը���Ҳ���Խ���ģ��
	}
};


//Ҫдһ���º�����  ��Ϊ�������е��������Ͷ�����ģ��
// һ�������ǿ���ģ�ģ�string�������޷�ģ��
// ��������Ҫдһ���º������ﵽ�������ݿ���ģ
//����Ҳ���������˹�ϣ���һ��ģ�����l
template<class K,class V>
//��ϣ��
class Hash_table
{
	typedef HashNode<K, V> Node;


	//��ϣ��Ҫ��д������������Ȼ�Զ�������vector���Զ�����Ĭ����������������ĳ�Ա���������ͣ�û��Ĭ�Ϲ��죬
	//������Ҫ�����Լ�����ÿ�����
public:
	~Hash_table()
	{
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* next = cur->_next;
				//Ҫ�ȱ�������
				delete cur;
				cur = next;
			}
			_table[i] = nullptr;
		}
	}
	Hash_table()
	{
		//����
		_table.resize(10, nullptr);
		//���ȿ���ʮ���ռ䣬ÿ���ռ�ֵΪ��
	}

	bool insert(const pair<K, V> _kv)
	{

		//�ڲ���֮ǰ��ȷ��һ�±����Ƿ��Ѿ��������ֵ��������˾Ͳ��ò�����
		if (find(_kv.first))
		{
			return false;
		}

		//�����Լ����������߼�����Ȼvector���Լ����ݣ�������Ҫ���ơ���Ϊ�����꣬�е�key���ͻ�е�ֵ�ֲ���ͻ�ˡ�
	   //��������ݣ���ô��ͻ���˾͸�������һ����
		//���������Ӵ�Լ����1ʱ��Ҫ���ݣ�ƽ��ÿ��Ͱ����һ������

		if (n == _table.size())
		{
			//������ݣ����¿��ռ�
			size_t newSize = _table.size() * 2;
			vector<Node*> newtable;
			newtable.resize(newSize, nullptr);

			//�����ٸ�������ķ������������ã���Ϊ�����ؿ��ռ䣬Ȼ����Ҫ�ͷţ�
			//����Ӧ�ý�ԭ���Ľ���ù���ʹ��
			//�������Ǳ����ɱ����ɱ�Ľ���ù�����ǩ���±���
			for (size_t i = 0; i < _table.size(); i++)
			{
				//���ݺ󣬿ռ�size����ˣ��е����ݾͿ��ܻ�浽��ͬ��Ͱ����
				//�������Ľ��Ҫ���¼���Ž��ĸ�λ��
				Node* cur = _table[i];
				//cur������ܻ������ӵĽ��
				while (cur)
				{
					size_t hashi = cur->_kv.first % newtable.size();
					

					Node* next = cur->_next;
					//ͷ�嵽�±�
					cur->_next = newtable[hashi];
					//ͷ�� ������� �ӵ��������ǰ��԰�
					//��ônext�ͽӵ�newtavle[i]
					newtable[hashi] = cur;

					//�����߽�������
					cur = next;
				}

				//��ǰͰ��Ľ�㱻�ù�󣬾���Ϊ��
				_table[i] = nullptr;


			}
			//����±����������Ҫ�ı���ô��������vector���Ľ������þɱ���±���

			_table.swap(newtable);
		}



		size_t hashi = _kv.first % _table.size();

		Node* newnode = new Node(_kv);
		newnode->_next = _table[hashi];
		_table[hashi] = newnode;
		//���½��ͷ�嵽��ϣͰ��
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

		//���Ը���find������find�ҵ�keyȻ��ɾ��key�أ�4
		//�����ԣ���Ϊɾ��һ�������Ҫ�ҵ��������ǰ��ͺ����λ�ã�������ֻ��key��λ�ã����Բ���ֱ�Ӹ���find�����Ǹ������еĲ���
		size_t hashi = key % _table.size();

		Node* cur = _table[hashi];
		Node* prev = nullptr;
		while (cur)
		{
			if (cur->_kv.first == key)//�ҵ�Ҫɾ���Ľ���
			{
				
				//��ǰ��Ľ���ָ��ָ������ǰ��

				//����һ�ֿ���cur����Ͱ��ĵ�һ������ô����ͷɾ�ˣ�prev����nullptr
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
				//ÿ���ȼ�¼һ��ǰ��Ľ��λ��
				cur = cur->_next;
			}
				
		}
		return false;

	}
private:
	//�ײ��װ����һ��ָ������
	//�������ָ��ָ��Ķ��ǹ�ϣ���
	vector<Node*> _table;
	//�ײ㻹��װһ����Сn��ʾʵ�ʱ��е����ݸ���
	size_t n=0;///�������㸺������
};
