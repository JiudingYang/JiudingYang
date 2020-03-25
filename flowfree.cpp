/*
CMPUT 657
Project Flow Free
Jiuding Yang
*/
#include<iostream>
#include "State.h"
#include "Search.h"
#include <string.h>

using namespace std;

using ClockType = decltype(std::chrono::system_clock::now());

int main(int argc, char *argv[]) {
    std::cout.setf(std::ios_base::unitbuf);
    State game;
    Search search;
    search.importGame(game);

    string order;
    if (argc == 3) {
        if (strcmp(argv[1] , "-t")==0) {
            int time = stoi(argv[2]);
            search.setTime(time);
        }
        else { cout << "[Unknown Input]" << endl;}
    }

    while (getline(cin, order)) {
        
        if (order.empty()) { continue; }
        // input q to quit the program, debug use, run single problems
        if (order.at(0) == 'q' && order.size()==1) {cout << "[Game Quit]" << endl;  break; }
        // read board
        if (order.at(0) == 'P') {// P Jumbo 14x14 30
			string ID;
            if (order.size() > 3) {
                ID = order.substr(2, order.size());
                game.setID(ID);
            }
            else { cout << "[Problem Wrong Name!]" << endl; continue; }
			int nrow = 0; int ncol = 0;
			bool finished = false;
			for (unsigned int i = 0; i < ID.size(); ++i) {
				if (ID.at(i) > '0'&&ID.at(i) <= '9') {
					for (i; ID.at(i) != 'x'; ++i) {
						nrow = nrow * 10 + (ID.at(i) - '0');
					}
					++i;
					for (i; ID.at(i) != '_'; ++i) {
						ncol = ncol * 10 + (ID.at(i) - '0');
						finished = true;
					}
					if (finished) { break; }
				}
			}
            string s; string row;
            s = "";
            bool complete = false;
            for (int i = 0; i < nrow; i++) {
                getline(cin, row);
                if (row.size() < unsigned(ncol)) { cout << "[wrong row size]" << endl; break; }
                else {
                    row = row.substr(0, nrow);
                    s = s + row;
                }
				if (i==nrow-1) { complete = true; break; }
            }
            if (complete) {
                game.setup(s,nrow,ncol);
				game.print();
                search.initialize_search();
                search.goTT();
            }
            continue;
        }
    }
    return 0;
}

