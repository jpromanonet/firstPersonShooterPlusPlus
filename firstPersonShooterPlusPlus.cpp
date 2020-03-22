// Calling main libraries
#include <iostream>
#include <chrono>
#include <Windows.h>

// Caliing namespace
using namespace std;

// Defining the screen size
int nScreenWidth = 120;
int nScreenHeight = 40;

// Defining the player position and angle
float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0F;

// Defining the map
int nMapHeight = 16;
int nMapWidth = 16;

// Distances read
float fFOV = 3.14159 / 4.0;
float fDepth = 16.0f;

int main() {
	
	// Create screen buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytestWritten = 0;

	// Drawing the map
	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	// Time points
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	// Game loop
	while (1) {

		// Using the time points
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();
		
		//Controls & Hanlde CCW Rotation

		// Left side movement is key A
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (0.1f) * fElapsedTime;

		// Right side movement is key D
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (0.1f) * fElapsedTime;

		for (int x = 0; x < nScreenWidth; x++) {
			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) + fFOV;

			float fDistanceToWall = 0;
			bool bHitWall = false;

			// Unit vector for ray in player space
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			// Cutting distance micro step by micro step
			while (!bHitWall && fDistanceToWall < fDepth) {
				fDistanceToWall += 0.1f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				// Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true; // Maximun depth
					fDistanceToWall = fDepth;
				}
				else {
					// Ray is inbounds so test to see if the ray cell is a wall block
					if (map[nTestY * nMapWidth + nTestX] == '#') {
						bHitWall = true;
					}
				}
			}

			// Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			for (int y = 0; y < nScreenHeight; y++) {
				if (y < nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if(y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = '#';
				else
					screen[y * nScreenWidth + x] = ' ';
			}
		}

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytestWritten);
	}

	return 0;
}