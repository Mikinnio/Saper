#include <allegro5\allegro.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Engine.h"
#include "Square.h"
#include "Settings.h"
#include "Files.h"

using namespace std;

struct IMAGES
{
	ALLEGRO_BITMAP *cell;
	ALLEGRO_BITMAP *emptyCell;
	ALLEGRO_BITMAP *explodedMineCell;
	ALLEGRO_BITMAP *flaggedCell;
	ALLEGRO_BITMAP *flaggedWrongCell;
	ALLEGRO_BITMAP *mine;
	ALLEGRO_BITMAP *clock;
};

void CreateDisplay(int width, int height);
void InitializeComponent();
void DrawField();
void ShowResults();

const float FPS = 1.0;
bool draw = true;
int xMouse, yMouse;

int numberOfLeftClicked = 0;
int numberOfRightClicked = 0;

bool gameOn;

IMAGES images;

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *font;
ALLEGRO_FONT *fontTimer;

bool GameStatus()
{
	return gameOn;
}

void NewGame(int x, int y, int mine)
{
	SetFieldHorizontal(x);
	SetFiledVertical(y);
	SetMineNumber(mine);
	SetGameTime(0);

	CreateSquareFields();

	CalulateDisplaySize();

	CreateDisplay(GetDisplayWidth(), GetDisplayHeight());

	GenerateField();
	DrawField();
}

void NewGameFromMaps(string map)
{
	LoadSquareFields(map);

	CalulateDisplaySize();

	CreateDisplay(GetDisplayWidth(), GetDisplayHeight());
	GenerateField();
	DrawField();
}

void CreateDisplay(int width, int height)
{
	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not initialize Alleglro 5", NULL, NULL);
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED);

	display = al_create_display(width, height);

	ReadMonitorResolution();

	al_set_window_position(display, (GetMonitorWidth() / 2) - (width / 2), (GetMonitorHeight() / 2) - (height / 2));

	al_set_window_title(display, "Saper");
	al_clear_to_color(al_map_rgb(220, 255, 255));

	if (!display)
	{
		al_show_native_message_box(NULL, NULL, NULL, "Could not create Alleglro window", NULL, NULL);
	}

	InitializeComponent();
}

void InitializeComponent()
{
	gameOn = true;

	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	font = al_load_font("arial.ttf", 12, NULL);
	fontTimer = al_load_font("arial.ttf", 20, NULL);

	images.cell = al_load_bitmap("Graphics/Cell.png");
	images.emptyCell = al_load_bitmap("Graphics/EmptyCell.png");
	images.explodedMineCell = al_load_bitmap("Graphics/ExplodedMineCell.png");
	images.flaggedCell = al_load_bitmap("Graphics/FlaggedCell.png");
	images.flaggedWrongCell = al_load_bitmap("Graphics/FlaggedWrongCell.png");
	images.mine = al_load_bitmap("Graphics/Mine.png");
	images.clock = al_load_bitmap("Graphics/Clock.png");
}

void Events(int gameType)
{
	ALLEGRO_EVENT events;
	al_wait_for_event(event_queue, &events);

	if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		xMouse = events.mouse.x;
		yMouse = events.mouse.y;
	}

	if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (GetGameState() == KEEP_PLAYING)
		{
			int x = XCalculate(yMouse);
			int y = YCalculate(xMouse);

			if ((x != -1) && (y != -1))
			{
				if (events.mouse.button & LEFT_MOUSE_BUTTON)
				{
					numberOfLeftClicked++;
					if (HasFlag(x, y) == false)
					{
						if (numberOfLeftClicked == 1 && numberOfRightClicked == 0)
						{
							if (gameType == RANDOM_GAME)
							{
								GenerateMine();
								CalculateNearMine();
							}
							StartTimer();
						}

						OpenField(x, y);
					}
				}

				if (events.mouse.button & RIGHT_MOUSE_BUTTON)
				{
					if (numberOfLeftClicked > 0 || gameType == LOADED_GAME)
					{

						if (numberOfRightClicked == 0)
						{
							StartTimer();
						}

						numberOfRightClicked++;

						AddFlag(x, y);
						CalculateNearFlags();
					}
				}

				DrawField();
			}
		}
	}

	if (events.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (events.keyboard.keycode)
		{
		case ALLEGRO_KEY_S:
			if (GetGameState() == KEEP_PLAYING)
			{
				al_show_native_message_box(display, "Uwaga!", "Zapis", "Gra zostala zapisana!", NULL, NULL);
				WriteToFile("save");
			}
			break;
		case ALLEGRO_KEY_ESCAPE:
			if (GetGameState() == KEEP_PLAYING)
			{
				int value = al_show_native_message_box(display, "Uwaga!", "Czy chcesz zapisac stan gry?", "Kliknij Yes jesli tak lub No jesli nie", NULL, ALLEGRO_MESSAGEBOX_YES_NO);

				if (value == 1)
				{
					WriteToFile("save");
				}
			}

			gameOn = false;
			break;
		}
	}

	if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		if (GetGameState() == KEEP_PLAYING)
		{
			int value = al_show_native_message_box(display, "Uwaga!", "Czy chcesz zapisac stan gry?", "Kliknij tak lub nie", NULL, ALLEGRO_MESSAGEBOX_YES_NO);

			if (value == 1)
			{
				WriteToFile("save");
			}
		}

		gameOn = false;
	}

	if (events.type == ALLEGRO_EVENT_TIMER)
	{
		if (GetGameState() == KEEP_PLAYING && numberOfLeftClicked > 0)
		{
			AddGameTime();
			DrawField();
		}
	}
}

void DrawField()
{
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR fieldColor = al_map_rgb(220, 255, 255);

	al_clear_to_color(fieldColor);

	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			POSITION myCoordinates = FieldPosition(i, j);

			int cx = myCoordinates.x2 - (GetFieldPixels() / 2);
			int cy = myCoordinates.y2 - (GetFieldPixels() / 2);
			int r = GetFieldPixels() / 4;

			char strNearMine[10];
			snprintf(strNearMine, 10, "%d", NearMine(i, j));

			char time[10];
			snprintf(time, 10, "%d", GetGameTIme());

			char mines[10];
			snprintf(mines, 10, "%d", GetRemainingMines());

			if (GetGameState() == YOU_LOSE && HasFlag(i, j) == true && HasMine(i, j) == false)
			{
				al_draw_bitmap(images.flaggedWrongCell, myCoordinates.x1, myCoordinates.y1, NULL);
			}
			else if (HasFlag(i, j) == true)
			{
				al_draw_bitmap(images.flaggedCell, myCoordinates.x1, myCoordinates.y1, NULL);
			}
			else if (IsOpen(i, j) == true)
			{
				al_draw_bitmap(images.emptyCell, myCoordinates.x1, myCoordinates.y1, NULL);
			}
			else
			{
				al_draw_bitmap(images.cell, myCoordinates.x1, myCoordinates.y1, NULL);
			}

			if (HasMine(i, j) == true && IsOpen(i, j) == true && MineExplode(i, j) == true)
			{
				al_draw_bitmap(images.explodedMineCell, myCoordinates.x1, myCoordinates.y1, NULL);
			}
			else if (HasMine(i, j) == true && IsOpen(i, j) == true)
			{
				al_draw_bitmap(images.mine, myCoordinates.x1, myCoordinates.y1, NULL);
			}

			if (HasMine(i, j) == false && NearMine(i, j) != 0 && IsOpen(i, j) == true)
			{
				al_draw_text(font, black, cx - 1, cy - 7, ALLEGRO_ALIGN_CENTRE, strNearMine);
			}

			al_draw_filled_rectangle(GetMarginPixels(), 0, GetDisplayWidth() - GetMarginPixels(), GetInfoPixels(), fieldColor);

			al_draw_bitmap(images.clock, (GetDisplayWidth() / 2) - 60, GetInfoPixels() / 4, NULL);
			al_draw_text(fontTimer, black, (GetDisplayWidth() / 2) - 30, GetInfoPixels() / 4, NULL, time);

			al_draw_bitmap(images.mine, (GetDisplayWidth() / 2) + 10, GetInfoPixels() / 4, NULL);
			al_draw_text(fontTimer, black, (GetDisplayWidth() / 2) + 40, GetInfoPixels() / 4, NULL, mines);
		}
	}

	if (GetGameState() == YOU_LOSE || GetGameState() == YOU_WIN)
	{
		ShowResults();
	}
}

void ShowResults()
{
	if (GetGameState() == YOU_WIN)
	{
		al_show_native_message_box(display, "Wygrales", "Wygrales!", "Wygrales!", NULL, NULL);
		if (CheckRecords() == true)
		{
			al_show_native_message_box(display, "Uwaga!", "Rekord", "Ustanowiles nowy rekord!", NULL, NULL);
			WriteStatistic();
		}
	}
	else
	{
		al_show_native_message_box(display, "Przegrales", "Przegrales!", "Przegrales!", NULL, NULL);
	}
}

void CreateEventQueue()
{
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
}

void CreateTimer()
{
	timer = al_create_timer(1.0 / FPS);
}

void StartTimer()
{
	al_start_timer(timer);
}

void DestroyComponent()
{
	DeleteSquareFields();

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_destroy_font(fontTimer);

	al_destroy_bitmap(images.cell);
	al_destroy_bitmap(images.emptyCell);
	al_destroy_bitmap(images.explodedMineCell);
	al_destroy_bitmap(images.flaggedCell);
	al_destroy_bitmap(images.flaggedWrongCell);
	al_destroy_bitmap(images.mine);
	al_destroy_bitmap(images.clock);

	numberOfLeftClicked = 0;
	numberOfRightClicked = 0;
}

void Wait(float seconds)
{
	al_rest(seconds);
}

void FlipDisplay()
{
	al_flip_display();
}