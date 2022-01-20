#include <allegro5\allegro.h>
#include <cstdlib>
#include <string>
#include "Settings.h"

using namespace std;

int fieldVertical = 0;
int fieldHorizontal = 0;

int AAAmineNumber = 0;
int remainingMines = 0;
int gameTime = 0;

int marginPixels = 10;
int fieldPixels = 24;
int breakPixels = 0;
int infoPixels = 40;

int monitorWidth = 0;
int monitorHeight = 0;
int displayWidth = 0;
int displayHeight = 0;

string allMaps[20];
int numberAllMaps = 0;

int statistics[3];

int mapType = -1;
bool isGenerateMap = false;

string newMapName = "";

void ReadMonitorResolution()
{
	ALLEGRO_DISPLAY_MODE   displayData;

	al_get_display_mode(al_get_num_display_modes() - 1, &displayData);

	monitorWidth = displayData.width;
	monitorHeight = displayData.height;
}

void InsertStatistics(int index, int value)
{
	statistics[index] = value;
}

int GetStatistisc(int index)
{
	return statistics[index];
}

void SetMapType(int type)
{
	mapType = type;
}

int GetMapType()
{
	return mapType;
}

int CheckRecords()
{
	if (GetMapType() != -1)
	{
		if (gameTime < statistics[GetMapType()])
		{
			statistics[GetMapType()] = gameTime;
			return true;
		}
	}
	return false;
}

int GetGameTIme()
{
	return gameTime;
}

void SetGameTime(int value)
{
	gameTime = value;
}

void AddGameTime()
{
	gameTime++;
}

int GetDisplayWidth()
{
	return displayWidth;
}

int GetDisplayHeight()
{
	return displayHeight;
}

int GetMonitorWidth()
{
	return monitorWidth;
}

int GetMonitorHeight()
{
	return monitorHeight;
}

void SetFiledVertical(int value)
{
	fieldVertical = value;
}

void  SetFieldHorizontal(int value)
{
	fieldHorizontal = value;
}

void SetMineNumber(int value)
{
	AAAmineNumber = value;
	remainingMines = value;
}

void AddMineNumber()
{
	AAAmineNumber++;
}

void SubMineNumber()
{
	AAAmineNumber--;
}

void SetRemainingMines(int value)
{
	remainingMines = value;
}

int GetRemainingMines()
{
	return remainingMines;
}

void AddRemainigMines()
{
	remainingMines++;
}

void SubRemainigMines()
{
	remainingMines--;
}

int GetFieldVertical()
{
	return fieldVertical;
}

int GetFieldHorizontal()
{
	return fieldHorizontal;
}

int GetMineNumber()
{
	return AAAmineNumber;
}

int GetMarginPixels()
{
	return marginPixels;
}

int GetFieldPixels()
{
	return fieldPixels;
}

int GetBreakPixels()
{
	return breakPixels;
}

int GetInfoPixels()
{
	return infoPixels;
}

void CalulateDisplaySize()
{
	displayWidth = (2 * GetMarginPixels()) + (GetFieldHorizontal() * GetFieldPixels()) + ((GetFieldHorizontal() - 1) * GetBreakPixels());
	displayHeight = (2 * GetMarginPixels()) + (GetFieldVertical() * GetFieldPixels()) + ((GetFieldVertical() - 1) * GetBreakPixels()) + GetInfoPixels();
}