#include <iostream>
#include <string>
#include <vector>
#include <functional>
using namespace std;

struct Fruit
{
   string name;
   vector<int> hyperFeatures;
};

struct GradBinHyperWeight
{
   int featureIdx;
   double gradSlider;
   int binThreshold;
   GradBinHyperWeight *backwardContinuation = nullptr;
   GradBinHyperWeight *forwardContinuation = nullptr;

   bool shouldBackward(Fruit &f) const
   {
      return f.hyperFeatures[featureIdx] < binThreshold;
   }

   void applyGradSlider(Fruit &f) const
   {
      f.hyperFeatures[featureIdx] = static_cast<int>(static_cast<double>(f.hyperFeatures[featureIdx]) * gradSlider);
   }
};

struct Node
{
   GradBinHyperWeight weight;
   Node *left = nullptr;
   Node *right = nullptr;
};

Node *createTree(const vector<GradBinHyperWeight> &hyperWeights)
{
   vector<Node *> nodes(hyperWeights.size());

   // Create the nodes
   for (int i = 0; i < hyperWeights.size(); ++i)
   {
      nodes[i] = new Node();
      nodes[i]->weight = hyperWeights[i];
   }

   // Set the child relationships
   for (int i = 0; i < hyperWeights.size(); ++i)
   {
      int leftChildIndex = i * 2 + 1;
      int rightChildIndex = i * 2 + 2;
      if (leftChildIndex < hyperWeights.size())
      {
         nodes[i]->left = nodes[leftChildIndex];
      }
      if (rightChildIndex < hyperWeights.size())
      {
         nodes[i]->right = nodes[rightChildIndex];
      }
   }

   // Return the root node
   return nodes[0];
}

void applyHyperWeight(Node *node, vector<Fruit> &fruits)
{
   if (node == nullptr)
   {
      return;
   }
   const GradBinHyperWeight &weight = node->weight;

   // Apply the gradSlider to the fruits
   for (Fruit &fruit : fruits)
   {
      weight.applyGradSlider(fruit);
   }

   // Check if the fruits should go to the left or right child
   vector<Fruit> leftFruits;
   vector<Fruit> rightFruits;
   // works except for case 8,10
   for (int i = 0; i < fruits.size(); ++i)
   {
      Fruit &fruit = fruits[i];
      if (weight.shouldBackward(fruit))
      { // left
         leftFruits.push_back(fruit);
      }
      else
      {
         rightFruits.push_back(fruit); // Apply the GradBinHyperWeight to the fruits in the right subarray
      }
   }

   // Recursively apply hyper-weights to the left and right subtrees
   applyHyperWeight(node->left, leftFruits);
   applyHyperWeight(node->right, rightFruits);

   if (node->left == nullptr && leftFruits.size() == 1)
      cout << leftFruits[0].name <<" "<<leftFruits[0].hyperFeatures[node->weight.featureIdx] << endl;
   if (node->right == nullptr && rightFruits.size() == 1)
      cout << rightFruits[0].name << " "<<rightFruits[0].hyperFeatures[node->weight.featureIdx] << endl;

   // Restore the subarrays
   // fruits.clear();
   // fruits.insert(fruits.end(), leftFruits.begin(), leftFruits.end());
   // fruits.insert(fruits.end(), rightFruits.begin(), rightFruits.end());
   /*
   if (rightFruits.size() == 1) {
       cout << "Last featureIdx: " << rightFruits[0].hyperFeatures[rightFruits[0].hyperFeatures.size() - 1] << endl;
   }
   */
}

void printTree(Node *node, int level = 0)
{
   if (node == nullptr)
   {
      return;
   }

   for (int i = 0; i < level; ++i)
   {
      cout << "  ";
   }
   cout << "(" << node->weight.featureIdx << "," << node->weight.gradSlider << "," << node->weight.binThreshold << ")" << endl;

   printTree(node->left, level + 1);
   printTree(node->right, level + 1);
}

int findSize(Node *root)
{
   if (root == NULL)
      return 0;
   else
      return (findSize(root->left) + 1 + findSize(root->right));
}

void printFruits(const vector<Fruit> &fruits, int featureIdx)
{
   for (const Fruit &fruit : fruits)
   {
      cout << fruit.name << " " << fruit.hyperFeatures[featureIdx] << endl;
   }
   /*
   for (const Fruit& fruit : fruits) {
       for (int value : fruit.hyperFeatures) {
           cout << value << " ";
       }
       cout << endl;
   }
   */
}

void postOrderTraversal(Node *node, vector<Fruit> &fruits, int last_featureIdx)
{
   if (node == nullptr)
   {
      return;
   }
   int l_size = findSize(node->left);
   int r_size = findSize(node->right);
   // cout<<l_size<<"  "<<r_size<<endl;
   if (node->left == nullptr)
   {
      if (node->right == nullptr)
      {
         printFruits(fruits, last_featureIdx);
      }
      else
      {
         postOrderTraversal(node->right, fruits, last_featureIdx);
      }
      // printFruits(fruits);
   }
   postOrderTraversal(node->left, fruits, last_featureIdx);
   // postOrderTraversal(node->right, fruits);
}

int main()
{
   int n, m;
   cin >> n >> m;

   vector<Fruit> fruits(n);
   for (int i = 0; i < n; ++i)
   {
      cin >> fruits[i].name;
      fruits[i].hyperFeatures.resize(m);
      for (int j = 0; j < m; ++j)
      {
         cin >> fruits[i].hyperFeatures[j];
      }
   }

   int o;
   cin >> o;
   vector<GradBinHyperWeight> hyperWeights(o);
   for (int i = 0; i < o; ++i)
   {
      cin >> hyperWeights[i].featureIdx;
      cin >> hyperWeights[i].gradSlider;
      cin >> hyperWeights[i].binThreshold;
   }

   // cout<<hyperWeights[o-1].featureIdx<<endl;
   //  Create the tree
   Node *root = createTree(hyperWeights);
   // Apply hyper-weights to fruits
   applyHyperWeight(root, fruits);
   // Print the tree structure
   // cout << "Tree structure:" << endl;
   // printTree(root);

   // Perform post-order traversal to print the final result
   // postOrderTraversal(root, fruits, hyperWeights[o - 1].featureIdx);

   return 0;
}