#pragma once

using CreateClientClass_t = void *( *)( int entnum, int serialNum );
using CreateEvent_t = void *( *)();

class ClientClass {
public:
	CreateClientClass_t m_pCreateFn;
	CreateEvent_t m_pCreateEventFn;
	char *m_pNetworkName;
	RecvTable *m_pRecvTable;
	ClientClass *m_pNext;
	int m_ClassID;
};
