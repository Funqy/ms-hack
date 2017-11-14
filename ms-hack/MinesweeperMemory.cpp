#include "MinesweeperMemory.h"



MinesweeperMemory::MinesweeperMemory()
{
	m_baseAddress = (BYTE*)0x1000000;
}


MinesweeperMemory::~MinesweeperMemory()
{
}

int MinesweeperMemory::Initialize()
{
	HWND hWnd = FindWindow(NULL, "MineSweeper");
	DWORD processID;
	GetWindowThreadProcessId(hWnd, &processID);

	m_hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, processID);

	return (int)m_hProc;
}

void MinesweeperMemory::Update()
{
	GetSizes();
	GetField();
}

void MinesweeperMemory::Cheat()
{
	SIZE_T bytesWritten;
	int val = 0;
	WriteProcessMemory(m_hProc, m_baseAddress + MMOVES_OFFSET, &val, 4, &bytesWritten);
}

void MinesweeperMemory::GetSizes()
{
	SIZE_T bytesWritten;
	ReadProcessMemory(m_hProc, m_baseAddress + MWIDTH_OFFSET, &m_fieldWidth, 4, &bytesWritten);
	ReadProcessMemory(m_hProc, m_baseAddress + MHEIGHT_OFFSET, &m_fieldHeight, 4, &bytesWritten);
}

void MinesweeperMemory::GetField()
{
	SIZE_T bytesWritten;
	ReadProcessMemory(m_hProc, m_baseAddress + MFIELD_OFFSET, &m_field, 0x340, &bytesWritten);
}

int MinesweeperMemory::CheckPosition(int x, int y)
{
	return (int)m_field[1 + x + 0x20 * y];
}

bool MinesweeperMemory::IsMine(int x, int y)
{
	return m_field[1 + x + 0x20 * y] == 0x8F;
}

bool MinesweeperMemory::IsEmpty(int x, int y) 
{
	return m_field[1 + x + 0x20 * y] == 0x0F;
}

void MinesweeperMemory::SetFieldHeight(int height)
{
	m_fieldHeight = height;
}

void MinesweeperMemory::SetFieldWidth(int width)
{
	m_fieldWidth = width;
}

int MinesweeperMemory::GetFieldHeight()
{
	return m_fieldHeight;
}

int MinesweeperMemory::GetFieldWidth()
{
	return m_fieldWidth;
}
