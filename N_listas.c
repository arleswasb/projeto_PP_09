#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Estrutura do Nó e da Lista Encadeada
typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
} LinkedList;

// Implementação da função para inserir um novo nó no início da lista
void insert(LinkedList* list, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        // Em um programa real, um tratamento de erro mais robusto seria necessário
        return; 
    }
    newNode->data = value;
    newNode->next = list->head;
    list->head = newNode;
}

// Implementação da função para liberar a memória de uma lista
void free_list(LinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

int main() {
    const int N_INSERTIONS = 100000;
    int M_LISTS;

    printf("Digite o número de listas: ");
    scanf("%d", &M_LISTS);

    if (M_LISTS <= 0) {
        printf("Número de listas deve ser positivo.\n");
        return 1;
    }

    // 1. Aloca dinamicamente um array de listas
    LinkedList* lists = (LinkedList*)malloc(M_LISTS * sizeof(LinkedList));
    // 2. Aloca dinamicamente um array de locks
    omp_lock_t* locks = (omp_lock_t*)malloc(M_LISTS * sizeof(omp_lock_t));

    // Verificação de robustez da alocação
    if (lists == NULL || locks == NULL) {
        fprintf(stderr, "Falha ao alocar memória para listas ou locks.\n");
        free(lists);
        free(locks);
        return 1;
    }

    // Inicializa cada lista e seu respectivo lock
    for (int i = 0; i < M_LISTS; ++i) {
        lists[i].head = NULL;
        omp_init_lock(&locks[i]); // Inicializa o lock
    }

    #pragma omp parallel for
    for (int i = 0; i < N_INSERTIONS; ++i) {
        unsigned int seed = (unsigned int)time(NULL) ^ omp_get_thread_num();
        
        int value_to_insert = rand_r(&seed) % 1001;
        int list_index = rand_r(&seed) % M_LISTS; // Escolhe uma das M listas

        // 3. Adquire o lock explícito para a lista escolhida
        omp_set_lock(&locks[list_index]);
        
        // --- Início da Região Crítica ---
        insert(&lists[list_index], value_to_insert);
        // --- Fim da Região Crítica ---
        
        // 4. Libera o lock
        omp_unset_lock(&locks[list_index]);
    }

    printf("Inserções concluídas.\n");

    long long total_count = 0;
    for (int i = 0; i < M_LISTS; ++i) {
        long count = 0;
        for (Node* current = lists[i].head; current != NULL; current = current->next) count++;
        total_count += count;
    }
    printf("Total de inserções: %lld (esperado: %d)\n", total_count, N_INSERTIONS);

    // 5. Destrói os locks e libera toda a memória
    for (int i = 0; i < M_LISTS; ++i) {
        omp_destroy_lock(&locks[i]);
        free_list(&lists[i]);
    }
    free(lists);
    free(locks);

    return 0;
}

