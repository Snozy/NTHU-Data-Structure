#include <iostream>
#include <stack>
#include <queue>
#include <string>

using namespace std;

int main() {
    stack<string> leaving_order;
    queue<int> toasting_order[20];
    char event;
    string line;
    //Variables
    int total_glasses = 0;
    int table_nums[6] = {0}; 
    int max = -1;
    int min = 9999;
    int time_delta;
    int table_num_index = -1;
    
    while (getline(cin, line)) {
        event = line[0];
        if (event == 'R') {
            string name;
            int arrival_time;
            cin >> name >> arrival_time;
            max = std::max(max,arrival_time);
            min = std::min(min,arrival_time);
            leaving_order.push(name);
        } else if (event == 'T') {
            int table_num, num_glasses;
            cin >> table_num >> num_glasses;      
            table_nums[++table_num_index] = table_num;
            toasting_order[table_num-1].push(num_glasses);
        }
        cin.ignore(); // ignore newline character after each line
    }
    
    //Leaving Guests
    while (!leaving_order.empty()) {
        cout << leaving_order.top() << " ";
        leaving_order.pop();
    }
    cout<<endl;
    
    //Delta time
    time_delta = max - min;
    cout<<time_delta<<endl;
    //Table numbers
    for(int i = 0; i <= table_num_index; i++) {
        cout << table_nums[i] << " ";
    }
    cout<<endl;
    //Total number of drinks that TA drank
    for (int i = 0; i < 20; i++) {
        while (!toasting_order[i].empty()) {
            total_glasses += toasting_order[i].front();
            toasting_order[i].pop();
        }
    }
    cout << total_glasses<<endl;
    return 0;
}
