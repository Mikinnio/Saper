#include <string>
#include "Files.h"
#include "Settings.h"
#include "Square.h"

using namespace std;

enum
{
	FIRST_LINE = 0,
	SECOND_LINE = 1,
	THIRD_LINE = 2
};

void OpenFileRead(string fileName);
void OpenFileWrite(string fileName);
void OpenFileAppend(string fileName);
void CloseFile();

string extention = ".data";
string folder = "Maps/";

FILE *file;

void ReadFile(string map)
{
	OpenFileRead(map);
	char line[3];
	int count = 0;
	int a = 0;
	int b = 0;
	int i = 0;
	int j = 0;

	int vertical = 0;
	int horizontal = 0;
	int time = 0;

	while (!feof(file))
	{
		fgets(line, 3, file);

		if ((int)line[0] != 10)
		{
			a = (int)line[0] - 48;
			b = (int)line[1] - 48;

			switch (count)
			{
			case FIRST_LINE:

				if (b >= 0)
				{
					vertical = a * 10 + b;
				}
				else
				{
					vertical = a;
				}

				SetFiledVertical(vertical);
				break;
			case SECOND_LINE:

				if (b >= 0)
				{
					horizontal = a * 10 + b;
				}
				else
				{
					horizontal = a;
				}

				SetFieldHorizontal(horizontal);
				CreateSquareFields();
				break;
			case THIRD_LINE:

				if (b >= 0)
				{
					time = a * 10 + b;
				}
				else
				{
					time = a;
				}

				SetGameTime(time);
				break;
			default:

				if (i < GetFieldVertical())
				{
					if (j < GetFieldHorizontal())
					{
						int value;

						if (b >= 0)
						{
							value = a * 10 + b;
						}
						else
						{
							value = a;
						}

						SetFieldFromFile(i, j, value);
						j++;
					}

					if (j == GetFieldHorizontal())
					{
						j = 0;
						i++;
					}
				}
				break;
			}

			count++;
		}
	}

	CloseFile();
}

void WriteToFile(string fileName)
{
	OpenFileWrite(fileName);

	fprintf(file, "%d\n", GetFieldVertical());
	fprintf(file, "%d\n", GetFieldHorizontal());
	fprintf(file, "%d\n", GetGameTIme());

	for (int i = 0; i < GetFieldVertical(); i++)
	{
		for (int j = 0; j < GetFieldHorizontal(); j++)
		{
			fprintf(file, "%d\n", ReturnFieldValue(i, j));
		}
	}

	CloseFile();
}

void ReadStatistic()
{
	file = fopen("statistics.data", "r");

	char line[4];
	int a, b, c;
	int i = 0;
	int value = 0;


	while (!feof(file))
	{
		fgets(line, 4, file);

		if ((int)line[0] != 10)
		{
			a = (int)line[0] - 48;
			b = (int)line[1] - 48;
			c = (int)line[2] - 48;


			if (c >= 0)
			{
				value = a * 100 + b * 10 + c;
			}
			else if (b >= 0)
			{
				value = a * 10 + b;
			}
			else
			{
				value = a;
			}

			InsertStatistics(i, value);
			i++;

		}
	}

	CloseFile();
}

void WriteStatistic()
{
	file = fopen("statistics.data", "w");

	for (int i = 0; i < 3; i++)
	{
		fprintf(file, "%d\n", GetStatistisc(i));
	}

	CloseFile();
}

void OpenFileRead(string fileName)
{
	string sPath = folder + fileName + extention;
	const char *cPath = sPath.c_str();

	file = fopen(cPath, "r");
}

void OpenFileWrite(string fileName)
{
	string sPath = folder + fileName + extention;
	const char *cPath = sPath.c_str();

	file = fopen(cPath, "w");
}

void OpenFileAppend(string fileName)
{
	string sPath = folder + fileName + extention;
	const char *cPath = sPath.c_str();

	file = fopen(cPath, "a");
}

void CloseFile()
{
	fclose(file);
}
