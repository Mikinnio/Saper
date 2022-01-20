#include <string>

using namespace std;

enum
{
	LEFT_MOUSE_BUTTON = 1,
	RIGHT_MOUSE_BUTTON = 2
};

enum
{
	RANDOM_GAME = 1,
	LOADED_GAME = 2
};

enum
{
	BUTTON_1 = 49,
	BUTTON_2 = 50,
	BUTTON_3 = 51,
	BUTTON_4 = 52,
	BUTTON_ESC = 27
};

int GetMarginPixels();
int GetFieldPixels();
int GetBreakPixels();
int GetInfoPixels();

void SetFiledVertical(int value);
int GetFieldVertical();
void SetFieldHorizontal(int value);
int GetFieldHorizontal();
void SetMineNumber(int value);
int GetMineNumber();
void SetGameTime(int value);
int GetGameTIme();
void AddGameTime();

void SetRemainingMines(int value);
int GetRemainingMines();
void AddRemainigMines();
void SubRemainigMines();

void ReadMonitorResolution();
int GetMonitorWidth();
int GetMonitorHeight();

void CalulateDisplaySize();
int GetDisplayWidth();
int GetDisplayHeight();

void AddMineNumber();
void SubMineNumber();

void InsertStatistics(int index, int value);
int GetStatistisc(int index);

void SetMapType(int type);
int CheckRecords();