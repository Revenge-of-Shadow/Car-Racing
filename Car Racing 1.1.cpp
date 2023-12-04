
#pragma comment(lib, "winmm.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <conio.h>

#define FIELD_WIDTH		20
#define FIELD_HEIGHT	20

#define KEY_ARROW_LEFT	75
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_UP	72
#define KEY_ARROW_DOWN	80
#define KEY_ENTER		13

#define WHEEL_SYMBOL	'O'
#define CABIN_SYMBOL	'0'

struct rivalCar {
	short X = 0, Y = 0;
	short speed = 1;
	short carColor = 1;
};


void showCursor(bool flag)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	// info.dwSize = 20;
	info.bVisible = flag;
	SetConsoleCursorInfo(hOut, &info);
}



void setCursorPosition(COORD coord)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	// COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}
void game_over(int game_time);
void draw_a_car(COORD coordinates, short consoleColor = 1, short carColor = rand() % 16, short displacement_x = 1, short displacement_y = 1);
rivalCar* spawn_a_car(rivalCar* rivalCarArr, short& arr_length);
rivalCar* kill_a_car(rivalCar* rivalCarArr, short& arr_length, short index);
bool car_collision_check(COORD car1_coord, COORD car2_coord);
bool car_arr_collision_check(rivalCar* rca, short arrlen, short& first, short& second);
void erase_a_car(COORD coordinates, short consoleColor, short carColor, short displacement_x, short displacement_y);



int main() {
	//launch musical  track;
	unsigned int track_length = 225000;// Length of the track in seconds.
	PlaySound(TEXT("track.wav"), NULL, SND_ASYNC | SND_LOOP);
	while (true) {
		// Basic initialization
		system("mode con cols=22 lines=25");
		showCursor(false);
		srand(time(NULL));
		short consoleColor = 13, playerColor = 2;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleColor);
		// Field and other inititalization
		bool bl = true;
		bool pause = false;
		unsigned long game_time = 0;
		int speed = 200;
		int road_marker_displacement = 0; // For road center marker to be redrawn properly.
		COORD player_coords;
		FIELD_WIDTH % 2 ? player_coords = { FIELD_WIDTH / 2, FIELD_HEIGHT - 3 } : player_coords = { FIELD_WIDTH / 2 - 1, FIELD_HEIGHT - 3 };// Centering the coordinates
		short rCarr_length = 0;
		rivalCar* rCarArr = new rivalCar[rCarr_length];
		// Drawing field

		for (int i = 0; i < FIELD_WIDTH + 2; i++) {
			std::cout << '-';
		}
		std::cout << std::endl;
		for (int i = 0; i < FIELD_HEIGHT; i++) {
			std::cout << '|';
			for (int j = 0; j < (FIELD_WIDTH); j++) {
				std::cout << ' ';
			}
			std::cout << '|';
			std::cout << std::endl;
		}for (int i = 0; i < (FIELD_WIDTH)+2; i++) {
			std::cout << '-';
		}
		// Drawing road marks
		for (int i = 0; i < FIELD_HEIGHT; i++) {
			if (i % 3 - road_marker_displacement == 0)
				if (FIELD_WIDTH % 2) { // If field has uneven amount of cells in width
					setCursorPosition({ (short)(FIELD_WIDTH / 2 + 1 + 1),(short)(i + 1) });
					std::cout << '|';
				}
				else {
					setCursorPosition({ (short)(FIELD_WIDTH / 2),(short)(i + 1) });
					std::cout << '|' << '|';
				}
		}

		// Printing score
		setCursorPosition({ 0 , FIELD_HEIGHT + 3 });
		std::cout << " Time: ";
		bool play = true;
		while (play) {
			if (!pause) {
				// Run all rival car checks.
				bl = true;
				while (bl) {
					bl = false;
					if (rCarr_length) {
						// Erase all the cars to redraw them on different places later.
						for (short i = 0; i < rCarr_length; i++) {
							erase_a_car({ rCarArr[i].X, rCarArr[i].Y }, consoleColor, rCarArr[i].carColor, 1, 1);
						}
						// Check for car being out because of any reason
						for (short i = 0; i < rCarr_length; i++) {
							if (rCarArr[i].Y >= FIELD_HEIGHT) {
								rCarArr = kill_a_car(rCarArr, rCarr_length, i);
								bl = true;
							}
						}
						// Check for car colissions between rival and rival
						short first = -1, second = -1;
						if (rCarr_length && car_arr_collision_check(rCarArr, rCarr_length, first, second)) {
							rCarArr = kill_a_car(rCarArr, rCarr_length, first);
							rCarArr = kill_a_car(rCarArr, rCarr_length, second);
							bl = true;
						}
						// Check for car colissions between rival and player
						if (rCarr_length)
							for (short i = 0; i < rCarr_length; i++) {
								if (car_collision_check(player_coords, { rCarArr[i].X, rCarArr[i].Y })) {
									game_over(game_time / 1000);
									play = false;
								}
							}
					}
				}


				// Clear road marks
				for (int i = 0; i < FIELD_HEIGHT; i++) {
					if (i % 3 - road_marker_displacement == 0) {
						if (FIELD_WIDTH % 2) { // If field has uneven amount of cells in width
							setCursorPosition({ (short)(FIELD_WIDTH / 2 + 1 + 1),(short)(i + 1) });
							std::cout << ' ';
						}
						else {
							setCursorPosition({ (short)(FIELD_WIDTH / 2),(short)(i + 1) });
							std::cout << ' ' << ' ';
						}
					}
				}

				road_marker_displacement == 2 ? road_marker_displacement = 0 : road_marker_displacement++;

				// Redraw marks
				for (int i = 0; i < FIELD_HEIGHT; i++) {
					if (i % 3 - road_marker_displacement == 0) {
						if (FIELD_WIDTH % 2) { // If field has uneven amount of cells in width
							setCursorPosition({ (short)(FIELD_WIDTH / 2 + 1 + 1),(short)(i + 1) });
							std::cout << '|';
						}
						else {
							setCursorPosition({ (short)(FIELD_WIDTH / 2),(short)(i + 1) });
							std::cout << '|' << '|';
						}
					}
				}

				// Erase player's car
				erase_a_car(player_coords, consoleColor, playerColor, 1, 1);

				// Movement
				if (_kbhit()) {
					char ch = _getch();
					switch (ch) {
					case KEY_ARROW_LEFT:
						if (player_coords.X - 1 > 0) {
							player_coords.X -= 2;
						}
						else if (player_coords.X > 0) {
							player_coords.X -= 1;
						}
						break;
					case KEY_ARROW_RIGHT:
						if (player_coords.X + 2 < FIELD_WIDTH - 2) {
							player_coords.X += 2;
						}
						else if (player_coords.X + 1 < FIELD_WIDTH - 2) {
							player_coords.X += 1;
						}
						break;
					case KEY_ARROW_UP:
						if (player_coords.Y > 0) {
							player_coords.Y--;
						}
						break;
					case KEY_ARROW_DOWN:
						if (player_coords.Y + 2 < FIELD_HEIGHT - 1) {
							player_coords.Y++;
						}
						break;
					case KEY_ENTER:
						pause = true;
						break;
					}
				}

				// Spawn rival car, maybe
				if (rand() % 20 == 19) {
					rCarArr = spawn_a_car(rCarArr, rCarr_length);
				}
				if (rCarr_length) {
					for (short i = 0; i < rCarr_length; i++) {
						rCarArr[i].Y += rCarArr[i].speed;
						draw_a_car({ rCarArr[i].X, rCarArr[i].Y }, consoleColor, rCarArr[i].carColor, 1, 1);
					}
				}

				// Draw player's car
				draw_a_car(player_coords, consoleColor, playerColor, 1, 1);

				Sleep(speed);
				game_time += speed;

				game_time % 30000 <= speed && speed > 25 ? speed -= 25 : speed;
				//game_time % track_length <= speed
				setCursorPosition({ 7 , FIELD_HEIGHT + 3 });
				std::cout << game_time / 1000 << 's';
			}
			else {
				setCursorPosition({ FIELD_WIDTH / 2 - 3 , FIELD_HEIGHT + 2 });
				std::cout << "Paused.";
				Sleep(1000);
				while (true) {
					if (_kbhit() && _getch() == KEY_ENTER) {
						pause = false;
						break;
					}
				}
				setCursorPosition({ FIELD_WIDTH / 2 - 3 , FIELD_HEIGHT + 2 });
				std::cout << "       ";
			}
		}
		system("cls");
		std::cout << "Want to play again?\npress Enter to retry, anything else to close the game.";
		while (true) {
			if (_kbhit())
				if (_getch() == KEY_ENTER)
					break;
				else {
					return 0;
				}
		}
		system("cls");
	}
}

void draw_a_car(COORD coordinates, short consoleColor, short carColor, short displacement_x, short displacement_y) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), carColor);
	if (coordinates.Y >= 0 && coordinates.Y <= FIELD_HEIGHT - 1) {
		setCursorPosition({ (short)(coordinates.X + displacement_x),(short)(coordinates.Y + displacement_y) });
		std::cout << (char)WHEEL_SYMBOL << '-' << (char)WHEEL_SYMBOL;
	}
	if (coordinates.Y >= -1 && coordinates.Y <= FIELD_HEIGHT - 2) {
		setCursorPosition({ (short)(coordinates.X + displacement_x + 1),(short)(coordinates.Y + displacement_y + 1) });
		std::cout << (char)CABIN_SYMBOL;
	}
	if (coordinates.Y >= -2 && coordinates.Y <= FIELD_HEIGHT - 3) {
		setCursorPosition({ (short)(coordinates.X + displacement_x),(short)(coordinates.Y + displacement_y + 2) });
		std::cout << (char)WHEEL_SYMBOL << '-' << (char)WHEEL_SYMBOL;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleColor);

}void erase_a_car(COORD coordinates, short consoleColor, short carColor, short displacement_x, short displacement_y) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), carColor);
	if (coordinates.Y >= 0 && coordinates.Y <= FIELD_HEIGHT - 1) {
		setCursorPosition({ (short)(coordinates.X + displacement_x),(short)(coordinates.Y + displacement_y) });
		std::cout << ' ' << ' ' << ' ';
	}
	if (coordinates.Y >= -1 && coordinates.Y <= FIELD_HEIGHT - 2) {
		setCursorPosition({ (short)(coordinates.X + displacement_x + 1),(short)(coordinates.Y + displacement_y + 1) });
		std::cout << ' ';
	}
	if (coordinates.Y >= -2 && coordinates.Y <= FIELD_HEIGHT - 3) {
		setCursorPosition({ (short)(coordinates.X + displacement_x),(short)(coordinates.Y + displacement_y + 2) });
		std::cout << ' ' << ' ' << ' ';
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), consoleColor);

}
rivalCar* spawn_a_car(rivalCar* rivalCarArr, short& arr_length) {
	arr_length++;
	rivalCar* temp = new rivalCar[arr_length];
	if (arr_length - 1) {
		for (short i = 0; i < arr_length - 1; i++) {
			temp[i] = rivalCarArr[i];
		}
	}
	short color = rand() % 14 + 1;
	temp[arr_length - 1] = { (short)(rand() % (FIELD_WIDTH - 3)), -3, (short)(rand() % 2 + 1), color };
	delete rivalCarArr;
	return temp;
}
rivalCar* kill_a_car(rivalCar* rivalCarArr, short& arr_length, short index) {
	arr_length--;
	rivalCar* temp = new rivalCar[arr_length];
	if (arr_length) {
		for (short i = 0; i < arr_length; i++) {
			if (i < index)
				temp[i] = rivalCarArr[i];
			else if (i >= index)
				temp[i] = rivalCarArr[i + 1];
		}
	}
	delete[] rivalCarArr;
	return temp;
}
bool car_collision_check(COORD car1_coord, COORD car2_coord) {
	for (short i = 0; i < 3; i++) {
		for (short j = 0; j < 3; j++) {
			for (short k = 0; k < 3; k++) {
				for (short l = 0; l < 3; l++) {
					if (car1_coord.X + i == car2_coord.X + j && car1_coord.Y + k == car2_coord.Y + l) {
						return true;
					}
				}
			}
		}
	}
	return false;
}bool car_arr_collision_check(rivalCar* rca, short arrlen, short& first, short& second) {
	for (short i = 0; i < arrlen; i++) {
		for (short j = 0; j < arrlen; j++) {
			if (i != j && car_collision_check({ rca[i].X, rca[i].Y }, { rca[j].X, rca[j].Y })) {
				return true;
			}
		}
	}
	return false;
}
void game_over(int game_time) {
	Sleep(100);
	setCursorPosition({ FIELD_WIDTH / 2 - 1, FIELD_HEIGHT / 2 - 1 });
	std::cout << "Game";
	setCursorPosition({ FIELD_WIDTH / 2, FIELD_HEIGHT / 2 });
	std::cout << "is";
	setCursorPosition({ FIELD_WIDTH / 2 - 1, FIELD_HEIGHT / 2 + 1 });
	std::cout << "over";

	setCursorPosition({ 0, FIELD_HEIGHT + 4 });
	std::cout << "Enter your name. It is needed to save high scores.\n";
	char* name = new char[100];
	std::cin >> name;
	std::fstream fs;
	fs.open("Car racing scores.txt", std::ios_base::app);
	if (fs.is_open()) {
		for (short i = 0; name[i] != '\0'; i++) {
			fs.put(name[i]);
		}
		fs.put(' ');
		fs.put(' ');
		fs.put('T');
		fs.put('i');
		fs.put('m');
		fs.put('e');
		fs.put(':');
		fs.put(' ');
		fs << game_time;
		fs.put('s');
		fs.put('\n');
		fs.close();
	}

}