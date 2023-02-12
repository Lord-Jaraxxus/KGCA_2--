//#include "Sample.h"
//
//void   Sample::ChatMsg(USER_PACKET& t)
//{
//	std::wstring fmt = L"%s";
//	Print(fmt.c_str(), to_mw(t.msg).c_str());
//    m_Net.AddSend(t.msg,t.ph.len, PACKET_CHAR_MSG);
//}
//
//void   Sample::NameReq(USER_PACKET& t)
//{
//    USER_PACKET packet;
//    m_Net.AddSend(t.msg, t.ph.len, PACKET_USER_JOIN);
//}

#include "K_Network.h"
void   K_Network::ChatMsg(USER_PACKET& t)
{
    m_Print({ to_mw(t.msg) });
    AddSend(t.msg, t.ph.len, PACKET_CHAR_MSG);
}

void   K_Network::NameReq(USER_PACKET& t)
{
   m_Print({ to_mw(t.msg) + L"¥‘¿Ã ¿‘¿Â«œºÃΩ¿¥œ¥Ÿ."});
   AddSend(t.msg, t.ph.len, PACKET_USER_JOIN);
}