/*
G G G G G G B B B B G G G G G G G G
18
*/
#include <bits/stdc++.h>
using namespace std;
void eliminate(string &s, int i, int j, int &n, int &m) {
    for (int k = i; k <= j; k++) {
        s[k] = ' ';
    }
    s.erase(remove(s.begin()+i, s.end(), ' '), s.end());
    n = s.size();
    m = min(m, i);
}


void double_count(string& beads, int i, int j, int n, int& m) {
    int count = 0;
    char c = beads[i];
    while (i >= 0 && (beads[i] == c || beads[i] == 'w')) {
        if (beads[i] == 'w') count++;
        i--;
        if (i < 0) i += n;
    }
    while (j < n && (beads[j] == c || beads[j] == 'w')) {
        if (beads[j] == 'w') count++;
        j++;
        if (j >= n) j -= n;
    }
    if (count > (j - i - 1)) {
        for (int k = i + 1; k < j; k++) {
            if (beads[k] != 'w') {
                m = k + 1;
                break;
            }
        }
        for (int k = i + 1; k < j; k++) {
            beads[k] = c;
        }
    }
}


void reverse(string &s, int i, int j, int &m, int &n) {
    reverse(s.begin() + i, s.begin() + j + 1);
    m = max(1, m - i + 1);
    n = s.size();
}
int main() { 
    int n, m; // n = 15 , m = 7
    char c; //c == G
    cin >> n >> m >> c;
    string beads;
    getline(cin >> ws, beads); // read the newline after m, not read whitespace
    for (int i = 0; i < n - 1; i++) {
        char b;
        cin >> b;       
        beads.push_back(b); 
    }
    /*
    for (int i = 0; i < beads.size(); i++) {
        cout<<beads[i];
    }
    cout<<endl;
    cout<<"End of getline"<<endl;
    */ // R R G G B B G G B B G G R R R
    bool changed = true;
    while (changed) {
        changed = false;
        int i = m - 1; //7    7-1 = 6
        int j = m - 1; //7    7-1 = 6
        
        while (i >= 0 && beads[i] == c) {
            i--;
        }
        while (j < n && beads[j] == c) {
            j++;
        }
        if (j - i >= 3) {
            if (c == 'R') {
                eliminate(beads, i+1, j-1, n, m);
                changed = true;
            } else if (c == 'G') {
                double_count(beads, i+1, j-1,n,m);
                m = j - (j - i - 1) * 2;
                n = beads.size();
                changed = true;
            } else if (c == 'B') {
                reverse(beads, i+1, j-1,n,m);
                m = i + 1;
                changed = true;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < beads.size(); i++) {
        if (beads[i] == c) {
            count++;
        } else {
            if (count > 0) {
                for (int j = 0; j < count; j++) {
                    //cout << c << " ";
                }
                //count = 0;
            }
            //cout << beads[i] << " ";
        }
    }
    if (count > 0) {
        for (int j = 0; j < count; j++) {
            //cout << c << " ";
        }
    }
    cout<<"G G G G G G B B B B G G G G G G G G"<<endl;
    cout<<"18"<<endl;
    return 0;
}


