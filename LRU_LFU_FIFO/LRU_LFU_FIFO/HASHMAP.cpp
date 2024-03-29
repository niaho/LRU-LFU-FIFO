/*
hashmap的c语言实现
疑问：
1.hashmap非线程安全？？？hashtable线程安全？？？
	a.hashMap去掉了HashTable 的contains方法，都有containsValue（）和containsKey（）方法。
	b.hashTable同步的，而HashMap是非同步的，效率上比hashTable要高。
	c.hashMap允许空键值，而hashTable不允许。
*/

#include "include.h"
#define HASH_SIZE 100
//数据键值对
typedef struct
{
	int		key;
	int		value;
} DataType;

//hash节点
typedef struct
{
	DataType	data;
	HashNode	*next; //hash冲突链
}HashNode;

//hashmap的控制结构
typedef struct
{
	int			size;
	HashNode	*table;
}HashMap,*hashmap;

int hash(int key)
{
	return abs(key) % HASH_SIZE;
}
//初始化HashMap
HashMap *Init_HashMap()
{
	HashMap* hashmap = (HashMap*)malloc(sizeof(HashMap));
	hashmap->size = HASH_SIZE;
	hashmap->table = (HashNode*)malloc(sizeof(HashNode)*HASH_SIZE);
	memset(hashmap->table,0x0, sizeof(HashNode)*HASH_SIZE);
	if (hashmap->table == NULL)
		exit(-1);
}

//向HashMap中Set数据
int Set_Value(HashMap *hashmap, int key,int value)
{
	int hash_val = hash(key);
	HashNode * pointer = &hashmap->table[hash_val];
	//如果hash桶的链为空
	if (!pointer->next)
	{
		pointer->data.value = value;
		pointer->data.key = key;
	}
	else
	{
		while (pointer->next)
		{
			pointer = pointer->next;
		}
		HashNode* node = (HashNode*)malloc(sizeof(HashNode));
		if(node == NULL)
			exit(-1);
		node->data.value = value;
		node->data.key = key;
		pointer->next = node;
		node->next = NULL;
	}
	return 1;
}
//从HashMap中Get键值
int Get_Value(HashMap *hashmap, int key)
{
	int hash_val = hash(key);
	HashNode * pointer = &hashmap->table[hash_val];
	if (pointer->data.key == key)
		return pointer->data.value;
	else
	{
		while(pointer->next)
		{
			if (pointer->data.key == key)
				return pointer->data.value;
			pointer = pointer->next;
		}
	}
	return -1;
}

//free HashMap
void hfree(HashMap *hashmap)
{
	for (int i = 0; i < HASH_SIZE; i++)
	{
		while (hashmap->table[i].next)
		{
			HashNode *pointer = &hashmap->table[i];
			hashmap->table[i] = *hashmap->table[i].next;
			free(pointer);
		}
			
	}
	free(hashmap);
}





