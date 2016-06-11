/*
@function: map+list实现LRUCache
@time: 2016-06-10
支持如下功能：
1、set；
2、get；
3、del；
第一个版本，实现了基本功能。
感觉没弄清楚精髓所在！重构一个版本，写个泛型的版本！
*/
#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <stdio.h>
using namespace std;
struct Node
{
	string key;
	int value;
};
class LRUCache
{
public:
	LRUCache(unsigned cache_max_size = 10):m_cache_max_size(cache_max_size), m_free_size(cache_max_size), m_used_size(0)
	{
		cout << "LRUCache max size:" << m_cache_max_size << endl;
	}
	bool set(const Node& node);
	bool get(Node& node, const std::string& key);
	bool del(const std::string& key);
	unsigned get_used_size();
	unsigned get_free_size();
	unsigned get_cache_size();
	void print();
	void print_node(const list<Node>::iterator node_it);
private:
	map<const std::string, list<Node>::iterator> m_node_map;
	list<Node> m_node_list;
	unsigned m_cache_max_size;
	unsigned m_free_size;
	unsigned m_used_size;
};

/*
@function：插入节点到list中
@param1:待插入的节点
@return：false：插入失败，true：插入成功
*/
bool LRUCache::set(const Node& node)
{
	//查找当前节点是否存在
	typedef map<const std::string, list<Node>::iterator>::iterator mapiterator;
	mapiterator it = m_node_map.find(node.key);
	//节点存在，更新节点位置和值
	if(it != m_node_map.end())
	{
		m_node_list.erase(it->second);
		m_node_list.push_front(node);
		m_node_map[node.key] = m_node_list.begin();
		return true;
	}
	//不存在,检查cache是否已满
	if(m_free_size == 0)
	{
		m_node_list.pop_back();
		++m_free_size;
		--m_used_size;
	}
	m_node_list.push_front(node);
	m_node_map[node.key] = m_node_list.begin();
	--m_free_size;
	++m_used_size;
	return true;
}
/*
@function:查找指定节点是否存在
@param1:查找到的节点
@param2：查找的key
@return：false：查找失败，true：查找成功
*/
bool LRUCache::get(Node& node, const std::string& key)
{
	//查找节点是否存在
	typedef map<const std::string, list<Node>::iterator>::iterator mapiterator;
	mapiterator it = m_node_map.find(key);
	if(it == m_node_map.end())
	{
		return false;
	}
	//节点存在，更新节点位置
	node.key = key;
	node.value = it->second->value;
	m_node_list.erase(it->second);
	m_node_list.push_front(node);
	m_node_map[key] = m_node_list.begin();
	
	return true;
}

/*
@function：删除节点
@param1:待删除节点的key
@return：false：成功，true：失败
*/
bool LRUCache::del(const std::string& key)
{
	//查找对应节点是否存在
	typedef map<const std::string, list<Node>::iterator>::iterator mapiterator;
	mapiterator it = m_node_map.find(key);
	if(it == m_node_map.end())
	{
		//节点不存在
		return false;
	}
	m_node_list.erase(it->second);
	m_node_map.erase(it);

	--m_used_size;
	++m_free_size;
	
	return true;
}

/*
@function：获取当前已使用掉的节点数
*/
unsigned LRUCache::get_used_size()
{
	return m_used_size;
}
/*
@function：获取当前可用的节点数
*/
unsigned LRUCache::get_free_size()
{
	return m_free_size;
}
/*
@function: 获取cache的最大长度
*/
unsigned LRUCache::get_cache_size()
{
	return m_cache_max_size;
}
void LRUCache::print_node(const list<Node>::iterator node_it)
{
	cout << "(" << node_it->key << ", " << node_it->value << ")" << endl; 
}

void LRUCache::print()
{
	cout << "total_size: " << get_cache_size() << endl;
	cout << "used_size: " << get_used_size() << endl;
	cout << "free_size: " << get_free_size() << endl;
//	for_each(m_node_list.begin(), m_node_list.end(), print_node);

	typedef list<Node>::iterator list_it;
	list_it it = m_node_list.begin();
	list_it end = m_node_list.end();
	for(; it != end; ++it)
	{
		print_node(it);
	}
}

string itoa(int num)
{
	char buf[10] = {0};
	sprintf(buf, "%d", num);
	return string(buf);
}
int main()
{
	LRUCache cache;
	cache.print();
	Node node;
	for(int i = 0; i < 10; ++i)
	{
		node.key = itoa(i);
		node.value = i;
		cache.set(node);
	}
	cout << "after insert: " << endl;
	cache.print();
	
	node.key = "12";
	node.value = 1;
	if(!cache.set(node))
	{
		cout << "set (" << node.key << ", " << node.value << ") fail" << endl;
	}
	else
	{
		cout << "set (" << node.key << ", " << node.value << ") suc" << endl;
	}
	cout << "after set key:12" << endl;
	cache.print();
	
	node.key = "2";
	node.value = 2;
	if(!cache.set(node))
	{
		cout << "set (" << node.key << ", " << node.value << ") fail" << endl;
	}
	else
	{
		cout << "set (" << node.key << ", " << node.value << ") suc" << endl;
	}
	
	//测试查找
	if(!cache.get(node, "1"))
	{
		cout << "get node key:1 fail" << endl;
	}
	else
	{
		cout << "get node key:1 suc, " << "(" << node.key << ", " << node.value << ")" << endl;
	}
	cout << "after find key:1 " << endl;
	cache.print();
	
	if(!cache.get(node, "2"))
	{
		cout << "get node key:2 fail" << endl;
	}
	else
	{
		cout << "get node key:2 suc, " << "(" << node.key << ", " << node.value << ")" << endl;
	}
	cout << "after find key:2 " << endl;
	cache.print();
	

	//测试查找不存在的节点
	if(!cache.get(node, "3"))
	{
		cout << "get node key:3 fail" << endl;
	}
	else
	{
		cout << "get node key:3 suc, " << "(" << node.key << ", " << node.value << ")" << endl;

	}
	cout << "after find key:3 " << endl;
	cache.print();
	
	
	//测试删除
	if(!cache.del("1"))
	{
		cout << "del key:1 fail" << endl;
	}
	else
	{
		cout << "del key:1 suc" << endl;
	}
	cout << "after del key:1" << endl;
	cache.print();
	
	//gtest
	return 0;
}
