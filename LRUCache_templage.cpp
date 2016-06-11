/*
使用map+list实现了LRUcache的泛型版本
1、使用泛型修饰的iterator需要有typename修饰；
2、模板无法分离编译；
3、只有2个成员时，可以使用stl的pair，而不需要自己定义一个struct；
4、注意类模板和其成员函数的定义方式；
5、提供iterator，方便遍历；
*/
#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <functional>
using namespace std;
template <class Key, class Value>
class LRUCache
{
public:
	LRUCache(unsigned cache_size = 3): m_cache_max_size(cache_size){}
	
	bool set(const Key& key, const Value& value);
	bool get(Value& value, const Key& key);
	
	unsigned get_cache_max_size() const;
	unsigned get_cache_free_size() const;
	unsigned get_cache_used_size() const;
	unsigned get_cache_size() const;
	unsigned get_cache_map_size() const;
	
	void print_node(const pair<Key, Value>& node) const;
	void print();
	
	//iterator
	typedef typename list<pair<Key, Value> >::iterator iterator;
	iterator begin();
	iterator end();
private:
	map<Key, typename list<pair<Key, Value> >::iterator> m_cache_map;
	list<pair<Key, Value> > m_cache;
	unsigned m_cache_max_size;
};
template<class Key, class Value>
typename LRUCache<Key, Value>::iterator LRUCache<Key, Value>::begin()
{
	return m_cache.begin();
}
template<class Key, class Value>
typename LRUCache<Key, Value>::iterator LRUCache<Key, Value>::end()
{
	return m_cache.end();
}


template<class Key, class Value>
bool LRUCache<Key, Value>::set(const Key& key, const Value& value)
{
	typename map<Key, typename list<pair<Key, Value> >::iterator >::iterator it = m_cache_map.find(key);
	if(it != m_cache_map.end())
	{
		//节点存在，更新节点位置
		pair<Key, Value> node = *(it->second);
		node.second = value;
		
		m_cache.erase(it->second);
		
		m_cache.push_front(node);
		m_cache_map[key] = m_cache.begin();
		return true;
	}
	//节点不存在
	//检查cache是否已经满了,淘汰掉最后一个
	if(get_cache_used_size() == m_cache_max_size)
	{
		//cache已满，淘汰掉最后一个
		m_cache_map.erase(m_cache.back().first);
		m_cache.pop_back();
	}
	//cache未满
	pair<Key, Value> node;
	node.first = key;
	node.second = value;
	
	m_cache.push_front(node);
	m_cache_map[key] = m_cache.begin();
	
	return true;
}
template<class Key, class Value>
bool LRUCache<Key, Value>::get(Value& value, const Key& key)
{
	typename map<Key, typename list<pair<Key, Value> >::iterator >::iterator it = m_cache_map.find(key);
	if(it != m_cache_map.end())
	{
		//key存在，移动节点位置
		pair<Key, Value> node = *(it->second);
		
		m_cache.erase(it->second);
		m_cache.push_front(node);
		m_cache_map[key] = m_cache.begin();
		
		value = node.second;
		return true;
	}
	return false;
}
template<class Key, class Value>
unsigned LRUCache<Key, Value>::get_cache_max_size() const
{
	return m_cache_max_size;
}
template<class Key, class Value>
unsigned LRUCache<Key, Value>::get_cache_free_size() const
{
	return m_cache_max_size - m_cache.size();
}
template<class Key, class Value>
unsigned LRUCache<Key, Value>::get_cache_used_size() const
{
	return m_cache.size();
}
template<class Key, class Value>
unsigned LRUCache<Key, Value>::get_cache_size() const
{
	return m_cache.size();
}
template<class Key, class Value>
unsigned LRUCache<Key, Value>::get_cache_map_size() const
{
	return m_cache_map.size();
}
template<class Key, class Value>
void LRUCache<Key, Value>::print_node(const pair<Key, Value>& node) const 
{
	cout << "(" << node.first << ", " << node.second << ")" << endl;
}
template<class Key, class Value>
void LRUCache<Key, Value>::print()
{
	typedef typename list<pair<Key, Value> >::iterator list_iterator;
	list_iterator it = m_cache.begin();
	list_iterator end = m_cache.end();
	cout << "cache_max_size: " << get_cache_max_size() << endl;
	cout << "cache_free_size: " << get_cache_free_size() << endl;
	cout << "cache_used_size: " << get_cache_used_size() << endl;
	while(it != end)
	{
		print_node(*it);
		++it;
	}
}
/*
main函数如下：
#include <iostream>
#include <string>
#include "LRUCache_v2.h"
using namespace std;


int main(int argc, char* argv[])
{
	LRUCache<int, string> cache(2);
	cache.set(1, "a");
	cache.set(2, "b");
	cache.set(3, "c");
	cache.set(4, "d");
	cache.set(3, "f");
	cout << "init: " << endl;
	cache.print();
	string value;
	if(cache.get(value, 4))
	{
		cout << "after get key 1: " << endl;
		cout << "key 1: " << value << endl;
	}
	else
	{
		cout << "get key 1 fail" << endl;
	}
	cache.print();
	cache.get(value, 34);
	cache.print();
	
	//遍历
	cout << "iterate the cache:" << endl;
	cout << "cache_max_size: " << cache.get_cache_max_size() << endl;
	cout << "cache_free_size: " << cache.get_cache_free_size() << endl;
	cout << "cache_used_size: " << cache.get_cache_used_size() << endl;
	LRUCache<int, string>::iterator it = cache.begin();
	LRUCache<int, string>::iterator end = cache.end();
	while(it != end)
	{
		cache.print_node(*it);
		++it;
	}
	return 0; 
} 
*/
