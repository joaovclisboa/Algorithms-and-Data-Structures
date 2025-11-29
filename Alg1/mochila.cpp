#include <bits/stdc++.h>
using namespace std;

int n;
vector<long long> values;
vector<long long> weights;
vector<vector<long long>> memo;

long long maximal(long long a, long long b){
    return a > b ? a : b;
}

long long dp(int i, int weight){

    // Caso base
    if (i >= n){
        return 0;
    }

    // Se já foi calculado, retorna direto
    if (memo[i][weight] != -1){
        return memo[i][weight];
    }

    // Se não cabe, pula o item
    if (weights[i] > weight){
        return memo[i][weight] = dp(i + 1, weight);
    }

    // Calcula e salva na memo
    return memo[i][weight] = maximal(
        dp(i + 1, weight),
        dp(i + 1, weight - weights[i]) + values[i]
    );
}

int main(){
    int capacity;
    cin >> n >> capacity;

    values.resize(n);
    weights.resize(n);

    for (int i = 0; i < n; i++){
        cin >> weights[i] >> values[i];
    }

    // Inicializa memo com -1
    memo.assign(n, vector<long long>(capacity + 1, -1));

    cout << dp(0, capacity) << endl;
}
