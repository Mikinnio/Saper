#include <stdlib.h>
#include "Square.h"
#include "Settings.h"
#include "Files.h"
#include <iostream>

using namespace std;

int CalculateMine(int x, int y);
int CalculateFlags(int x, int y);
int CalculateOpen();

int gameState;
int mineNumber;
int flagNumber;

SQUARE **fields;

void CreateSquareFields()
{
	gameState = KEEP_PLAYING;

	fields = new SQUARE *[GetFieldVertical()];
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		fields[i] = new SQUARE[GetFieldHorizontal()];
	}
}

void LoadSquareFields(string map)
{
	mineNumber = 0;
	flagNumber = 0;

	ReadFile(map);
	SetMineNumber(mineNumber);
	SetRemainingMines(GetMineNumber() - flagNumber);
	CalculateNearMine();
	CalculateNearFlags();
}

void SetFieldFromFile(int i, int j, int number)
{
	while (number > 0)
	{
		if (number >= 4)
		{
			fields[i][j].hasFlag = true;
			number = number - 4;
			flagNumber++;
		}
		else if (number >= 2)
		{
			fields[i][j].hasMine = true;
			number = number - 2;
			mineNumber++;
		}
		else if (number == 1)
		{
			fields[i][j].isOpen = true;
			number = number - 1;
		}
	}
}

void DeleteSquareFields()
{
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		delete[]  fields[i];
	}

	delete[] fields;
}

bool IsOpen(int x, int y)
{
	return fields[x][y].isOpen;
}

bool HasFlag(int x, int y)
{
	return fields[x][y].hasFlag;
}

bool HasMine(int x, int y)
{
	return fields[x][y].hasMine;
}

bool MineExplode(int x, int y)
{
	return fields[x][y].mineExplode;
}

int NearMine(int x, int y)
{
	return fields[x][y].nearMine;
}

POSITION FieldPosition(int x, int y)
{
	return fields[x][y].coordinates;
}

int GetGameState()
{
	return gameState;
}

void GenerateField()
{
	int x1, y1, x2, y2;
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			x1 = GetMarginPixels() + (j * GetFieldPixels()) + (j * GetBreakPixels());
			y1 = GetMarginPixels() + (i * GetFieldPixels()) + (i * GetBreakPixels()) + GetInfoPixels();
			x2 = GetMarginPixels() + ((j + 1) * GetFieldPixels()) + (j * GetBreakPixels());
			y2 = GetMarginPixels() + ((i + 1) * GetFieldPixels()) + (i * GetBreakPixels()) + GetInfoPixels();

			fields[i][j].coordinates.x1 = x1;
			fields[i][j].coordinates.y1 = y1;
			fields[i][j].coordinates.x2 = x2;
			fields[i][j].coordinates.y2 = y2;
		}
	}
}

void GenerateMine()
{
	int xRandom, yRandom;
	int mine = GetMineNumber();

	while (mine > 0)
	{
		xRandom = rand() % GetFieldVertical();
		yRandom = rand() % GetFieldHorizontal();

		if ((fields[xRandom][yRandom].hasMine == false) && (fields[xRandom][yRandom].isOpen == false))
		{
			fields[xRandom][yRandom].hasMine = true;
			mine--;
		}
	}
}

void CalculateNearMine()
{
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			if (fields[i][j].hasMine == false)
			{
				fields[i][j].nearMine = CalculateMine(i, j);
			}
		}
	}
}

int CalculateMine(int x, int y)
{
	int howMany = 0;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (!(i < 0 || i >= GetFieldVertical() || j < 0 || j >= GetFieldHorizontal()))
			{
				if (fields[i][j].hasMine == true)
				{
					howMany++;
				}
			}
		}
	}

	return howMany;
}

int YCalculate(int xMouse)
{
	int myY = -1;
	for (int i = 0; i < GetFieldHorizontal(); i++)
	{
		int x1 = fields[0][i].coordinates.x1;
		int x2 = fields[0][i].coordinates.x2;

		if ((x1 <= xMouse) && (x2 >= xMouse))
		{
			myY = i;
			break;
		}
	}

	return myY;
}

int XCalculate(int yMouse)
{
	int myX = -1;
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		int y1 = fields[i][0].coordinates.y1;
		int y2 = fields[i][0].coordinates.y2;

		if ((y1 <= yMouse) && (y2 >= yMouse))
		{
			myX = i;
			break;
		}
	}

	return myX;
}

void CalculateNearFlags()
{
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			fields[i][j].nearFlag = CalculateFlags(i, j);
		}
	}
}

int CalculateFlags(int x, int y)
{
	int howMany = 0;
	for (int i = x - 1; i <= x + 1; i++)
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			if (!(i < 0 || i >= GetFieldVertical() || j < 0 || j >= GetFieldHorizontal()))
			{
				if (fields[i][j].hasFlag == true)
				{
					howMany++;
				}
			}
		}
	}

	return howMany;
}

void OpenAllMines()
{
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			SQUARE myField = fields[i][j];

			if (myField.hasMine == true && myField.hasFlag == false)
			{
				fields[i][j].isOpen = true;
			}
		}
	}
}

void OpenCloseSingleField(int x, int y)
{
	if (!(x < 0 || x >= GetFieldVertical() || y < 0 || y >= GetFieldHorizontal()) && gameState == KEEP_PLAYING)
	{
		SQUARE myField = fields[x][y];

		if (myField.isOpen == false && myField.hasMine == false && myField.hasFlag == false)
		{
			fields[x][y].isOpen = true;
		}
		else
		{
			fields[x][y].isOpen = false;
		}
	}
}

void SetMineFlag(int x, int y)
{
	if (!(x < 0 || x >= GetFieldVertical() || y < 0 || y >= GetFieldHorizontal()) && gameState == KEEP_PLAYING)
	{
		SQUARE myField = fields[x][y];

		if (myField.hasMine == false && myField.hasFlag == false && myField.isOpen == false)
		{
			fields[x][y].hasMine = true;
			AddMineNumber();
		}
		else if (myField.hasMine == true && myField.hasFlag == false && myField.isOpen == false)
		{
			fields[x][y].hasFlag = true;
		}
		else if (myField.hasMine == true && myField.hasFlag == true && myField.isOpen == false)
		{
			fields[x][y].hasMine = false;
			fields[x][y].hasFlag = false;
			SubMineNumber();
		}
	}
}

void OpenField(int x, int y)
{
	if (!(x < 0 || x >= GetFieldVertical() || y < 0 || y >= GetFieldHorizontal()) && gameState == KEEP_PLAYING)
	{
		SQUARE myField = fields[x][y];

		if (myField.isOpen == false)
		{
			if ((myField.hasMine == true) && (myField.hasFlag == false))
			{
				gameState = YOU_LOSE;
				OpenAllMines();
				fields[x][y].mineExplode = true;
			}
			else
			{
				if (myField.nearMine > 0 && myField.hasMine == false)
				{
					fields[x][y].isOpen = true;
				}
				else if (myField.nearMine == 0 && myField.hasMine == false)
				{
					fields[x][y].isOpen = true;

					OpenField(x - 1, y + 1);
					OpenField(x, y + 1);
					OpenField(x + 1, y + 1);

					OpenField(x - 1, y);
					OpenField(x + 1, y);

					OpenField(x - 1, y - 1);
					OpenField(x, y - 1);
					OpenField(x + 1, y - 1);
				}
			}
		}
	}

	if (CalculateOpen() == (GetFieldHorizontal() * GetFieldVertical()) - GetMineNumber() && gameState == KEEP_PLAYING)
	{
		gameState = YOU_WIN;
	}
}

void AddFlag(int x, int y)
{
	SQUARE myField = fields[x][y];

	if ((myField.hasFlag == false) && (myField.isOpen == false))
	{
		fields[x][y].hasFlag = true;
		SubRemainigMines();
	}
	else if ((myField.hasFlag == true) && (myField.isOpen == false))
	{
		fields[x][y].hasFlag = false;
		AddRemainigMines();
	}
}

int CalculateOpen()
{
	int howMany = 0;
	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			if (fields[i][j].isOpen == true)
			{
				howMany++;
			}
		}
	}

	return howMany;
}

int ReturnFieldValue(int x, int y)
{
	int fieldValue = 0;

	if (fields[x][y].isOpen == true)
	{
		fieldValue += 1;
	}

	if (fields[x][y].hasMine == true)
	{
		fieldValue += 2;
	}

	if (fields[x][y].hasFlag == true)
	{
		fieldValue += 4;
	}

	return fieldValue;
}