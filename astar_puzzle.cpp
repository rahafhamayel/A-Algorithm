#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <cmath>
#include <fstream>
using namespace std;

struct State {
    vector<int> board;
    int g; // cost from start
    int h; // heuristic value
    string moves; 
    int f() const { return g + h; } // f(n) = g + h
};

vector<int> goal = {1, 2, 3, 4, 5, 6, 7, 0};

int heuristic(const vector<int>& b) {
    int cnt = 0;
    for (int i = 0; i < 8; i++) {
        if (b[i] != 0 && b[i] != goal[i])
            cnt++;
    }
    return cnt;
}

void printBoard(const vector<int>& b) {
    for (int i = 0; i < 8; i++) {
        cout << b[i] << " ";
        if (i % 2 == 1) cout << endl;
    }
    cout << endl;
}

bool inBounds(int r, int c) {
    return (r >= 0 && r < 4 && c >= 0 && c < 2);
}

void solveAStar(vector<int> start) {
    map<vector<int>, int> visited; 

    State startState = {start, 0, heuristic(start), ""};
    auto cmp = [](State a, State b) { return a.f() > b.f(); };
    priority_queue<State, vector<State>, decltype(cmp)> pq(cmp);

    pq.push(startState);
    int expanded = 0;

    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();

        expanded++;

        if (cur.board == goal) {
            cout << "✅ Solution Found!\n";
            cout << "Number of expanded nodes: " << expanded << endl;
            cout << "Length of solution: " << cur.moves.size() << endl;
            cout << "Moves:\n" << cur.moves << endl;
            return;
        }

        visited[cur.board] = 1;

        int zeroPos = find(cur.board.begin(), cur.board.end(), 0) - cur.board.begin();
        int zr = zeroPos / 2;
        int zc = zeroPos % 2;

        int dr[4] = {-1, 1, 0, 0};
        int dc[4] = {0, 0, -1, 1};
        char dir[4] = {'U', 'D', 'L', 'R'};

        for (int i = 0; i < 4; i++) {
            int nr = zr + dr[i];
            int nc = zc + dc[i];
            if (inBounds(nr, nc)) {
                vector<int> nb = cur.board;
                swap(nb[zr * 2 + zc], nb[nr * 2 + nc]);
                if (!visited[nb]) {
                    pq.push({nb, cur.g + 1, heuristic(nb), cur.moves + dir[i] + string("\n")});
                }
            }
        }

        int row = zr;
        for (int c = 0; c < 1; c++) {
            int idx1 = row * 2 + c;
            int idx2 = row * 2 + c + 1;
            if (idx1 < 8 && idx2 < 8 && cur.board[idx1] != 0 && cur.board[idx2] != 0) {
                vector<int> nb = cur.board;
                swap(nb[idx1], nb[idx2]);
                if (!visited[nb]) {
                    string move = "E " + to_string(cur.board[idx1]) + " " + to_string(cur.board[idx2]) + "\n";
                    pq.push({nb, cur.g + 2, heuristic(nb), cur.moves + move});
                }
            }
        }
    }

    cout << "❌ No solution found.\n";
}

int main() {
    string filename;
    cout << "Enter file name containing initial and goal states: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!\n";
        return 0;
    }

    vector<int> start(8);
    goal.resize(8);

    for (int i = 0; i < 8; i++) file >> start[i];
    for (int i = 0; i < 8; i++) file >> goal[i];
    file.close();

    cout << "\nInitial State:\n";
    printBoard(start);
    cout << "Goal State:\n";
    printBoard(goal);

    solveAStar(start);

    return 0;
}
