// Calling main libraries
#include <iostream>
#include <Windows.h>

// Caliing namespace
using namespace std;

// Defining the screen size
int nScreenWidth = 120;
int nScreenHeight = 40;

// Defining the player position and angle
float fPlayerX = 0.0f;
float fPlayerY = 0.0f;
float fPlayerA = 0.0F;

int main() {
	
	// Create screen buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytestWritten = 0;

	// Game loop
	while (1) {
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytestWritten);
	}

	return 0;
}