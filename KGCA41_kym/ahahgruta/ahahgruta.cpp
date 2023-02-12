#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <string>

void main() 
{
	FILE* fp = fopen("kimdoe.txt", "r");
	if (fp == NULL){ puts("n파일이 존재하지 않습니다.");	return; }

	fseek(fp, 0, SEEK_END); // 파일의 바이트 수를 찾고 밑에 iSize에 저장
	int iSize = ftell(fp);
	printf("%d, %s\n", iSize, "바이트 입니다.");
	fseek(fp, 0, SEEK_SET);

		//char buffer[256];
		char buffer[100];
		char *origin = (char*)calloc(iSize, sizeof(char));
		//char origin[256];

		while (!feof(fp))
		{
			if (fgets(buffer, 256, fp) == NULL) break;
			strcat(origin, buffer);
		}

		printf("\n%s \n", origin);
		for (int i = strlen(origin); i > 0; i -= 2) 
		{ 
			//if (origin[i] > 0) { i--; }
			//printf("%c%c", origin[i-1], origin[i]); 

			while (origin[i] >= 0) { printf("%c", origin[i]); i--; }
			if (i > 0) { printf("%c%c", origin[i - 1], origin[i]); }
		}

}