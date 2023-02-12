#include "Sample.h"

void   Sample::ChatMsg(USER_PACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str());
}
void   Sample::ChatNameReq(USER_PACKET& t)
{
	std::wstring fmt = L"%s";
	Print(fmt.c_str(), L"�г����� �Է��ϼ��� : ");
}
void   Sample::UserJoin(USER_PACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"���� �����Ͽ����ϴ�.");

}
void   Sample::NameAck(USER_PACKET& t)
{
	std::wstring fmt = L"%s%s";
	Print(fmt.c_str(), to_mw(t.msg).c_str(), L"�г��� ��� ����!");

}