#include <string>

using namespace std;

void NewGame(int x, int y, int mine);
void NewGameFromMaps(string map);
bool GameStatus();
void DestroyComponent();
void CreateEventQueue();
void Wait(float seconds);
void GenerateField();
void CreateTimer();
void StartTimer();
void Events(int gameType);
void FlipDisplay();