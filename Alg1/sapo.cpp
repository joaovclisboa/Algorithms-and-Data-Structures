//Descrição curta: dado N casas com alturas, e K salto máximo, calcule o menor custo para ir da casa 0 até a casa N‑1. O custo de um salto i→j é |h[j] − h[i]|. A implementação usa programação dinâmica recursiva com memoização.

//Entrada: N K seguido de N inteiros (alturas).

//Saída: valor do custo mínimo para alcançar a casa N−1.

//Abordagem: estado dp[i] = custo mínimo para chegar em i; caso base dp[0]=0; transição dp[i] = min_{j em [i-K..i-1]} (dp[j] + |h[i]-h[j]|). Implementado recursivamente com vetor de pares (altura, custo) e memoização.



#include <iostream>
#include <vector>
#include <climits>

typedef long long ll;

ll K;

ll mod(ll num){
    if (num > 0){
        return num;
    }
    return num * -1;
}

ll maximo(ll num1, ll num2){
    return num1 > num2 ? num1 : num2;
}
ll progDin(std::vector<std::pair<ll, ll>> &costs, ll N){
    if (N == 0){
        costs[0].second = 0;
        return 0;
    }
    if (costs[N].second != -1){
        return costs[N].second;
    }
    ll minimo = LLONG_MAX/4;
    for (int i = N-1; i >= maximo(N-K, 0); i--){
        if (costs[i].second == -1){
            progDin(costs, i);
        }
        if ((costs[i].second + mod((costs[N].first) - costs[i].first)) < minimo){
            minimo = costs[i].second + mod((costs[N].first) - costs[i].first);
        }
    }
    costs[N].second = minimo;
    return minimo;
}

int main(void){
    //N eh o numero de "casas", K é o passo máximo
    //logo pras N casas temos que ver os K's passos contrários.
    //nos queremos ver o custo de N
    ll N;
    std::cin >> N >> K;
    std::vector<std::pair<ll, ll>> custos(N, {-1,-1});
    for (int i = 0; i < N; i++){
        std::cin >> custos[i].first;
    }
    progDin(custos, N-1);
    std::cout << custos[N-1].second;
}
