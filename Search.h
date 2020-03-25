#pragma once
#include<iostream>
#include<vector>
#include<string> 
#include<chrono>
#include<unordered_map>
#include<algorithm>
#include"State.h"

typedef struct HashItem{ 
    // the entries to store in TT;
    unsigned long long int hash;            // 64 bit hash;
    unsigned int bound;
}HashItem;

class Search {
private:
     double _FixedSearchTime;            // Search time per problem;
     State game;                         // game engine;
     std::vector<HashItem> TT;           // transposition table;
     decltype(std::chrono::system_clock::now()) _search_start_time;    // the time when a new search start;
     bool _isOverTime;                      // whether is over fixed time;
     int h_time;                            // time of h evaluated
     std::chrono::duration<double> time;    // time of search end
     bool solved;    //whether game solved
     int TT_size;    // size of TT
     unsigned int node_count; // total nodes
     double total_h;          // sum of all estimated h 
     unsigned int minCut;     // min depth when cut
     unsigned int maxCut;     // max depth when cut
     unsigned int timeCut;    // times of cut
     unsigned long long int sumCut;     // sum of depth of all cut

public:
    Search();
    //-----------Functions--------------------
    void initialize_search();              // initialize before IDS
    void importGame(State &game);          // Read gameboard to local vars;
    bool isOverTime();                     // check whether searching is over time;
    void print_end();                      // print after search end
    void setTime(const double &time);      // set time limit for each problem

    //-----------Hash Table-------------------
    void StoreEntry(unsigned int &bound);                      // store entries into TT;
    bool checkTT(unsigned int &move, unsigned int &tt_bound);  // check TT

    //-----------Search Engine----------------        
    void goTT();
    unsigned int IDATT(unsigned int g, unsigned int t);
};