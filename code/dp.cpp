#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
const int INF = 0x3f3f3f3f;

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
    // 预处理每个物品在不同商店组合下的最低价格
    vector<vector<int>> minPrice(n, vector<int>(1 << m, INF));
    for (int i = 0; i < n; i++) {
        for (int mask = 0; mask < (1 << m); mask++) {
            int min_val = INF;
            for (int shop = 0; shop < m; shop++) {
                if (mask & (1 << shop)) {
                    if (prices[i][shop] < min_val) {
                        min_val = prices[i][shop];
                    }
                }
            }
            minPrice[i][mask] = min_val;
        }
    }
    // 初始化DP数组
    vector<vector<int>> dp(k + 1, vector<int>(1 << m, INF));
    dp[0][0] = 0;  // 初始状态：0个物品，0个商店
    // 动态规划 - 按物品顺序进行状态转移
    for (int i = 0; i < n; i++) {
        vector<vector<int>> new_dp(k + 1, vector<int>(1 << m, INF));
        for (int j = 0; j <= k; j++) {
            for (int mask = 0; mask < (1 << m); mask++) {
                if (dp[j][mask] == INF) continue;
                // 情况1: 在现有商店组合中购买当前物品
                if (minPrice[i][mask] != INF) {
                    int new_cost = dp[j][mask] + minPrice[i][mask];
                    if (new_cost < new_dp[j][mask]) {
                        new_dp[j][mask] = new_cost;
                    }
                }
                // 情况2: 新增商店购买当前物品
                if (j < k) {
                    for (int shop = 0; shop < m; shop++) {
                        if (mask & (1 << shop)) continue;  // 跳过已选商店

                        int new_mask = mask | (1 << shop);
                        int new_cost = dp[j][mask] + delivery[shop] + prices[i][shop];

                        if (new_cost < new_dp[j + 1][new_mask]) {
                            new_dp[j + 1][new_mask] = new_cost;
                        }
                    }
                }
            }
        }
        // 滚动数组：更新状态到下一物品
        dp = move(new_dp);
    }
    // 寻找最优解
    int min_cost = INF;
    int best_mask = -1;
    int best_j = -1;
    for (int j = 0; j <= k; j++) {
        for (int mask = 0; mask < (1 << m); mask++) {
            if (dp[j][mask] < min_cost) {
                min_cost = dp[j][mask];
                best_mask = mask;
                best_j = j;
            }
        }
    }
    cout << "动态规划算法实现：" << endl;
    // 输出结果
    if (min_cost == INF) {
         cout << -1 << endl;
    }
    else {
        // 计算物品分配方案
        vector<int> assignment(n);
        for (int i = 0; i < n; i++) {
            int best_shop = -1;
            int min_price = INF;
            for (int shop = 0; shop < m; shop++) {
                if ((best_mask & (1 << shop)) && prices[i][shop] < min_price) {
                    min_price = prices[i][shop];
                    best_shop = shop;
                }
            }
            assignment[i] = best_shop;
        }
        // 提取选中的商店
        vector<int> selected_shops;
        for (int j = 0; j < m; j++) {
            if (best_mask & (1 << j)) {
                selected_shops.push_back(j);
            }
        }
        sort(selected_shops.begin(), selected_shops.end());
        // 输出结果
        cout << "最小总花费: " << min_cost << endl;
        cout << "选择的商店 (" << selected_shops.size() << "家): ";
        for (int shop : selected_shops) cout << shop << " ";
        cout << "\n物品分配方案:" << endl;
        for (int i = 0; i < n; i++) {
            cout << "  物品" << i << " → 商店" << assignment[i] << endl;
        }
    }
    return 0;
}