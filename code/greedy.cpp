#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//�������۵���Ʒ�۸���ΪINF�����������Сֵ���㡣
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
    //��̬�滮�ĺ���
    int best_cost = INF;
    vector<int> best_assignment(n, -1);
    int best_mask = -1;
    for (int mask = 0; mask < (1 << m); mask++) {
        if (popcount(mask) > k) continue;//�����2^m���Ӽ�����ͨ��k��Լ����֦��
        //�Ե�ǰѡ�е��̵꣨mask��Ϊ1��λ�����ۼ������ͷѡ�
        int total_delivery = 0;
        for (int j = 0; j < m; j++)
            if (mask & (1 << j)) total_delivery += delivery[j];

        vector<int> assignment(n, -1);
        int total_item = 0;
        bool valid = true;

        for (int i = 0; i < n; i++) {//ÿ����Ʒ����ѡ�������̵꣬��֤�ֲ������ԡ�
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
        int total_cost = total_delivery + total_item;//����ȫ�����Ž�
        if (total_cost < best_cost) {
            best_cost = total_cost;
            best_assignment = assignment;
            best_mask = mask;
        }
    }
    //������
    cout << "̰���㷨ʵ�֣�" << endl;
    if (best_cost == INF) {
        cout << -1 << endl;
    }
    else {
        cout << "��С�ܻ���: " << best_cost << endl;
        vector<int> selected_shops;
        for (int j = 0; j < m; j++)
            if (best_mask & (1 << j)) selected_shops.push_back(j);
        sort(selected_shops.begin(), selected_shops.end());
        cout << "ѡ����̵� (" << selected_shops.size() << "��): ";
        for (int j : selected_shops) cout << j << " ";
        cout << "\n��Ʒ���䷽��:\n";
        for (int i = 0; i < n; i++)
            cout << "  ��Ʒ" << i << " �� �̵�" << best_assignment[i] << endl;
    }
    return 0;
}