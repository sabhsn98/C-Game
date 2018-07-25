#include <iostream>
#include <sstream>
#include <vector>
#include <conio.h>
#include <ctime>
#include <cassert>

using namespace std;
const int SIZEY(9);
const int SIZEX(19);
const int NUMAPPLES(4);
const char MOUSE('@');
const char CAT('C');
const char TUNNEL(' ');
const char WALL('#');
const char APPLE('O');
const char NUT('*'); // C3. Nut character
const int UP(72);
const int DOWN(80);
const int RIGHT(77);
const int LEFT(75);

struct Item {
	int x, y;
	char symbol;
};

int main() {
	void setUpGrid(char[][SIZEX + 1]);
	// C5. Change the overNut parameter to the reference to the nuts vector 
	void paintGrid(const char[][SIZEX + 1], int, vector<Item>& n, bool);
	int getKeyPress();
	// C5. Modify moveMouse function to pass the reference to the nuts vector instead of the 
	// counter of mouses walks over a nut 
	void moveMouse(const char[][SIZEX + 1], Item& m, int, int&, vector<Item>& n);

	// C1. initializeMouse declaration
	void initialiseMouse(Item& m);
	// C2. placeItem declaration
	void placeItem(char[][SIZEX + 1], const Item&);
	// C3. initializeNuts
	void initializeNuts(vector<Item>& nuts);
	// C6. moveCat function declaration
	void moveCat(const char[][SIZEX + 1], Item& c, Item& m);

	char grid[SIZEY + 1][SIZEX + 1];
	setUpGrid(grid);

	Item a0 = { 5, 3, APPLE };
	Item a1 = { 15, 3, APPLE };
	Item a2 = { 5, 7, APPLE };
	Item a3 = { 15, 7, APPLE };

	vector<Item> apples;
	apples.push_back(a0);
	apples.push_back(a1);
	apples.push_back(a2);
	apples.push_back(a3);

	// C3. Store the information related to each nut in a structure variable and create a vector, 
	// called nuts, to hold the information related to the 3 nuts
	Item n0, n1, n2;
	vector<Item> nuts;
	nuts.push_back(n0);
	nuts.push_back(n1);
	nuts.push_back(n2);
	// Call function to set initial position of the nuts
	initializeNuts(nuts);
	// Place the symbol representing each nut in the array  
	for (size_t nno(0); nno < nuts.size(); ++nno)
		placeItem(grid, nuts[nno]);

	Item mouse;
	// C1. The part of the code that initialises the position of the mouse on the grid 
	//Item mouse = { (SIZEX / 2) + 1, (SIZEY / 2) + 1, MOUSE };
	// Call the function
	initialiseMouse(mouse);
	bool catched = false;

	for (int ano(0); ano < NUMAPPLES; ++ano)
		//grid[apples[ano].y][apples[ano].x] = apples[ano].symbol;
		placeItem(grid, apples[ano]); // C2

									  // C2. The part of the code that positions the mouse on the grid  
									  //grid[mouse.y][mouse.x] = mouse.symbol;
									  // Call the function instead
	placeItem(grid, mouse); // C2

							// C6. Select random cat position that doesnt overlap with anything 
	int catX, catY;
	srand(unsigned(time(0)));
	do {
		catX = rand() % SIZEX;
		catY = rand() % SIZEY;
	} while (grid[catY][catX] != TUNNEL);
	Item cat = { catX, catY, CAT };
	// Place cat character on the grid
	placeItem(grid, cat);

	int bumps(0);

	paintGrid(grid, bumps, nuts, catched);

	int key(getKeyPress());
	while (key != 'Q' && nuts.size() && !catched)
	{
		setUpGrid(grid);

		for (int ano(0); ano < NUMAPPLES; ++ano)
			//grid[apples[ano].y][apples[ano].x] = APPLE;
			placeItem(grid, apples[ano]); // C2.
										  // C3.
		for (size_t nno(0); nno < nuts.size(); ++nno)
			placeItem(grid, nuts[nno]);

		//grid[mouse.y][mouse.x] = MOUSE; 
		moveMouse(grid, mouse, key, bumps, nuts);
		placeItem(grid, mouse); // C2.

								// C6.
		moveCat(grid, cat, mouse);
		// Check if the cat has catched the mouse
		if (grid[cat.y][cat.x] == MOUSE) {
			catched = true;
		}
		placeItem(grid, cat);

		paintGrid(grid, bumps, nuts, catched);
		key = getKeyPress();
	}
	system("pause"); //hold console window open
	return 0;
}

// C1. Function that initialises the position of the mouse on the grid
void initialiseMouse(Item& mouse) {
	mouse = { (SIZEX / 2) + 1, (SIZEY / 2) + 1, MOUSE };
}

// C2. Function that positions the item on the grid
void placeItem(char grid[][SIZEX + 1], const Item& item) {
	grid[item.y][item.x] = item.symbol;
}

// C3. Function that sets the initial positions of the nuts on the grid
void initializeNuts(vector<Item>& nuts) {
	nuts[0] = { 4, 6, NUT };
	nuts[1] = { 5, 6, NUT };
	nuts[2] = { 7, 7, NUT };
}

// C6 Function that changes cat position (chases mouse) each time mouse move 
void moveCat(const char grid[][SIZEX + 1], Item& cat, Item& mouse) {
	int dx = 0;
	int dy = 0;
	// Determine cat direction
	if (mouse.x > cat.x)
		dx = 1;
	else if (mouse.x < cat.x)
		dx = -1;
	if (mouse.y > cat.y)
		dy = 1;
	else if (mouse.y < cat.y)
		dy = -1;

	switch (grid[cat.y + dy][cat.x + dx])
	{
		// Set new position
	case TUNNEL: case MOUSE: case NUT:
		cat.y += dy;
		cat.x += dx;
		break;
		// Bypass apple
	case APPLE:
		// Do random change of the previously selected optimal direction
		if (dy == 0) {
			dy = rand() % 2 ? -1 : 1;
		}
		else if (dx == 0) {
			dx = rand() % 2 ? -1 : 1;
		}
		else {
			if (rand() % 2) dx = 0;
			else dy = 0;
		}
		// Make sure that new position is free to go 
		if (grid[cat.y + dy][cat.x + dx] != WALL && grid[cat.y + dy][cat.x + dx] != APPLE) {
			cat.y += dy;
			cat.x += dx;
		}
		break;
	}
}

void setUpGrid(char grid[][SIZEX + 1]) {
	for (int row(1); row <= SIZEY; ++row)
		for (int col(1); col <= SIZEX; ++col)
			if ((row == 1) || (row == SIZEY) ||
				(col == 1) || (col == SIZEX))
				grid[row][col] = WALL;
			else
				grid[row][col] = TUNNEL;
}

void moveMouse(const char grid[][SIZEX + 1], Item& mouse, int key, int& bumps, vector<Item>& nuts) {
	void setKeyDirection(int, int&, int&);
	int dx, dy;
	setKeyDirection(key, dx, dy);
	switch (grid[mouse.y + dy][mouse.x + dx])
	{
	case TUNNEL:
		mouse.y += dy;
		mouse.x += dx;
		break; //B
	case WALL:
		++bumps;
		//C
		break;
	case NUT:
		mouse.y += dy;
		mouse.x += dx;
		// C5. Determine which nut mouse has passed over
		int nutIndex = 0;
		for (size_t nno(0); nno < nuts.size(); ++nno) {
			if (nuts[nno].x == mouse.x && nuts[nno].y == mouse.y)
				nutIndex = nno;
		}
		// Remove that nut from the vector 
		nuts.erase(nuts.begin() + nutIndex);

		break;
	}
}

void setKeyDirection(int key, int& dx, int& dy) {
	dx = 0;
	dy = 0;
	switch (key)
	{
	case LEFT:
		dx = -1;
		dy = 0;
		break;
	case RIGHT:
		dx = +1;
		dy = 0;
		break;
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = +1;
		break;
	}
}

void showMessage(const string& message) {
	cout << message;
}

void paintGrid(const char grid[][SIZEX + 1], int bumps, vector<Item>& nuts, bool catched) {
	void showMessage(const string& message);
	system("cls"); //clear screen
	for (int row(1); row <= SIZEY; ++row)
	{
		for (int col(1); col <= SIZEX; ++col)
			cout << grid[row][col];
		cout << endl;
	}
	ostringstream os;
	os << "\n\nNUMBER OF BUMPS: " << bumps << endl;
	// C5. Show a message 
	os << "\nNUMBER OF NUTS REMAINING: " << nuts.size() << endl;

	// C6. Check winning game condition 
	if (nuts.size() == 0 && !catched) {
		os << "\nTHE MOUSE HAS EATEN ALL OF THE NUTS: YOU WIN!" << endl;
	}
	if (catched) {
		os << "\nTHE CAT HAS CAUGHT THE MOUSE: YOU LOSE!" << endl;
	}
	showMessage(os.str());
}

int getKeyPress() {
	int keyPressed;
	keyPressed = _getch(); //read in selected arrow key or command letter
	while (keyPressed == 224) //ignore symbol following cursor key
		keyPressed = _getch();
	return toupper(keyPressed); //return it in uppercase
}
