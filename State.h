#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<random> 
#include<algorithm>
class State
{
public:
  static const int SIZE = 7;
  static const int MAX16 = 65535;
  static std::vector<std::vector<int>> board;     // game board, vector of int
  static std::vector<unsigned int> history;       // history
  static const unsigned int end = 34952;          // 1000 1000 1000 1000 => 8 8 8 8
  static const int X = 21;                        // the int of X
 
private:
  static int nrow, ncol;
  static int current_step;
  static int current_color;
  static std::vector <unsigned int> moveInt;        //the int of move
  static std::unordered_map<int, bool> singleWay;   //whether is single way
  static std::unordered_map<char, int> toInt;       //char to int
  static std::unordered_map<int, char> toChar;      //int to Char
  static std::unordered_map<int, int> path;        //the shape of each animal
  static std::unordered_map<int, unsigned int> location;    //location of each chess
  static std::vector<std::vector<std::vector<unsigned long long int>>> ZobristTable;    // hash value table
  static bool goal;    //whether is goal detected
  static unsigned long long int current_hash;            // current state hash
  static std::string ID;    //game ID
  static std::vector<int> toMove, toReach;
  static int empty;
  static std::vector<int> color_list;
public:  
  State();
  //get game ID
  std::string getID()const;

  //get current h
  unsigned int getCost()const;

  //get move index, when reverse is true, reverse target and source location of the move, for Undo
  void getMoveIndex(const unsigned int mint)const;

  //compress move into unsigned int
  unsigned int getMoveBit(unsigned int sRow, unsigned int sCol, unsigned int tRow, unsigned int tCol )const;

  //check whether a location is on board
  bool isOnBoard(int row,int col)const;

  //initialize a game
  void initialize()const;

  //setup the board
  void setup(const std::string &s, int num_row, int num_col);

  //print baord
  void print()const;
  
  //get the number of avaliable child
  int getNumOfChild(int _row, int _col)const;

  //Used to make move
  bool Move(const unsigned int moveint);    

  //undo
  void Undo();

  //generate children
  std::vector <unsigned int>genChildren()const;

  //return current step
  int getStep()const;

  //return goal
  bool isGoal()const;

  //get chess of board from a given location
  int getBoard(int row,int col)const;

  //return current move int
  std::vector<unsigned int> getIndex()const;

  //get char representation of a given int
  char getChar(int x)const;

  //get the location of a given char
  std::vector<int>getLocation(const char &item)const;

  //set game ID
  void setID(const std::string &name)const;

  //only move hash, helps to check TT
  bool hashMove(const unsigned int moveint, unsigned long long int &moved_hash)const;

  //---------------------HASH-------------------------------
  void initialTable()const;
  unsigned long long int calHash()const;        // calculating hash
  unsigned long long int randomInt()const;      // generate random 64 bits
  unsigned long long int getHash()const;        // get current hash
};
