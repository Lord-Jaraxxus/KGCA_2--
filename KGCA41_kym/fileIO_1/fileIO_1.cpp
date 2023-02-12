#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include	<conio.h>
///////////////////////////////////////////////////////////////////
// 파일을 읽어들여 16진수와 문자로 출력한다.
///////////////////////////////////////////////////////////////////
void DrawHexAscii(char* file)
{
	unsigned char munja[10];
	int i, j, n = 0;
	FILE* fp;
	fp = fopen(file, "r");// 원래 rb라서 2진파일만 읽혔음; 
	if (fp == NULL)
	{
		puts("n파일이 존재하지 않습니다.");
		return;
	}

	while (!feof(fp)) // 파일의 끝이 아닐때까지, 즉 파일이 끝날때까지 반복
	{
		printf("\n%05d : ", ftell(fp));
		for (i = 0; i < 10; i++)
		{
			munja[i] = fgetc(fp);
			printf("%02X", munja[i]); //- 1바이트를 16진수로 출력
			if (feof(fp)) break;
		}
		printf("  ");
		for (j = 0; j < i; j++)
		{
			if (munja[j] == '\t' || munja[j] == '\n' || munja[j] == '\r')
			{
				munja[j] = '.';
			}
			putchar(munja[j]);
		}
		n++;
		if (n % 20 == 0)
		{
			puts("\n< 아무키나 누르세여 >\n"), _getch();
		}
	}
}
///////////////////////////////////////////////////////////////////
// 파일을 로딩하여 역순으로 출력한다.
///////////////////////////////////////////////////////////////////
void DrawInverse(char* file)
{

	FILE* fp;
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		puts("n파일이 존재하지 않습니다.");
		return;
	}

	fseek(fp, 0, SEEK_END); //- 파일포인터를 3번째 인자(SEEK_END)에서 두번째 인자(0)만큼 이동
	int iSize = ftell(fp);
	printf("%d, %s\n", iSize, "바이트 입니다.");
	fseek(fp, 0, SEEK_SET);

	int n = 0;
	do {
		char* munja = 0;
		munja = (char*)calloc(256, sizeof(char));

		int i, j;

		for (i = 0; i < 256; i++)
		{
			if (!feof(fp))
			{
				munja[i] = fgetc(fp);
			}
			else
			{
				break;
			}

			//int iCnt = fseek(fp, 2, SEEK_CUR );
			if (munja[i] == '\0' || ftell(fp) <= 0) //- if (munja[i] == '\n' || ftell(fp) <= 0)   <-원래코드
			{
				printf("어서도망쳐");
				break;
			}
		}
		for (j = i; j+1> 0; j--)
		{
			putchar(munja[j]);
		}
		n++;
		free(munja);
		if (n % 20 == 0)
		{
			puts("\n< 아무키나 누르세여 >\n"), _getch();
		}
	} while (!feof(fp));
}
void main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("\n 사용법 : fileOf.exe filePath 0(Inverse) or 1(Hex) \n");
		return;
	}
	int iValue = atoi(argv[2]);
	if (iValue == 1)
	{
		DrawHexAscii(argv[1]);
	}
	else
	{
		DrawInverse(argv[1]);
	}
}