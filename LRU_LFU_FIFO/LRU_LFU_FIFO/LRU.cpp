/*
LRU:最近最少使用算法(Least Recently used -LRU)

实现一个双向链表
用哈希记录key对应的node
总是删除末尾节点
新访问的节点总是从原位置移动到头部
*/
#include "include.h"

typedef struct 
{

	int value;
	int key;

	LRUCache * next;
	LRUCache * prev;

} LRUCache;


LRUCache* lRUCacheCreate(int capacity/*容量的意思*/) 
{

	LRUCache* cache = (LRUCache*)malloc(capacity * sizeof(LRUCache));
	if (!cache)
		return 0;
	else
	{
		memset(cache, 0x0, (capacity * sizeof(LRUCache)));
		//初始化链表
		//todo
		return cache;
	}
}

int lRUCacheGet(LRUCache* obj, int key) 
{
	//通过key在obj中找到对象
}

void lRUCachePut(LRUCache* obj, int key, int value) 
{
	//通过key和value制作hash放在obj中
	//判断obj是否满cache，若满删掉最后一个，未满
}

void lRUCacheFree(LRUCache* obj)
{
	//释放最后一个对象
}
