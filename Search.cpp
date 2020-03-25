#include "Search.h"
#include <iomanip>
using namespace std;

Search::Search() {
    _FixedSearchTime = 16463370;
    _isOverTime = false;
    solved = false;
    TT_size = 11000 * 1000;
    TT=vector<HashItem>(TT_size);
    node_count = 0;
    total_h = 0;
    h_time = 0;
    minCut = 16463370;
    maxCut = 0;
    timeCut = 0;
    sumCut = 0;
    initialize_search();
}

//---------------------------Fucntions-------------------------------------
void Search::initialize_search() {
    // things need to initialized before every search
    node_count = 0;
    h_time = 0;
    total_h = 0;
    _isOverTime = false;
    solved = false;
    // this is trying to avoid memory goes over 200MB;
    for (int i = 0; i < TT_size; ++i) {TT[i] = { 0,0 };}
    minCut = 16463370;
    maxCut = 0;
    timeCut = 0;
    sumCut = 0;
}
//print solutions and other information
void Search::print_end() {
    // use history in State to generate path
    int step = game.getStep();
    int length = 0;    //move length
    vector<string> path(step+1,"xx9");
    char Z[] = "xx9";
    vector<unsigned int> Index;
	game.print();
	cout << "solution " << game.getID() << " " << fixed << setprecision(3) << time.count() << " " << step + 1 << endl;
    cout << "[Searched Nodes: " << node_count
        << " Avg_h: " << (total_h / h_time)
        <<" min/avrage/max cut:"<<minCut<<"/"<<double(sumCut)/double(timeCut)<<"/"<<maxCut
        << "]" << endl;
}

void Search::importGame(State &g) {game = g; }

bool Search::isOverTime() {
    // check whether over time
    // for the current code efficiency, check time for every node does not increase time cost much
    // so here check for every node to eliminate searching on time
    if (_isOverTime) { return true; }
    std::chrono::duration<double> d = (std::chrono::system_clock::now() - _search_start_time);
    if (d.count() >= _FixedSearchTime) { _isOverTime = true; return true; }
    return false; 
}

void Search::setTime(const double &time) { _FixedSearchTime = time; }

//-----------------------------Hash-----------------------------------------
//Store TT
void Search::StoreEntry(unsigned int &bound) {
    unsigned long long int _hash = game.getHash();
    int index = _hash % TT_size;
    if (TT[index].hash == _hash) {
        TT[index].bound = bound;
        return;
    }
    // round robin window size =1
    
    else if (index < TT_size - 1) {
        if (TT[index + 1].hash == _hash || TT[index + 1].hash == 0){
            TT[index + 1].hash = _hash;
            TT[index + 1].bound = bound;
            return;
        }
    }
    TT[index].hash = _hash;
    TT[index].bound = bound;
}

//Check TT
bool Search::checkTT(unsigned int &move,unsigned int &tt_bound) {
    unsigned long long int _hash = 0;
    game.hashMove(move, _hash);
    int index = _hash % TT_size;
    if (TT[index].hash == _hash) {
        tt_bound = TT[index].bound;
        return true;
    }
    // round robin window size =1
    
    else if (index < TT_size-1) {
        if (TT[index+1].hash == _hash) {
            tt_bound = TT[index+1].bound;
            return true;
        }
    }
    
    game.Move(move);
    tt_bound = game.getCost();
    h_time++;
    total_h += tt_bound;
    game.Undo();
    return false;
}
//---------------------------Search Engine----------------------------------
void Search::goTT() {
    _search_start_time = std::chrono::system_clock::now();
    initialize_search();
    unsigned int bound = game.getCost();

    while (!solved && bound < 16463370 && !_isOverTime) {
        bound = IDATT(0,bound);
    }

    time = (std::chrono::system_clock::now() - _search_start_time);

    if (solved) {
        print_end();
    }

    else { 
        if (!_isOverTime) { cout << "[Not Solvable!]" << endl; }
        else { print_end(); }
         }
}

unsigned int Search::IDATT(unsigned int depth, unsigned int bound) {
    node_count++;
    if (isOverTime()) {return 16463370;}
    if (game.isGoal()) { solved = true; return 0; }

    //----------Start-----------------
    vector <unsigned int> child = game.genChildren();

    for (unsigned int i = 0; i < child.size(); ++i) {
        //check whether there is a goal-move(8,8,8,8) in children
        if (child[i] == game.end) {
            solved = true;
            return 0;
        }
    }
    
    //Check TT and sort
    unsigned int tt_bound = 1646337;
    for (unsigned int i = 0; i < child.size(); ++i) {
        checkTT(child[i], tt_bound);
        child[i] = child[i] | ((1 + tt_bound) << 16);
    }
    sort(child.begin(), child.end());

    //searching
    unsigned int new_bound = 16463370;
    unsigned int old_bound = 16463370;
    unsigned int t = 16463370;
    for (unsigned int i = 0; i < child.size(); ++i) {
        bool moved = false;
        old_bound = child[i] >> 16;
        if (old_bound <= bound) {
            game.Move(child[i]&0xffff);
            moved = true;
            t = 1 + IDATT(depth + 1, bound - 1);
        }
        else {
            t = old_bound;
            //update cut info
            if (depth < minCut) { minCut = depth; }
            if (depth > maxCut) { maxCut = depth; }
            sumCut += depth;
            timeCut++;
        }
        if (solved) {
            return t;
        }
        if (moved) { game.Undo(); }
        if (t < new_bound) {
            new_bound = t;
        }
    }
    StoreEntry(new_bound);
    return new_bound;
}