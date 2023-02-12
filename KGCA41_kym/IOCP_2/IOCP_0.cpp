
#include <windows.h>
#include <iostream>
#include <process.h>//_beginthreadex

HANDLE  g_hIOCP;							// C?P 핸들, 여기에 파일 핸들들이 포트번호로 묶여있다
HANDLE  g_hEventFinish;						// 파일을 다 읽고 썼다면 시그널을 보내는 이벤트
HANDLE	hReadFile;							// 읽기버전 파일 핸들
HANDLE	hWriteFile;							// 쓰기버전 파일 핸들
const DWORD g_dwMaxReadSize = 4096*4096;	// 한번에 읽고 쓰는 최대크기
//const DWORD g_dwMaxWriteSize = 4096;
wchar_t* g_pFileBuffer = nullptr;			// 읽어온 데이터를 저장할 버퍼

OVERLAPPED readOV = { 0, };					// 오버랩 구조체, 오프셋? 대충 read의 진행상황 같은?
OVERLAPPED writeOV = { 0, };				// 오버랩 구조체, 
LARGE_INTEGER g_LoadFileSize;				// 파일의 총 크기
LARGE_INTEGER g_LargerRead = { 0, };		// 오버랩 구조체에 바로 더하기 뭣하니까 임시로 거치는 놈 같은디 음
LARGE_INTEGER g_LargerWrite = { 0, };		// 얘들을 안쓰고 싶었는데 오히려 더 길어지네; 

unsigned WINAPI WorkProc(LPVOID arg)
{	
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOV;

	DWORD dwReadOffset = g_dwMaxReadSize;
	DWORD dwWriteOffset = g_dwMaxReadSize;
	while (1)
	{
		DWORD dwEvent = WaitForSingleObject(g_hEventFinish, 0);
		if (dwEvent == WAIT_OBJECT_0) { break; };

		// 비동기 읽기 완성 여부 판단
		BOOL bRet = ::GetQueuedCompletionStatus( g_hIOCP,		// 포트번호가 바인딩된 핸들
												 &dwTransfer,	// 완료된 I/O 작업에서 전송된 바이트 수를 수신하는 변수에 대한 포인터	
												 &KeyValue,		// 완료된 비동기 뭐시기의 포트번호
												 &pOV,			// 완료된 비동기 뭐시기의 오버랩 구조체 (지금은 오프셋)
												 INFINITE );	// 타임아웃, 지금은 무한이니까 뭐라도 받을때까지 대기인가
		
		// 읽기 또는 쓰기 비동기 완성
		if (bRet== TRUE)
		{
			// 읽기 끗
			if (KeyValue == 1000)
			{
				LARGE_INTEGER Read;
				Read.LowPart = readOV.Offset;
				Read.HighPart = readOV.OffsetHigh;
				LARGE_INTEGER Write;
				Write.LowPart = writeOV.Offset;
				Write.HighPart = writeOV.OffsetHigh;

				Read.QuadPart += dwTransfer;
				readOV.Offset = Read.LowPart;
				readOV.OffsetHigh = Read.HighPart;

				DWORD dwTrans = 0;
				wchar_t* pOffsetData = &g_pFileBuffer[Write.QuadPart];
				BOOL ret = ::WriteFile( hWriteFile, pOffsetData, 
					dwTransfer, &dwTrans, &writeOV);

			}

			// 쓰기 끗
			if (KeyValue == 2000)
			{
				if (readOV.Offset == g_LoadFileSize.QuadPart)
				{
					::SetEvent(g_hEventFinish);
				}
				else
				{
					LARGE_INTEGER Read;
					Read.LowPart = readOV.Offset;
					Read.HighPart = readOV.OffsetHigh;
					LARGE_INTEGER Write;
					Write.LowPart = writeOV.Offset;
					Write.HighPart = writeOV.OffsetHigh;

					Write.QuadPart += dwTransfer;
					readOV.Offset = Write.LowPart;
					readOV.OffsetHigh = Write.HighPart;

					wchar_t* pOffsetData = &g_pFileBuffer[Read.QuadPart];


					DWORD dwRead;
					BOOL ret = ::ReadFile(hReadFile, pOffsetData,
						dwReadOffset, &dwRead, &readOV);
				}
			}
		}	
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_HANDLE_EOF)
			{
				break;
			}
			else
			{
				int k = 0;
			}
		}
	}
	return 0;
}

void main()
{  
	g_hEventFinish = ::CreateEvent(0, TRUE, FALSE, 0);

	std::wstring readfile = L"lemon.mp3";
	std::wstring writefile = L"melon.mp3";

	hReadFile = CreateFile(readfile.c_str(),
		GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hReadFile == INVALID_HANDLE_VALUE){ return; }

	hWriteFile = CreateFile(writefile.c_str(),
		GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hWriteFile == INVALID_HANDLE_VALUE) { return; }


	// CP생성하고 위에 파일 핸들이랑 포트번호랑 바인딩
	g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0,0,0);
	::CreateIoCompletionPort(hReadFile, g_hIOCP, 1000, 0);
	::CreateIoCompletionPort(hWriteFile, g_hIOCP, 2000, 0);
	
	// 파일의 사이즈를 가져와서 g_LoadFileSize에 리턴받음
	::GetFileSizeEx(hReadFile, &g_LoadFileSize);
	// 버퍼의 용량의 잔여량이 입출력 단위보다 작으면 오류가 발생한다. 
	g_pFileBuffer = new wchar_t[g_LoadFileSize.QuadPart]; //파일과 동일한 크기의 버퍼를 new해줌

	// 쓰레드 ON.. 인가? 
	unsigned int idRead;
	unsigned long hWorkThread =
		_beginthreadex(NULL, 0, WorkProc, (void*)readfile.c_str(), 0, &idRead);

	// 여기부터 이해가 힘들단 말이지.. 오프셋이 뭐여 대체 -> 파일에서 읽기를 시작할 위치
	g_LargerRead.QuadPart += 0;
	readOV.Offset = g_LargerRead.LowPart;
	readOV.OffsetHigh = g_LargerRead.HighPart;

	DWORD dwRead;
	BOOL ret = ::ReadFile(	hReadFile,			// 핸들, 이 경우에는 파일의 핸들
							g_pFileBuffer,		// 읽어온 걸 저장할 버퍼
							g_dwMaxReadSize,	// 읽을 최대 바이트 수
							&dwRead,			// 읽은 바이트 수를 받는 변수에 대한 포인터
							&readOV);			// 오버랩 구조체, 여기선 파일에서 읽기를 시작할 위치 역할


	/*while (1)
	{
		Sleep(100);
		float fCurentSize = (float)g_LargerRead.QuadPart / (float)g_LoadFileSize.QuadPart;
		std::cout << fCurentSize * 100.0f << "\n";
		if (g_LargerRead.QuadPart == g_LoadFileSize.QuadPart)
		{
			break;
		}
	}*/

	WaitForSingleObject((HANDLE)hWorkThread, INFINITE); // 스레드가 끝날때까지 대기?
	CloseHandle((HANDLE)hWorkThread);


	CloseHandle(hReadFile);
	CloseHandle(hWriteFile);

	delete g_pFileBuffer;
}
