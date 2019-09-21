/*
LRU:�������ʹ���㷨(Least Recently used -LRU)

ʵ��һ��˫������
�ù�ϣ��¼key��Ӧ��node
����ɾ��ĩβ�ڵ�
�·��ʵĽڵ����Ǵ�ԭλ���ƶ���ͷ��
*/
#include "include.h"

typedef struct 
{

	int value;
	int key;

	LRUCache * next;
	LRUCache * prev;

} LRUCache;


LRUCache* lRUCacheCreate(int capacity/*��������˼*/) 
{

	LRUCache* cache = (LRUCache*)malloc(capacity * sizeof(LRUCache));
	if (!cache)
		return 0;
	else
	{
		memset(cache, 0x0, (capacity * sizeof(LRUCache)));
		//��ʼ������
		//todo
		return cache;
	}
}

int lRUCacheGet(LRUCache* obj, int key) 
{
	//ͨ��key��obj���ҵ�����
}

void lRUCachePut(LRUCache* obj, int key, int value) 
{
	//ͨ��key��value����hash����obj��
	//�ж�obj�Ƿ���cache������ɾ�����һ����δ��
}

void lRUCacheFree(LRUCache* obj)
{
	//�ͷ����һ������
}
