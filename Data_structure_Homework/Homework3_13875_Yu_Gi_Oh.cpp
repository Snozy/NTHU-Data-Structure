#include <iostream>
#include <string>
#include <sstream>
#include <regex>
using namespace std;
struct TreeNode{
  int data;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int data) : data(data), left(nullptr), right(nullptr) {}
};

class Ricky{
  public:
    Ricky() : root(new TreeNode(-1)), left(nullptr), right(nullptr), wall_damage(0) {}
    void summon(int monster){
        //tree is half full, just add under 5 star monster
        if(root->left == nullptr && monster < 5){
            root->left = new TreeNode(monster);
            //root->left->data = monster;
            //cout<<root->left->data<<endl;
        }else if(root->right == nullptr && monster < 5){
            root->right = new TreeNode(monster);
        }else if(root->right != nullptr && root->left == nullptr && monster >4 && monster < 7){
            //Implement half full tree with 1 sac 
            // left is nullptr
            //cout<<"In here2"<<endl;
            TreeNode * new_node = new TreeNode(monster);
            new_node->right = root->right;
            new_node->left = nullptr;
            root->right = new_node;
        }else if(root->left != nullptr && root->right == nullptr && monster >4 && monster < 7){
            //right is nullptr
            //cout<<"In here"<<endl;
            TreeNode * new_node = new TreeNode(monster);
            new_node->left = root->left;
            new_node->right = nullptr;
            root->left = new_node;
        }else{//tree is full or monster require sac, update
            sacrifice(monster);
        }
    }
    
    void sacrifice(int monster){
        //sac 1, full tree     , half full tree
        if(monster > 4 && monster < 7){ //5,6
            //left == right
            if(root->left->data == root->right->data){
                TreeNode* new_node = new TreeNode(monster);
                new_node->left = root->left;
                new_node->right = nullptr;
                root->left = new_node;
            }
            else if(root->right->data > root->left->data){
                //right > left, monster > left
                //make monster root of right  left = 4, right = 3
                TreeNode* new_node = new TreeNode(monster);
                new_node->left = root->left;
                new_node->right = nullptr;
                root->left = new_node;
            }else if(root->left->data > root->right->data){
                //left > right, monster > right , replace right with current monster
                TreeNode* new_node = new TreeNode(monster);
                new_node->right = root->right;
                new_node->left = nullptr;
                root->right = new_node;
            }           
        } 
        //sac 2, just if full and so make it root 
        else if(monster > 6 && monster <9){ //7,8
            if(root->right != nullptr && root->left != nullptr){
                TreeNode * new_node = new TreeNode(monster);
                new_node->left = root->left;
                new_node->right = root->right;
                root->left = new_node;
                root->right = nullptr;
                //cout<< "LEFT: " << root->left->data << "RIGHT: " << root->right << "Root:" << root->data<<endl;
            }else{
                return;
            }
        }else{ 
            //no sac, when tree is full and adding 5 star monster
            if(root->left->data == root->right->data && root->left->data < monster){
                //make monster root of left
                TreeNode* new_node = new TreeNode(monster);
                new_node->left = root->left;
                new_node->right = nullptr;
                root->left = new_node;
                //root = new_node;
            }
            else if(root->right->data > root->left->data && monster >root->left->data){
                //make monster root of right
                TreeNode* new_node = new TreeNode(monster);
                new_node->left = root->left;
                new_node->right = nullptr;
                root->left = new_node;

            }else if(root->left->data > root->right->data && monster > root->right->data){
                TreeNode* new_node = new TreeNode(monster);
                new_node->right = root->right;
                new_node->left = nullptr;
                root->right = new_node;
            }
        }
    }
    void attac(){
        int damage = 0;
        if(root->right == nullptr){
            damage += root->left->data;
        }else if(root->left == nullptr){
            damage += root->right->data;
        }else{
            damage += root->right->data + root->left->data;
        }
        wall_damage += damage;
    }

    string preorder_traversal(TreeNode* node,int size) {
        if (node == nullptr) {
            return "";
        }
        else if(size == 0){
            return to_string(node->data) + "*" + preorder_traversal(node->left,size) + preorder_traversal(node->right,size);
        }
        size--;
        return to_string(node->data) + "* " + preorder_traversal(node->left,size ) + preorder_traversal(node->right,size);
    }
    
    void print_results() {
        int l_size = findSize(root->left) - 1;
        int r_size = findSize(root->right) - 1;
        // r == 0, have 1 val
        //cout<< "Root left size: "<<l_size<<endl;
        //cout<< "Root right size: "<<r_size<<endl;
        
        if(r_size == -1){//r have no leaf where left*\n
            //cout<<"In here"<<endl;
            cout << preorder_traversal(root->left,l_size)<<preorder_traversal(root->right,r_size)<<endl;
        }else{//r have leaf where right*\n
            l_size += 1;
            cout << preorder_traversal(root->left,l_size)<<preorder_traversal(root->right,r_size)<<endl;
        }
        
        cout << wall_damage << endl;
    }
    int findSize(TreeNode* root) {
    if (root == NULL)
      return 0;
    else
      return(findSize(root->left) + 1 + findSize(root->right));
    }
  private:
    TreeNode* root;
    TreeNode* left;
    TreeNode* right;
    int wall_damage;
    int size;
};

int main() {
    string input;
    Ricky ricky;
    TreeNode test(-1);
    regex rating_regex("\\d+");

    while (getline(cin, input)) {
        string str2 = input.substr(0, 1);
        //cout << str2 << endl;
        if (str2 == "S") {
            // extract the rating using a regular expression
            smatch match;
            if (regex_search(input, match, rating_regex)) {
                int monster = stoi(match.str());
                ricky.summon(monster);
            }
        } else if (str2 == "A") {
            //cout << "Attacking..." << endl;
            ricky.attac();
        } else if (str2 == "T") {
            //cout << "Exiting..." << endl;
            ricky.print_results();
            break;
        }
    }

    return 0;
}
/*
Works if it is not text read
Summon -> 4* 
Attack 
Summon -> 8* 
Attack
Attack
TheEnd

Summon -> 7*
Summon -> 6*
*/
/* no output
Summon -> 4*
Summon -> 3*
Summon -> 5* 
Attack 
Summon -> 8*
Summon -> 4* 
Attack
Attack
Summon -> 7*
Summon -> 6*
TheEnd
*/
