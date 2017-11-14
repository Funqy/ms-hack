#pragma once
#include <Windows.h>

#define MWIDTH_OFFSET 0x5334
#define MHEIGHT_OFFSET 0x5338
#define MFIELD_OFFSET 0x5360
#define MMOVES_OFFSET 0x57A4

class MinesweeperMemory
{
public:
	MinesweeperMemory();
	~MinesweeperMemory();
	
	int Initialize();
	void Update();
	void Cheat();

	void GetSizes();
	void GetField();

	int CheckPosition(int, int);
	bool IsMine(int, int);
	bool IsEmpty(int, int);

	void SetFieldHeight(int);
	void SetFieldWidth(int);

	int GetFieldHeight();
	int GetFieldWidth();
private:
	HANDLE m_hProc;
	BYTE* m_baseAddress;
	int m_fieldWidth, m_fieldHeight;
	BYTE m_field[0x340];
};

