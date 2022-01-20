#include <string>

using namespace std;

struct POSITION
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
};

struct SQUARE
{
	POSITION coordinates;
	int nearMine = 0;
	int nearFlag = 0;
	int nearQuestionMark = 0;
	bool isOpen = false;
	bool hasFlag = false;
	bool hasMine = false;
	bool canHasMine = true;
	bool mineExplode = false;
};

enum
{
	KEEP_PLAYING = 1,
	YOU_WIN = 2,
	YOU_LOSE = 3
};

void CreateSquareFields();
void LoadSquareFields(string map);
void SetFieldFromFile(int i, int j, int number);
void DeleteSquareFields();

bool IsOpen(int x, int y);
bool HasFlag(int x, int y);
bool HasMine(int x, int y);
bool MineExplode(int x, int y);
int NearMine(int x, int y);
POSITION FieldPosition(int x, int y);

void GenerateField();
int YCalculate(int xMouse);
int XCalculate(int yMouse);
void OpenField(int x, int y);
void GenerateMine();
void CalculateNearMine();
void AddFlag(int x, int y);
void CalculateNearFlags();
int GetGameState();

void OpenCloseSingleField(int x, int y);
void SetMineFlag(int x, int y);
int ReturnFieldValue(int x, int y);
