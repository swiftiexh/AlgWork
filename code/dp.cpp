#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
const int INF = 0x3f3f3f3f;

int main() {
    //׼������  
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
    // Ԥ����ÿ����Ʒ�ڲ�ͬ�̵�����µ���ͼ۸�
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
    // ��ʼ��DP����
    vector<vector<int>> dp(k + 1, vector<int>(1 << m, INF));
    dp[0][0] = 0;  // ��ʼ״̬��0����Ʒ��0���̵�
    // ��̬�滮 - ����Ʒ˳�����״̬ת��
    for (int i = 0; i < n; i++) {
        vector<vector<int>> new_dp(k + 1, vector<int>(1 << m, INF));
        for (int j = 0; j <= k; j++) {
            for (int mask = 0; mask < (1 << m); mask++) {
                if (dp[j][mask] == INF) continue;
                // ���1: �������̵�����й���ǰ��Ʒ
                if (minPrice[i][mask] != INF) {
                    int new_cost = dp[j][mask] + minPrice[i][mask];
                    if (new_cost < new_dp[j][mask]) {
                        new_dp[j][mask] = new_cost;
                    }
                }
                // ���2: �����̵깺��ǰ��Ʒ
                if (j < k) {
                    for (int shop = 0; shop < m; shop++) {
                        if (mask & (1 << shop)) continue;  // ������ѡ�̵�

                        int new_mask = mask | (1 << shop);
                        int new_cost = dp[j][mask] + delivery[shop] + prices[i][shop];

                        if (new_cost < new_dp[j + 1][new_mask]) {
                            new_dp[j + 1][new_mask] = new_cost;
                        }
                    }
                }
            }
        }
        // �������飺����״̬����һ��Ʒ
        dp = move(new_dp);
    }
    // Ѱ�����Ž�
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
    cout << "��̬�滮�㷨ʵ�֣�" << endl;
    // ������
    if (min_cost == INF) {
         cout << -1 << endl;
    }
    else {
        // ������Ʒ���䷽��
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
        // ��ȡѡ�е��̵�
        vector<int> selected_shops;
        for (int j = 0; j < m; j++) {
            if (best_mask & (1 << j)) {
                selected_shops.push_back(j);
            }
        }
        sort(selected_shops.begin(), selected_shops.end());
        // ������
        cout << "��С�ܻ���: " << min_cost << endl;
        cout << "ѡ����̵� (" << selected_shops.size() << "��): ";
        for (int shop : selected_shops) cout << shop << " ";
        cout << "\n��Ʒ���䷽��:" << endl;
        for (int i = 0; i < n; i++) {
            cout << "  ��Ʒ" << i << " �� �̵�" << assignment[i] << endl;
        }
    }
    return 0;
}