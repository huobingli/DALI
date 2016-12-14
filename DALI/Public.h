#pragma once
#include <string>
class CPublic
{
public:
	static int triggerFlag;
	static int readyFlag;
	static int quitThreadFlag;
	static int controlFlag;
	CPublic() {
		
	}
	~CPublic();
};

