#include <iostream>
#include <vector>
#include <cstdint>

using std::cin;
using std::cout;
using std::endl;

// Retorna true se conjunto representado por maskA (sobre 0..n-1) é
// lexicograficamente menor que maskB
bool lexLessMask(unsigned long long maskA, unsigned long long maskB, int n) {
    if (maskA == maskB) return false;
    for (int i = 0; i < n; ++i) {
        bool a = (maskA & (1ULL << i)) != 0;
        bool b = (maskB & (1ULL << i)) != 0;
        if (a != b) {
            return a; // se A tem o i-ésimo e B não, A é menor
        }
    }
    return false;
}

// Versão para máscaras sobre apenas a segunda metade (0..n2-1)
bool lexLessRight(unsigned int maskA, unsigned int maskB, int n2) {
    if (maskA == maskB) return false;
    for (int i = 0; i < n2; ++i) {
        bool a = (maskA & (1U << i)) != 0;
        bool b = (maskB & (1U << i)) != 0;
        if (a != b) return a;
    }
    return false;
}

int main() {
    int N; // número de duendes (0..N-1)
    int M; // número de pares que brigam
    if (!(cin >> N >> M)) {
        return 0;
    }

    // Grafo como lista de adjacência em formato bitmask (até 64 vértices)
    std::vector<unsigned long long> adj(N, 0ULL);

    for (int k = 0; k < M; ++k) {
        int a, b;
        cin >> a >> b;
        if (a < 0 || b < 0 || a >= N || b >= N || a == b) continue;
        adj[a] |= (1ULL << b);
        adj[b] |= (1ULL << a);
    }

    // Divide em duas metades
    int n1 = N / 2;
    int n2 = N - n1;

    unsigned long long leftMaskGlobal = 0ULL;
    if (n1 > 0) leftMaskGlobal = (1ULL << n1) - 1ULL; // bits 0..n1-1
    unsigned long long allMaskGlobal = 0ULL;
    if (N > 0) allMaskGlobal = (1ULL << N) - 1ULL;     // bits 0..N-1
    unsigned long long rightMaskGlobal = allMaskGlobal & (~leftMaskGlobal);

    // Adjacência apenas dentro da metade esquerda (bits 0..n1-1)
    std::vector<unsigned int> adjLeftLocal(n1, 0U);
    for (int i = 0; i < n1; ++i) {
        unsigned long long m = adj[i] & leftMaskGlobal; // já está em 0..n1-1
        adjLeftLocal[i] = (unsigned int)m;
    }

    // Adjacência apenas dentro da metade direita, reindexando 0..n2-1
    std::vector<unsigned int> adjRightLocal(n2, 0U);
    for (int j = 0; j < n2; ++j) {
        int g = n1 + j; // índice global
        if (g >= N) break;
        unsigned long long m = adj[g] & rightMaskGlobal;   // bits n1..N-1
        m >>= n1; // reindexa para 0..n2-1
        adjRightLocal[j] = (unsigned int)m;
    }

    // Máscara de conflitos da metade esquerda para a metade direita
    // crossMaskR[i] diz com quais vértices da direita o vértice i (esquerda) briga
    std::vector<unsigned int> crossMaskR(n1, 0U);
    for (int i = 0; i < n1; ++i) {
        unsigned long long m = adj[i] & rightMaskGlobal; // conflitos na direita (global)
        m >>= n1;                                        // reindexando para 0..n2-1
        crossMaskR[i] = (unsigned int)m;
    }

    // =========================
    // DP na metade direita
    // =========================
    int sizeR = (n2 == 0) ? 1 : (1 << n2);
    std::vector<int> popR(sizeR, 0);
    std::vector<char> okR(sizeR, 0);
    std::vector<int> valR(sizeR, 0); // tamanho da independ. se mask é independente, senão -inf
    std::vector<int> bestSizeR(sizeR, 0); // melhor tamanho para subconjuntos de mask
    std::vector<unsigned int> repR(sizeR, 0U);      // representante independente em mask
    std::vector<unsigned int> bestRepR(sizeR, 0U);  // melhor representante para subconjuntos de mask

    const int NEG_INF = -1000000000;

    okR[0] = 1;
    valR[0] = 0;
    repR[0] = 0U;
    if (sizeR > 1) {
        for (int mask = 1; mask < sizeR; ++mask) {
            int lsb = __builtin_ctz(mask); // índice do bit menos significativo
            int prev = mask ^ (1 << lsb);
            popR[mask] = popR[prev] + 1;
            // independente se prev é independente e não há aresta entre lsb e prev
            if (okR[prev] && ((adjRightLocal[lsb] & (unsigned int)prev) == 0U)) {
                okR[mask] = 1;
                valR[mask] = popR[mask];
                repR[mask] = (unsigned int)mask;
            } else {
                okR[mask] = 0;
                valR[mask] = NEG_INF;
                repR[mask] = 0U;
            }
        }
    }

    // Inicializa bestSizeR/bestRepR
    for (int mask = 0; mask < sizeR; ++mask) {
        bestSizeR[mask] = valR[mask];
        bestRepR[mask] = repR[mask];
    }

    // DP de superconjuntos: para cada máscara, queremos o melhor subconjunto independente
    // contido nela. Técnica padrão: para cada bit, propaga dos subconjuntos menores.
    for (int bit = 0; bit < n2; ++bit) {
        for (int mask = 0; mask < sizeR; ++mask) {
            if (mask & (1 << bit)) {
                int sub = mask ^ (1 << bit);
                int candSize = bestSizeR[sub];
                if (candSize > bestSizeR[mask]) {
                    bestSizeR[mask] = candSize;
                    bestRepR[mask] = bestRepR[sub];
                } else if (candSize == bestSizeR[mask]) {
                    if (lexLessRight(bestRepR[sub], bestRepR[mask], n2)) {
                        bestRepR[mask] = bestRepR[sub];
                    }
                }
            }
        }
    }

    // =========================
    // DP na metade esquerda (apenas para saber quais máscaras são independentes)
    // =========================
    int sizeL = (n1 == 0) ? 1 : (1 << n1);
    std::vector<int> popL(sizeL, 0);
    std::vector<char> okL(sizeL, 0);

    okL[0] = 1;
    if (sizeL > 1) {
        for (int mask = 1; mask < sizeL; ++mask) {
            int lsb = __builtin_ctz(mask);
            int prev = mask ^ (1 << lsb);
            popL[mask] = popL[prev] + 1;
            if (okL[prev] && ((adjLeftLocal[lsb] & (unsigned int)prev) == 0U)) {
                okL[mask] = 1;
            } else {
                okL[mask] = 0;
            }
        }
    }

    // =========================
    // Combina as duas metades
    // =========================
    unsigned int allRightMask = (n2 == 0) ? 0U : (unsigned int)((1U << n2) - 1U);

    int bestTotal = -1;
    unsigned long long bestFullMask = 0ULL;

    for (int maskL = 0; maskL < sizeL; ++maskL) {
        if (!okL[maskL]) continue; // esquerda não é independente

        unsigned int allowedR = allRightMask;
        // remove da direita todos que brigam com algum da esquerda
        int tmp = maskL;
        while (tmp) {
            int v = __builtin_ctz(tmp);
            tmp ^= (1 << v);
            allowedR &= ~crossMaskR[v];
        }

        int sizeLeft = (maskL == 0) ? 0 : popL[maskL];
        int sizeRight = (n2 == 0) ? 0 : bestSizeR[allowedR];
        if (sizeRight < 0) sizeRight = 0; // pode ser NEG_INF se nada independente
        int total = sizeLeft + sizeRight;

        if (total < bestTotal) continue;

        unsigned int rightSetLocal = (n2 == 0) ? 0U : bestRepR[allowedR];
        unsigned long long fullMask = (unsigned long long)maskL;
        fullMask |= ((unsigned long long)rightSetLocal << n1);

        if (total > bestTotal) {
            bestTotal = total;
            bestFullMask = fullMask;
        } else if (total == bestTotal) {
            if (lexLessMask(fullMask, bestFullMask, N)) {
                bestFullMask = fullMask;
            }
        }
    }

    if (bestTotal < 0) {
        bestTotal = 0;
        bestFullMask = 0ULL;
    }

    // Saída
    cout << bestTotal << '\n';
    if (bestTotal > 0) {
        bool first = true;
        for (int i = 0; i < N; ++i) {
            if (bestFullMask & (1ULL << i)) {
                if (!first) cout << ' ';
                first = false;
                cout << i;
            }
        }
        cout << '\n';
    } else {
        cout << '\n';
    }

    return 0;
}
