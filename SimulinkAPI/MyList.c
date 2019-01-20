#include "MyList.h"
#include <stdlib.h>
#include <memory.h>

int MyList_Create(MyList_t *pList)
{
	MyListNode_t *pNode = NULL;

	if( pList == NULL){
		return -1;
	}
	pList->nCount = 0;
	
	pNode = (MyListNode_t *)malloc(sizeof(MyListNode_t));
	if( pNode == NULL){
		return -2;
	}
	pNode->nKey = 0;
	pNode->pData = NULL;
	pNode->pNext = NULL;
	pList->pHead = pNode;
	return 0;
}

MyListNode_t *MyList_Get(MyList_t *pList, int key)
{
	MyListNode_t *pNode = NULL;
	MyListNode_t *pFind = NULL;

	if( pList == NULL || pList->nCount <= 0){
		return NULL;
	}

	pNode = pList->pHead->pNext;
	while( pNode != NULL)
	{
		if( pNode->nKey == key){
			pFind = pNode;
			break;
		}
		pNode = pNode->pNext;
	}
	return pNode;
}

int MyList_Size(MyList_t *pList)
{
	return pList->nCount;
}

int MyList_Add(MyList_t *pList, int key, void *pData)
{
	MyListNode_t *pNode = NULL;
	MyListNode_t *pNewNode = NULL;

	if( pList == NULL){
		return -1;
	}

	pNode = MyList_Get(pList, key);
	if( pNode != NULL){
		return 1;
	}

	pNewNode = (MyListNode_t *)malloc(sizeof(MyListNode_t));
	if( pNewNode == NULL){
		return -2;
	}
	pNewNode->nKey = key;
	pNewNode->pData = pData;
	pNewNode->pNext = NULL;

	pNode = pList->pHead;
	while( pNode->pNext != NULL){
		pNode = pNode->pNext;
	}
	pNode->pNext = pNewNode;
	
	pList->nCount++;
	return 0;
}

int MyList_Del(MyList_t *pList, int key, MyList_FreeNodeDataCallback pFreeFun)
{
	MyListNode_t *p1;
	MyListNode_t *p2;

	if( pList == NULL || pList->nCount == 0){
		return -1;
	}

	p1 = pList->pHead->pNext;
	p2 = pList->pHead;
	while( p1 != NULL){
		if( p1->nKey == key){
			break;
		}
		p2 = p1;
		p1 = p1->pNext;
	}
	if( p1 != NULL){
		p2->pNext = p1->pNext;
		if( pFreeFun != NULL)
			pFreeFun(p1->pData);
		free(p1);
		pList->nCount--;
		return 0;
	}
	return -1;
}

int MyList_Destory(MyList_t *pList, MyList_FreeNodeDataCallback pFreeFun)
{
	MyListNode_t *pNode = NULL;
	MyListNode_t *pNode2 = NULL;

	if( pList == NULL){
		return -1;
	}

	pNode = pList->pHead->pNext;
	free(pList->pHead);
	pList->pHead = NULL;
	pList->nCount = 0;
	if( pNode != NULL){
		pNode2 = pNode;
		pNode = pNode->pNext;
		if( pFreeFun != NULL)
			pFreeFun(pNode2->pData);
		free(pNode2);
	}

	return 0;
}

int MyList_Print(MyList_t *pList, MyList_PrintNodeDataCallback pPrintFun)
{
	MyListNode_t *pNode = NULL;
	if( pList == NULL){
		return -1;
	}
	pNode = pList->pHead->pNext;

	printf("---------------------------nCount:%d------------------------- \n", pList->nCount);
	while( pNode != NULL){
		printf("Key=%d\n", pNode->nKey);
		if( pPrintFun != NULL)
			pPrintFun(pNode->pData);
		pNode = pNode->pNext;
	}
	printf("============================================================== \n", pList->nCount);
	return 0;
}