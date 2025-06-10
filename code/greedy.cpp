#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//将不销售的商品价格设为INF，避免干扰最小值计算。
const int INF = 0x3f3f3f3f;
int popcount(unsigned int x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

int main() {
    //准备数据  
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> delivery(m);
    for (int i = 0; i < m; i++) cin >> delivery[i];
    vector<vector<int>> prices(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> prices[i][j];
            if (prices[i][j] == -1) prices[i][j] = INF;
        }
    }
    //动态规划的核心
    int best_cost = INF;
    vector<int> best_assignment(n, -1);
    int best_mask = -1;
    for (int mask = 0; mask < (1 << m); mask++) {
        if (popcount(mask) > k) continue;//共检查2^m个子集，但通过k的约束剪枝。
        //对当前选中的商店（mask中为1的位），累加其配送费。
        int total_delivery = 0;
        for (int j = 0; j < m; j++)
            if (mask & (1 << j)) total_delivery += delivery[j];

        vector<int> assignment(n, -1);
        int total_item = 0;
        bool valid = true;

        for (int i = 0; i < n; i++) {//每个物品独立选择最优商店，保证局部最优性。
            int min_price = INF, best_shop = -1;
            for (int j = 0; j < m; j++) {
                if ((mask & (1 << j)) && prices[i][j] < min_price) {
                    min_price = prices[i][j];
                    best_shop = j;
                }
            }
            if (min_price == INF) {
                valid = false;
                break;
            }
            total_item += min_price;
            assignment[i] = best_shop;
        }

        if (!valid) continue;
        int total_cost = total_delivery + total_item;//更新全局最优解
        if (total_cost < best_cost) {
            best_cost = total_cost;
            best_assignment = assignment;
            best_mask = mask;
        }
    }
    //输出结果
    cout << "贪心算法实现：" << endl;
    if (best_cost == INF) {
        cout << -1 << endl;
    }
    else {
        cout << "最小总花费: " << best_cost << endl;
        vector<int> selected_shops;
        for (int j = 0; j < m; j++)
            if (best_mask & (1 << j)) selected_shops.push_back(j);
        sort(selected_shops.begin(), selected_shops.end());
        cout << "选择的商店 (" << selected_shops.size() << "家): ";
        for (int j : selected_shops) cout << j << " ";
        cout << "\n物品分配方案:\n";
        for (int i = 0; i < n; i++)
            cout << "  物品" << i << " → 商店" << best_assignment[i] << endl;
    }
    return 0;
}