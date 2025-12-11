#include <iostream>
#include <string>
using namespace std;

struct Celula {
    int dado;
    Celula* prox;
};

class PilhaEncadeada {
public:
    PilhaEncadeada();
    ~PilhaEncadeada();

    void empilhar(int x);

    int desempilhar();
    bool vazia() const;

private:
    Celula* topo;
};

PilhaEncadeada::PilhaEncadeada() : topo(nullptr) {}
PilhaEncadeada::~PilhaEncadeada() {
    while (!vazia()) {
        desempilhar();
    }
}

bool PilhaEncadeada::vazia() const {
    return topo == nullptr;
}

int PilhaEncadeada::desempilhar() {
    if (vazia()) {
        throw runtime_error("Pilha vazia");
    }
    Celula* rem = topo;
    int val = rem->dado;
    topo = topo->prox;
    delete rem;
    return val;
}

void PilhaEncadeada::empilhar(int x) {
    Celula* novo = new Celula;
    novo->dado = x;
    if (this->topo )
}


bool estaBalanceada(const string& seq) {
    PilhaEncadeada pilha;
    return true; 
}


void inverterPilha(PilhaEncadeada& pilha) {
    
}

int main() {
    PilhaEncadeada p;
    p.empilhar(10);
    p.empilhar(20);
    cout << "Desempilhado: " << p.desempilhar() << endl; // espera 20

    string seq1 = "([]){}";
    string seq2 = "([)]";
    cout << seq1 << " balanceada? " << (estaBalanceada(seq1) ? "Sim" : "Nao") << endl;
    cout << seq2 << " balanceada? " << (estaBalanceada(seq2) ? "Sim" : "Nao") << endl;

    p.empilhar(1);
    p.empilhar(2);
    p.empilhar(3);
    inverterPilha(p);
    cout << "Topo apos inversao: " << p.desempilhar() << " (esperado 1)" << endl;

    return 0;
}
