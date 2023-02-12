#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>

struct k_Student {
	int		m_iNum;
	int		m_iAge;
	int		m_iKor;
	int		m_iEng;
	int		m_iMath;
	int		m_iTotal;
	float	m_fAverage;
	char	m_sName[4];

	k_Student();
	~k_Student();
};

