#include <iostream>
#include <sstream>

using namespace std;

const int MAX_SIZE = 100;

class Stack {
private:
    string stack[MAX_SIZE];
    int top;

public:
    Stack() {
        top = -1;
    }

    void push(string value) {
        if (top == MAX_SIZE - 1) {
            // Stack is full
            return;
        }
        top++;
        stack[top] = value;
    }

    string pop() {
        if (top <= -1) {
            // Stack is empty
            return "";
        }
        string popped_value = stack[top];
        top--;
        return popped_value;
    }

    bool isEmpty() {
        return (top == -1);
    }

    int size() {
        return top + 1;
    }
};

class Queue {
private:
    string queue[MAX_SIZE];
    int front_index;
    int rear_index;

public:
    Queue() {
        front_index = 0;
        rear_index = -1;
    }

    void enqueue(string value) {
        if (rear_index == MAX_SIZE - 1) {
            // Queue is full
            return;
        }
        rear_index++;
        queue[rear_index] = value;
    }

    string dequeue() {
        if (front_index > rear_index) {
            // Queue is empty
            return "";
        }
        string dequeued_value = queue[front_index];
        front_index++;
        return dequeued_value;
    }

    bool isEmpty() {
        return (front_index > rear_index);
    }

    int getFrontIndex() {
        return front_index;
    }

    int getRearIndex() {
        return rear_index;
    }
    //check boundry
    string getElementAt(int index) {
        return queue[index];
    }
};

int main() {
    // Initialize variables
    string input;
    Stack stack;
    Queue queue;
    Queue valid_queue;
    string stack_sequence = "";
    int total_value = 0;
    int miss_count = 0;
    int value = 0;
    int new_line = 0;
    // Read inputs
    while (getline(cin, input)) {
        if (input == "TL") {
            new_line += 1;
            stack.push("TR");
        } else if (input == "TR") {
            new_line += 1;
            stack.push("TL");
        } else if (input == "KL" || input == "KR" || input == "JUMP" || input == "FLIP") {
            // Do nothing for these moves
        } else if (input == "MISS") {
            miss_count = 4; //hint
        } else if (input == "GOLD") {
            queue.enqueue("G");
            //cout<<miss_count<<endl;
            if(miss_count == 0){
                valid_queue.enqueue("G");
            }else{
                miss_count--;
            }
        } else if (input == "SILVER") {
            //stack.push("S");
            queue.enqueue("S");
            if(miss_count == 0){
                valid_queue.enqueue("S");
            }else{
                miss_count--;
            }
        }
    }

    bool found_sequence = true;
    while (found_sequence) {
        found_sequence = false;

        for (int i = queue.getFrontIndex(); i <= queue.getRearIndex() - 2; i++) {
            string sequence = queue.getElementAt(i) +
                              queue.getElementAt(i + 1) +
                              queue.getElementAt(i + 2);

            if (sequence == "GGG") {
                total_value += 500;
                for (int j = 0; j < 3; j++) {
                    queue.dequeue();
                }
                found_sequence = true;
                break;
            } else if (sequence == "GGS" || sequence == "SGG" || sequence == "GSG") {
                total_value += 300;
                for (int j = 0; j < 3; j++) {
                    queue.dequeue();
                }
                found_sequence = true;
                break;
            } else if (sequence == "GSS" || sequence == "SSG" || sequence == "SGS") {
                total_value += 150;
                for (int j = 0; j < 3; j++) {
                    queue.dequeue();
                }
                found_sequence = true;
                break;
            } else if (sequence == "SSS") {
                total_value += 50;
                for (int j = 0; j < 3; j++) {
                    queue.dequeue();
                }
                found_sequence = true;
                break;
            } 
        }

        if (!found_sequence) {
            string element = queue.dequeue();
            queue.enqueue(element);
        }
    }
    
    bool found_sequence1 = true;
    while (found_sequence1) {
        found_sequence1 = false;

        for (int i = valid_queue.getFrontIndex(); i <= valid_queue.getRearIndex() - 2; i++) {
            string valid_sequence = valid_queue.getElementAt(i) +
                              valid_queue.getElementAt(i + 1) +
                              valid_queue.getElementAt(i + 2);

            if (valid_sequence == "GGG") {
                value += 500;
                for (int j = 0; j < 3; j++) {
                    valid_queue.dequeue();
                }
                found_sequence1 = true;
                break;
            } else if (valid_sequence == "GGS" || valid_sequence == "SGG" || valid_sequence == "GSG") {
                value += 300;
                for (int j = 0; j < 3; j++) {
                    valid_queue.dequeue();
                }
                found_sequence1 = true;
                break;
            } else if (valid_sequence == "GSS" || valid_sequence == "SSG" || valid_sequence == "SGS") {
                value += 150;
                for (int j = 0; j < 3; j++) {
                    valid_queue.dequeue();
                }
                found_sequence1 = true;
                break;
            } else if (valid_sequence == "SSS") {
                value += 50;
                for (int j = 0; j < 3; j++) {
                    valid_queue.dequeue();
                }
                found_sequence1 = true;
                break;
            }
        }

        if (!found_sequence1) {
            string valid_element = valid_queue.dequeue();
            valid_queue.enqueue(valid_element);
        }
    }
    
    // Print the way back using the stack
    while (!stack.isEmpty()) {
        string move = stack.pop();
        cout << move;
        if (!stack.isEmpty()) {
            cout << "->";
        }
    }
    if(new_line >0){
        cout<<endl;
    }
    cout << value << endl;
    cout << total_value<<endl;

    return 0;
}
