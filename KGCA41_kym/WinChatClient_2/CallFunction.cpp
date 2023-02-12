#include "Sample.h"

void   Sample::ChatMsg(USER_PACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str());
}
void   Sample::ChatNameReq(USER_PACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), L"닉네임을 입력하세요 : ");
}
void   Sample::UserJoin(USER_PACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"님이 입장하였습니다.");

}
void   Sample::NameAck(USER_PACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"닉네임 사용 승인!");

}