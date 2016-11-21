#pragma once
#include "CacheTable.h"

static HANDLE recvMutex;
static HANDLE analMutex;

//分析处理类
class CAnalyzeMessage
{
public:
	CAnalyzeMessage();
	~CAnalyzeMessage();

	void scanAnalzyeCache(CCacheTable* analCacheTable, int operate, cacheNode *pCacheNode);

	void startAnlayzeMessage(CCacheTable* analCacheTable, cacheNode *recvNode, cacheNode* analNode);

	void changeBuffer(char *buffer, int start, int length, char *recvBuffer);

	bool findElement(CCacheTable* analCacheTable, cacheNode *pCacheNode);
};


