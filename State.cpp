#include "State.h"

// target end = 136 = {8,8}, used to represent goal location
// end == 34952 = {8,8,8,8}, used to represent goal location
//Here is a link for all colors
//https://flowfree.fandom.com/wiki/List_of_colors
using namespace std;
State::State() { initialTable(); initialize(); }

//------------Vars----------------------------
vector<vector<int>> State::board;    //game board, vector of chars;
vector<unsigned int> State::history; //game history;
int State::current_step = 0;
int State::current_color = 0;
vector<unsigned int> State::moveInt = { 0,0,0,0 };  //extracted moving information {source_row,source_col,targer_row,target_col}
unsigned long long int State::current_hash = 0;
bool State::goal = false;
string State::ID = "Default";
const int State::X;  // the int of 'x'
int State::nrow;
int State::ncol;
vector<int> State::toMove = { 0,0 };
vector<int> State::toReach = { 0,0 };
int State::empty;
vector<int> State::color_list;
//-------------Maps----------------------------
unordered_map<char, int> State::toInt = {
    //transfer char into int
    { '.',0 },     //empty
	{ 'R',1 },     //red
	{ 'B',2 },     //blue
	{ 'Y',3 },     //yellow
	{ 'G',4 },     //green
	{ 'O',5 },     //orange
	{ 'C',6 },     //cyan
	{ 'M',7 },     //magenta
	{ 'm',8 },     //maroon,brown
	{ 'P',9 },     //purple,
	{ 'A',10 },    //gray
	{ 'W',11 },    //white
	{ 'g',12 },    //bright green, lime
	{ 'T',13 },    //tan, beige
	{ 'b',14 },    //dark blue,Navy
	{ 'c',15 },    //dark cyan.teal
	{ 'p',16 },    //pink
};

unordered_map<int, char> State::toChar = {
    //int to char
    { 0,'.' },
	{ 1,'R'},     //red
	{ 2,'B' },     //blue
	{ 3,'Y' },     //yellow
	{ 4,'G' },     //green
	{ 5,'O' },     //orange
	{ 6,'C' },     //cyan
	{ 7,'M' },     //magenta
	{ 8,'m' },     //maroon,brown
	{ 9,'P' },     //purple,
	{ 10,'A' },    //gray
	{ 11,'W' },    //white
	{ 12,'g' },    //bright green, lime
	{ 13,'T' },    //tan, beige
	{ 14,'b' },    //dark blue,Navy
	{ 15,'c' },    //dark cyan.teal
	{ 16,'p' },    //pink
};

unordered_map<int, unsigned int> State::location;

//------------Vars----------------------------
void State::initialize()const {    // initialize game status;
    current_step = 0;
    goal = false;
    moveInt = { 0,0,0,0 };
    //board = vector<vector<int>>(SIZE, vector<int>(SIZE, 0));
    history = vector<unsigned int>(1000,0);
	nrow=5, ncol=5;
	empty = 0;
	color_list.clear();
	current_color = 0;
}

//get current game step
int State::getStep()const { return current_step; }

//get number of child
int State::getNumOfChild(int _row, int _col)const {
	// 1 = up; 2 = down; 3 = left; 4 = right;
	int numChild = 0;
	if (isOnBoard(_row - 1, _col)) {
		if (board[_row - 1][_col] == 0) {
			numChild = numChild + 1 + (1 << 4);
		}
	}
	if (isOnBoard(_row + 1, _col)) {
		if (board[_row + 1][_col] == 0) {
			numChild = numChild + 1 + (2 << 4);
		}
	}
	if (isOnBoard(_row, _col - 1)) {
		if (board[_row][_col - 1] == 0) {
			numChild = numChild + 1 + (3 << 4);
		}
	}
	if (isOnBoard(_row, _col + 1)) {
		if (board[_row][_col + 1] == 0) {
			numChild = numChild + 1 + (4 << 4);
		}
	}
	return numChild;
}

//get current board hash
unsigned long long int State::getHash()const { return current_hash; }

//whether game end
bool State::isGoal()const { return goal; }

//get animal of given location
int State::getBoard(int row, int col)const { return board[row][col]; }

//get current move index
vector<unsigned int> State::getIndex()const { return moveInt; }

//get char representation given int
char State::getChar(int x)const {
    if (toChar.find(x) != toChar.end()) { return toChar[x];}
    else return '?';
}

//set game ID
void State::setID(const string &name)const {ID = name;}

//get the game ID
string State::getID()const {return ID;}
//-----------FCNS------------------------------
//get move information given compressed unsigned int
void State::getMoveIndex(const unsigned int mint)const {
    moveInt[0] = (mint >> 12) & 0x0f;
    moveInt[1] = (mint >> 8) & 0x0f;
    moveInt[2] = (mint >> 4) & 0x0f;
    moveInt[3] = mint & 0x0f;
    return;
}

//compress move information into an unsgined int
unsigned int State::getMoveBit(unsigned int sRow, unsigned int sCol, unsigned int tRow, unsigned int tCol)const {
    return(sRow << 12 | sCol << 8 | tRow << 4 | tCol);
}

//get location of a certain animal
vector<int> State::getLocation(const char & item)const {
    if (toInt.find(item) != toInt.end()) {
        location[toInt[item]];
        return vector<int> {int((location[toInt[item]] >> 4) & 0x0f), int((location[toInt[item]]) & 0x0f) };
    }
    else return{ 8888,8888 };
}

//print board (debug use)
void State::print()const {//print board
	cout << "---------------------" <<endl;
    for (int i = 0; i < nrow; ++i) {
        for (int j = 0; j < ncol; ++j) {
            cout << toChar[abs(board[i][j])]<<" ";
        }
        cout << endl;
    }
}

//get current h
unsigned int State::getCost()const {
	return abs(toMove[0] - toReach[0]) + abs(toMove[1] - toReach[1]) + (empty );
}

//set up board
void State::setup(const std::string & s_board, int num_row, int num_col) {
    initialize();
	nrow = num_row; ncol = num_col;
	board = vector<vector<int>>(num_row, vector<int>(num_col, 0));
    //initialize location info
    location.clear();
	location = {
		{ 1,MAX16 },{ -1,MAX16 },
		{ 2,MAX16 },{ -2,MAX16 },
		{ 3,MAX16 },{ -3,MAX16 },
		{ 4,MAX16 },{ -4,MAX16 },
		{ 5,MAX16 },{ -5,MAX16 },
		{ 6,MAX16 },{ -6,MAX16 },
		{ 7,MAX16 },{ -7,MAX16 },
		{ 8,MAX16 },{ -8,MAX16 },
		{ 9,MAX16 },{ -9,MAX16 },
		{ 10,MAX16 },{ -10,MAX16 },
		{ 11,MAX16 },{ -11,MAX16 },
		{ 12,MAX16 },{ -12,MAX16 },
		{ 13,MAX16 },{ -13,MAX16 },
		{ 14,MAX16 },{ -14,MAX16 },
		{ 15,MAX16 },{ -15,MAX16 },
		{ 16,MAX16 },{ -16,MAX16 },
	
	};

	//collect board information
    for (int i = 0; i < num_row; ++i) {
        for (int j = 0; j < num_col; ++j) {
            //if the input is undefined, map will assume it is empty
            board[i][j] = toInt[s_board.at(i*num_col + j)];
            //update location of each animal
            if (board[i][j] != 0) {
                if (location[board[i][j]] == MAX16) {
                    location[board[i][j]] = i << 4 | j;
				}
				else if (location[-board[i][j]] == MAX16) {// mean second time show up
					location[-board[i][j]] = i << 4 | j;
					// gives a negative value to distinguish.
					board[i][j] = -board[i][j];
                }
            }
			else {
				++empty;
			}
        }
    }

    //update hash;
    calHash();

	//fill all color that only has one child
	for (auto iter = location.begin(); iter != location.end(); ++iter) {
		if (iter->second != 65535) {
			int _row = iter->second >> 4 & 0x0f;
			int _col = iter->second & 0x0f;
			//if only one child
			if ((getNumOfChild(_row, _col) & 0x0f) == 1) {
				int direc = getNumOfChild(_row, _col) >> 4;
				if (direc == 1) {
					board[_row - 1][_col] = iter->first;
					iter->second = (_row - 1) << 4 | _col;
				}
				else if (direc == 2) {
					board[_row + 1][_col] = iter->first;
					iter->second = (_row + 1) << 4 | _col;
				}
				else if (direc == 3) {
					board[_row][_col - 1] = iter->first;
					iter->second = _row << 4 | (_col - 1);
				}
				else if (direc == 4) {
					board[_row][_col + 1] = iter->first;
					iter->second = _row << 4 | (_col + 1);
				}
			}
		}
	}

	//generate color list, which color to move first, sort by length of initial cost
	vector<unsigned> templist;
	for (auto iter = location.begin(); iter != location.end(); ++iter) {
		if (iter->first >0 && iter->second != 65535) {
			int _sRow = iter->second >> 4 & 0x0f;
			int _sCol = iter->second & 0x0f;
			int _color = iter->first;
			int _tRow = location[-_color] >> 4 & 0x0f;
			int _tCol = location[-_color] & 0x0f;
			unsigned _cost = abs(_sRow - _tRow) + abs(_sCol - _tCol);

			templist.emplace_back((_cost << 8) + _color);
		}
	}
	sort(templist.begin(), templist.end());
	color_list.resize(templist.size());
	for (unsigned int i = 0; i < templist.size(); ++i) {
		color_list[i] = templist[i] & 0xff;
	}

	//update toMove & toReach
	toMove[0] = location[color_list[0]] >> 4 & 0x0f;
	toMove[1] = location[color_list[0]] & 0x0f;
	toReach[0] = location[-color_list[0]] >> 4 & 0x0f;
	toReach[1] = location[-color_list[0]] & 0x0f;
}

//whether is on board(debug use)
bool State::isOnBoard(int row, int col)const {
    if (col < ncol &&  row < nrow &&col >= 0 && row >= 0) { return true; }
    else { return false; }
}

//generate children
vector<unsigned int> State::genChildren()const {
    vector<unsigned int> child_list;
	int _row, _col;
	int _colorToMove;
	//check goal
	if (current_color == color_list.size()-1) {
		if (abs(toMove[0] - toReach[0]) + abs(toMove[1] - toReach[1]) == 1) {
			child_list.emplace_back(end);
			return child_list;
		}
	}
	//change toMove if one color is finished
	if (abs(toMove[0] - toReach[0]) + abs(toMove[1] - toReach[1]) == 1) {
		_colorToMove = color_list[current_color + 1];
		_row = location[_colorToMove] >> 4 & 0x0f;
		_col = location[_colorToMove] & 0x0f;
	}
	else {
		_colorToMove = color_list[current_color];
		_row = toMove[0];
		_col = toMove[1];
	}
	//generate children
	unsigned int tempMove;
	if (isOnBoard(_row - 1, _col)) {//up
		if (board[_row - 1][_col] == 0) {
			tempMove = _row << 12 | _col << 8 | (_row - 1) << 4 | _col;
			child_list.emplace_back(tempMove);
		}
	}
	if (isOnBoard(_row + 1, _col)) {//down
		if (board[_row + 1][_col] == 0) {
			tempMove = _row << 12 | _col << 8 | (_row + 1) << 4 | _col;
			child_list.emplace_back(tempMove);
		}
	}
	if (isOnBoard(_row, _col - 1)) {//left
		if (board[_row][_col - 1] == 0) {
			tempMove = _row << 12 | _col << 8 | _row << 4 | (_col - 1);
			child_list.emplace_back(tempMove);
		}
	}
	if (isOnBoard(_row, _col + 1)) {//right
		if (board[_row][_col + 1] == 0) {
			tempMove = _row << 12 | _col << 8 | _row << 4 | (_col + 1);
			child_list.emplace_back(tempMove);
		}
	}
    return child_list;
}

//-----------Move------------------------------
//move
bool State::Move(const unsigned int moveint) {
	if (goal == true) { return true; }
	if (moveint == end) { goal = true; return true; }
	getMoveIndex(moveint);
	int colorToMove = board[moveInt[0]][moveInt[1]];
	if (colorToMove != color_list[current_color]) {
		++current_color;
		toReach[0] = location[-colorToMove] >> 4 & 0x0f;
		toReach[1] = location[-colorToMove] & 0x0f;
	}
	history[current_step] = moveint;
	++current_step;
	board[moveInt[2]][moveInt[3]] = colorToMove;
	toMove[0] = moveInt[2];
	toMove[1] = moveInt[3];
	current_hash ^= ZobristTable[moveInt[2]][moveInt[3]][colorToMove];
	//print();
    return true;
}

//Undo move
void State::Undo() {
    if (current_step == 0) { cout << "[History Empty]" << endl; return; }
    --current_step;
    getMoveIndex(history[current_step]);
    int colorToMove = board[moveInt[0]][moveInt[1]];
	if (colorToMove != color_list[current_color]) { 
		--current_color;
		toReach[0] = location[-colorToMove] >> 4 & 0x0f;
		toReach[1] = location[-colorToMove] & 0x0f;
	}
	board[moveInt[2]][moveInt[3]] = 0;
	toMove[0] = moveInt[0];
	toMove[1] = moveInt[1];
	current_hash ^= ZobristTable[moveInt[2]][moveInt[3]][colorToMove];
}


//To only move hash
bool State::hashMove(const unsigned int moveint, unsigned long long int &moved_hash)const {
	getMoveIndex(moveint);
	int colorToMove = board[moveInt[0]][moveInt[1]];
	moved_hash = current_hash ^ ZobristTable[moveInt[2]][moveInt[3]][colorToMove];
	return true;
};

//-------Hash Calculation-----------
vector<vector<vector<unsigned long long int>>> State::ZobristTable;
mt19937 mt(1646337);

unsigned long long int State::randomInt()const{
    uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX);
    return dist(mt);
}

//initialize ZobristTable
void State::initialTable()const {
	// board max size 15x15, with 16 kinds of colors.
    ZobristTable = vector<vector<vector<unsigned long long int>>>(15, vector<vector<unsigned long long int>>(15, vector<unsigned long long int>(16, 0)));
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            for (int k = 0; k < 16; k++) {
                ZobristTable[i][j][k] = randomInt();
            }        
}

//calculate current hash
unsigned long long int State::calHash()const {
    current_hash = 0;
    for (auto iter = location.begin(); iter != location.end(); ++iter) {
        if (iter->second != 65535) {
			int _row = iter->second >> 4 & 0x0f;
			int _col = iter->second & 0x0f;
            current_hash ^= ZobristTable[_row][_col][abs(board[_row][_col])+1];
        }
    }
    return current_hash;
}







