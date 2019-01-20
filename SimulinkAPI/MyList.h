#ifndef  __SL_MYLIST_H__
#define __SL_MYLIST_H__
#include <stdio.h>

typedef void(*MyList_FreeNodeDataCallback)(void *pNodeData);
typedef void(*MyList_PrintNodeDataCallback)(void *pNodeData);

// List�ڵ㶨��
struct tagMyListNode{
	int  nKey;
	void *pData;
	struct tagMyListNode *pNext;
};
typedef struct tagMyListNode MyListNode_t;

// List����ͷ��㲻��������
typedef struct tagMyList{
	int nCount;
	MyListNode_t *pHead;
}MyList_t;

// List�ӿں���
int MyList_Create(MyList_t *pList);
MyListNode_t *MyList_Get(MyList_t *pList, int key);
int MyList_Size(MyList_t *pList);
int MyList_Add(MyList_t *pList, int key, void *pData);
int MyList_Del(MyList_t *pList, int key, MyList_FreeNodeDataCallback pFreeFun);
int MyList_Destory(MyList_t *pList, MyList_FreeNodeDataCallback pFreeFun);
int MyList_Print(MyList_t *pList, MyList_PrintNodeDataCallback pPrintFun);

#endif