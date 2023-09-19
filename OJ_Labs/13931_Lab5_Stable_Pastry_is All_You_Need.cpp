#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;
struct Pastry {
    string type;
    vector< int> prices;
    //int volatility;
    int sumVolatility;
};
/*
bool compareVolatility(const Pastry& left, const Pastry& right) {
    return left.sumVolatility < right.sumVolatility;
}
*/
int main() {
    int n, m;
    //pastry,month
    cin >> n >> m;

    vector<Pastry> pastries(m);
    //Get names for Pastry
    for ( int i = 0; i < m; i++) {
        cin >> pastries[i].type;
    }

    // Read price data for each month
    for ( int i = 0; i < n; i++) {
        for ( int j = 0; j < m; j++) {
            int price;
            cin >> price;
            pastries[j].prices.push_back(price);
        }
    }
    
    // Calculate volatilities for each pastry type
    // Vmi = delta(p) = |pmi - pmi-1|
    for (int i = 0; i < m; i++) {
        for (int j = 1; j < n; j++) {
            int priceDiff = pastries[i].prices[j] - pastries[i].prices[j - 1];
            int volatility = std::abs(priceDiff);
            pastries[i].sumVolatility += volatility;
        }
    }
    // sort a vector, use stable_sort if order has to be persisted 
    //(in ascending order) a= left, b = right
    std::sort(pastries.begin(), pastries.end(), [](const Pastry& a, const Pastry& b) {
        /*if (a.sumVolatility == b.sumVolatility) {
            // Tie-breaker: Use the order from the original input
            return a.type < b.type;
        }*/
        return a.sumVolatility < b.sumVolatility;
    });
    // vector is now sorted in ascending order
    // Output most stably-priced type of pastry.
        /*
    for (int i =0;i<m;i++){
        cout<<pastries[i].name<<endl;
    }
    */
    cout << pastries[0].type <<endl;

    return 0;
}
