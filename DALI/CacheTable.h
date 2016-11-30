#pragma once

//缓存区结构
//双向链表 包含st和内容

class cacheNode {
private:
	int m_St;
	char m_IP[16];
	int m_Length;
	char m_Operate;
	char m_NodeBuffer[256];
	cacheNode *m_NextNode;
	cacheNode *m_PriorNode;
public:
	cacheNode() {
		m_St = 0;
		m_Operate = 0;
		m_Length = 0;
		memset(m_IP, 0, sizeof(m_IP));
		memset(&m_NodeBuffer, 0, sizeof(m_NodeBuffer));
		m_NextNode = NULL;
		m_PriorNode = NULL;
	}

	~cacheNode() {
		m_NextNode = NULL;
		m_PriorNode = NULL;
	}

	void setst(int socket) {
		m_St = socket;
	}

	int getst() {
		return m_St;
	}

	void setIP(char* ip, int length) {
		//strcpy(m_IP, ip);
		memcpy(m_IP, ip, length);
		strcat(m_IP, "\0");
	}

	void getIP(char *ip) {
		//memset(buffer, 0xff, 1);
		memcpy(ip, m_IP, sizeof(m_IP));
		strcat(ip, "\0");
	}

	void setLength(int length) {
		m_Length = length;
	}

	int getLength() {
		return m_Length;
	}

	void setOperate(int operate) {
		m_Operate = operate;
	}

	int getOperate() {
		return m_Operate;
	}

	void setbuffer(char* buffer, int length) {
		memcpy(m_NodeBuffer, buffer, length);
		strcat(m_NodeBuffer, "\0");
	}

	void getbuffer(char *buffer) {
		//memset(buffer, 0xff, 1);
		memcpy(buffer, m_NodeBuffer, sizeof(m_NodeBuffer));
		strcat(buffer, "\0");
	}
	void get20buffer(char *buffer) {
		//memset(buffer, 0xff, 1);
		memcpy(buffer, m_NodeBuffer, 20);
		strcat(buffer, "\0");
	}

	void get32buffer(char *buffer) {
		//memset(buffer, 0xff, 1);
		memcpy(buffer, m_NodeBuffer, 32);
		strcat(buffer, "\0");
	}

	void getbuffer(char *buffer, int length) {
		memcpy(buffer, m_NodeBuffer, length);
	}

	cacheNode *getNextNode() {
		return m_NextNode;
	}
	cacheNode *getPriorNode() {
		return m_PriorNode;
	}

	void setNextNode(cacheNode * nextnode) {
		m_NextNode = nextnode;
	}
	void setPriorNode(cacheNode *priornode) {
		m_PriorNode = priornode;
	}

	//node' prior and next point need init NULL,then copy
	void copyLinkNode(cacheNode *pLinkNode) {
		//if (pLinkNode->getPriorNode() == NULL && pLinkNode->getNextNode() == NULL) {
		//set buffer
		pLinkNode->setbuffer(m_NodeBuffer, sizeof(m_NodeBuffer));
		pLinkNode->setst(m_St);
		pLinkNode->setLength(m_Length);
		pLinkNode->setOperate(m_Operate);
		//}
	}

	void initBuffer() {
		memset(m_NodeBuffer, 0, sizeof(m_NodeBuffer));
	}
};

class CCacheTable
{
private:
	cacheNode *m_HeadNode;
	int m_NodeNum;
	int m_TotalNum;
	cacheNode *m_EndNode;
public:
	CCacheTable(int iNum) {
		m_TotalNum = iNum;
		m_NodeNum = 0;
		m_HeadNode = NULL;
		//m_HeadNode->setPriorNode(NULL);
		m_EndNode = NULL;
		//m_EndNode->setNextNode(NULL);
	}

	~CCacheTable() {}


	int getNodeNum(char operate) {
		cacheNode *pScanNode;
		pScanNode = m_HeadNode;
		int num = 0;

		if (m_NodeNum == 0)
			return 0;
		//寻找含有相同的的操作符的链表节点
		int n = m_NodeNum;
		while (n--) {
			//如果找到操作符匹配项
			if (pScanNode->getOperate() == operate) {
				num++;
			}
			else {
				pScanNode = pScanNode->getNextNode();
			}
		}
		pScanNode = NULL;
		return num;
	}


	//tou cha fa
	void insertNode(cacheNode *insertNode) {
		if (m_NodeNum != m_TotalNum) {
			if (m_HeadNode == NULL){
				m_HeadNode = insertNode;
				m_EndNode = insertNode;

			}
			else {
				cacheNode *pTempNode = m_HeadNode;
				//insert node
				m_HeadNode = insertNode;
				insertNode->setNextNode(pTempNode);
				pTempNode->setPriorNode(insertNode);

			}
			m_NodeNum++;
		}
	}

	bool AnalyzeNode() {
		if (m_NodeNum == 0){
			return false;
		}
		else {
			getEndNode();
			return true;
		}
	}


	cacheNode *getEndNode() {
		if (m_NodeNum > 1) {
			cacheNode *tempEndNode = m_EndNode;
			m_EndNode = m_EndNode->getPriorNode();
			m_EndNode->setNextNode(NULL);
			tempEndNode->setPriorNode(NULL);
			m_NodeNum--;
			return tempEndNode;
		}
		else if (m_NodeNum = 0) {
			return NULL;
		}
		else if (m_NodeNum = 1) {
			cacheNode *tempEndNode = m_EndNode;
			m_EndNode = NULL;
			m_HeadNode = NULL;
			m_NodeNum--;
			return tempEndNode;
		}
	}

	int getNodeNum() {
		return m_NodeNum;
	}

	//扫描链表，获得需要的项，并清除该项
	//以节点形式返回
	void scanTable(int operate, cacheNode *pCacheNode) {
		cacheNode *pScanNode;
		pScanNode = m_HeadNode;

		int i = m_NodeNum;
		if (m_NodeNum == 0)
			return;
		int flag = 0;
		//寻找含有相同的的操作符的链表节点
		while (pScanNode != m_EndNode) {
			flag = 1;
			//如果找到操作符匹配项
			if ((pScanNode->getOperate() & 0xFF) == operate) {
				//获取buffer
				memcpy(pCacheNode, pScanNode, sizeof(cacheNode));
				
				//删除该指定节点
				deleteNode(pScanNode);
				break;
			}
			else {
				pScanNode = pScanNode->getNextNode();
			}
		}
		if (flag == 0) {
			memcpy(pCacheNode, pScanNode, sizeof(cacheNode));
			//删除该指定节点
			deleteNode(pScanNode);
			//pScanNode = NULL;
		}
	}

	
	void deleteNode(cacheNode *pDeleteNode) {
		//获取当前节点的前一个节点
		//如果该节点的前一个节点为空，表示头结点
		if (pDeleteNode->getPriorNode() == NULL) {
			//如果该节点的后一个节点也为空， 表示链表只有一个节点
			if (pDeleteNode->getNextNode() == NULL) {
				//将头节点和尾节点都设为NULL
				m_HeadNode = NULL;
				m_EndNode = NULL;
			}
			//若后一个节点不为空，将头结点置为当前结点的后一个节点，尾节点不修改
			else {
				m_HeadNode = pDeleteNode->getNextNode();
				//m_HeadNode->setPriorNode(NULL);
				pDeleteNode->getNextNode()->setPriorNode(NULL);
			}
		}
		//如果该节点头结点不为空
		else {
			//获得当前结点的前一个节点
			//修改指针
			//前节点将后节点置删除节点的后一个节点
			pDeleteNode->getPriorNode()->setNextNode(pDeleteNode->getNextNode());
			//后节点将前节点置删除节点的前一个节点
			pDeleteNode->getNextNode()->setPriorNode(pDeleteNode->getPriorNode());
		}

		m_NodeNum--;

		//释放空间
		delete pDeleteNode;
		pDeleteNode = NULL;
	}

	//查找元素，通过buffer是否相同来判断
	bool findElement(char *pCacheBuffer) {
		cacheNode *pScanNode;
		pScanNode = m_HeadNode;

		if (m_NodeNum == 0)
			return false;

		char scanbuffer[60];
		char comparebuffer[60];
		memset(scanbuffer, 0, sizeof(scanbuffer));
		memset(comparebuffer, 0, sizeof(comparebuffer));
		//判断是否到结尾
		while (pScanNode == m_EndNode) {
			//先判断操作符
			pScanNode->getbuffer(scanbuffer + 7, 48);

			//判断buffer是否相同
			if (strcmp(scanbuffer, comparebuffer) == 0) {
				return true;
			}
			else {
				return false;
			}
		}
	}


	//扫描线程使用，返回相同节点个数
	int scanNode(int operate) {
		int nodeNum = 0;
		cacheNode *tempNode = m_HeadNode;
		//如果查询到最后一个节点
		if (m_NodeNum == 0)
			return 0;
		while (tempNode != m_EndNode) {
			if (tempNode->getOperate() == operate) {
				++nodeNum;
			}
			tempNode = tempNode->getNextNode();
		}

		if (tempNode->getOperate() == operate)
			++nodeNum;

		return nodeNum;
	}

	/* scanNode(int operate) {

	}*/

};

