#include <stdio.h>
#include <string>
#include <conio.h>
#include <string>
#include "Engine.h"
#include "Settings.h"
#include "Files.h"

using namespace std;

void NewGameMenu();
void LoadSave();

int XCustomGame();
int YCustomGame();
int MinesCustomGame(int x, int y);

void ShowStatistic();

void ClearScreen();
void ExitGame();

bool consoleStatus = true;
string map = "";

void GenerateMainMenu()

{
	int keyConsole;

	while (consoleStatus)
	{
		printf("(1) Nowa gra\n");
		printf("(2) Wczytaj gre\n");
		printf("(3) Statystyki\n");
		printf("(ESC) Wyjdz z gry\n");

		keyConsole = _getch();

		if (keyConsole == BUTTON_1)
		{
			NewGameMenu();
		}
		else if (keyConsole == BUTTON_2)
		{
			LoadSave();
		}
		else if (keyConsole == BUTTON_3)
		{
			ShowStatistic();
		}
		else if (keyConsole == BUTTON_ESC)
		{
			ExitGame();
		}

		ClearScreen();
	}
}

void NewGameMenu()
{
	int keyNewGame;
	bool newGameStatus = true;
	int newX, newY, newMine;

	ClearScreen();

	while (newGameStatus)
	{
		ReadStatistic();
		printf("(1) Latwa\n");
		printf("(2) Srednia\n");
		printf("(3) Trudna\n");
		printf("(4) Wlasna\n");
		printf("(ESC) Wroc\n");

		keyNewGame = _getch();

		switch (keyNewGame)
		{
		case BUTTON_1:
			newX = newY = 9;
			newMine = 10;
			SetMapType(0);
			break;
		case BUTTON_2:
			newX = newY = 16;
			newMine = 40;
			SetMapType(1);
			break;
		case BUTTON_3:
			newX = 30;
			newY = 16;
			newMine = 99;
			SetMapType(2);
			break;
		case BUTTON_4:
			newX = XCustomGame();
			newY = YCustomGame();
			newMine = MinesCustomGame(newX, newY);
			break;
		case BUTTON_ESC:
			newGameStatus = false;
			break;
		}

		if (keyNewGame >= BUTTON_1 && keyNewGame <= BUTTON_4)
		{
			ClearScreen();

			NewGame(newX, newY, newMine);

			CreateTimer();
			CreateEventQueue();

			while (GameStatus())
			{
				Events(RANDOM_GAME);

				FlipDisplay();
			}

			DestroyComponent();
		}

		ClearScreen();
	}

	ClearScreen();
}

void LoadSave()
{
	NewGameFromMaps("save");

	CreateTimer();
	CreateEventQueue();

	while (GameStatus())
	{
		Events(LOADED_GAME);

		FlipDisplay();
	}

	DestroyComponent();
}

int XCustomGame()
{
	int x = -1;

	while (x < 10 || x > 30)
	{
		ClearScreen();
		printf("Ilosc pol w poziomie\n");
		scanf("%d", &x);
	}

	return x;
}

int YCustomGame()
{
	int y = -1;

	while (y < 10 || y > 30)
	{
		ClearScreen();
		printf("Ilosc pol w pionie\n");
		scanf("%d", &y);
	}

	return y;
}

int MinesCustomGame(int x, int y)
{
	int maxMines = x * y - 9;
	int mines = 0;

	while (mines < 10 || mines > maxMines)
	{
		ClearScreen();
		printf("Ilosc min z zakresu <10, %d>\n", maxMines);
		scanf("%d", &mines);
	}

	return mines;
}

void ShowStatistic()
{
	ClearScreen();
	ReadStatistic();

	int keyStatistic;
	bool statisticStatus = true;

	while (statisticStatus)
	{
		printf("Statystyki\n");
		printf("Latwy: %d\n", GetStatistisc(0));
		printf("Sredni: %d\n", GetStatistisc(1));
		printf("Trudny: %d\n", GetStatistisc(2));
		printf("(ESC) Wroc\n", GetStatistisc(2));

		keyStatistic = _getch();

		if (keyStatistic == BUTTON_ESC)
		{
			statisticStatus = false;
		}
	}
}

void ClearScreen()
{
	system("cls");
}

void ExitGame()
{
	consoleStatus = false;
}