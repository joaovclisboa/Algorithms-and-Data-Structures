#include <iostream>
using namespace std;


//Escreva um código (em pseudocódigo ou C++) que remova todas as ocorrências de um valor k em uma lista encadeada. Inclua um esboço de função com ponteiros e garanta atualizar corretamente o ponteiro head.
//Dado o ponteiro head para duas listas encadeadas ordenadas em ordem crescente, escreva um programa para mesclá-las em uma única lista encadeada ordenada (similar ao merge de duas listas ordenadas). Forneça apenas o esboço da função em C++:

struct Node {
    int dado;
    Node* prox;
    Node(int val) : dado(val), prox(nullptr) {}
};

Node* merge(Node*& head1, Node*& head2){
    if (head1 == nullptr) {
        return head2;
    }
    if (head2 == nullptr){
        return head1;
    }
    Node* temp1 = head1;
    Node* temp2 = head2;
    Node* Newhead;
    if (head1->dado < head2->dado){
        Newhead = head1;
        temp1 = temp1->prox;
    }
    else{
        Newhead = head2;
        temp2 = temp2->prox;
    }
    Node* temp3 = Newhead;
    while (temp1 != nullptr && temp2 != nullptr){
        if ((temp1->dado <= temp2->dado)){
            temp3->prox = temp1;
            temp3 = temp3->prox;
            temp1 = temp1->prox;
        }
        else if ((temp2->dado <= temp1->dado)){
            temp3->prox = temp2;
            temp3 = temp3->prox;
            temp2 = temp2->prox;
        }
    }
    if (temp1 != nullptr) {
        temp3->prox = temp1;
    }
    if (temp2 != nullptr) {
        temp3->prox = temp2;
    }    
    return Newhead;
}

void remover(int k, Node*& head){
    //garente que o head nao é igual ao k
    while (head != nullptr && head->dado == k){
        Node* temp = head;
        head = head->prox;
        delete(temp);
    }
    //agora verificamos o resto.
    Node* item = head;
    Node *next;
    while (item != nullptr && item->prox != nullptr){
        if (item->prox->dado == k){
            next = item->prox;
            item->prox = item->prox->prox;
            delete(next); 
        }
        else{
            item = item->prox;
        }
        
    }
}

// Função para inserir no final
void insert(Node*& head, int val) {
    Node* novo = new Node(val);
    if (!head) {
        head = novo;
        return;
    }
    Node* temp = head;
    while (temp->prox) {
        temp = temp->prox;
    }
    temp->prox = novo;
}

// Função para imprimir a lista
void printList(Node* head) {
    while (head) {
        cout << head->dado << " ";
        head = head->prox;
    }
    cout << endl;
}

int main() {
    Node* lista1 = nullptr;
    Node* lista2 = nullptr;

    insert(lista1, 1);
    insert(lista1, 3);
    insert(lista1, 5);
    insert(lista1, 5);
    insert(lista1, 7);
    insert(lista1, 9);

    insert(lista2, 2);
    insert(lista2, 4);
    insert(lista2, 6);
    insert(lista2, 10);
    Node* resultado = merge(lista1, lista2);

    cout << "Lista mesclada: ";
    printList(resultado);

    return 0;
}
