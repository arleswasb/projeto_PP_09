# Projeto 09: Análise de Estratégias de Sincronização em OpenMP
### Regiões Críticas Nomeadas vs. Locks Explícitos em Listas Encadeadas

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Framework](https://img.shields.io/badge/Framework-OpenMP-orange.svg)
![Licença](https://img.shields.io/badge/Licença-MIT-green.svg)

## Descrição do Projeto

Este projeto, desenvolvido para a disciplina de Programação Paralela (DCA3703), explora e compara diferentes estratégias de sincronização para gerenciar o acesso concorrente a listas encadeadas. O objetivo é analisar o impacto de duas abordagens distintas do OpenMP para resolver o problema clássico de condições de corrida durante múltiplas inserções paralelas em estruturas de dados compartilhadas.

Foram implementadas duas versões para investigar cenários de gerenciamento de recursos:
1.  **Cenário Estático:** Com um número fixo de listas, utilizando regiões críticas nomeadas.
2.  **Cenário Dinâmico:** Com um número `M` de listas definido pelo usuário, exigindo o uso de locks explícitos.

## Conceitos Abordados

* Paralelismo de Dados com a diretiva `#pragma omp parallel for`.
* Sincronização para evitar Condições de Corrida.
* **Regiões Críticas Nomeadas** (`#pragma omp critical (name)`) para múltiplos locks estáticos.
* **Locks Explícitos** (`omp_lock_t`) para sincronização dinâmica e granular.
* Análise comparativa entre Sincronização Estática vs. Dinâmica.
* Locking de Alta Granularidade (*Fine-Grained Locking*).

## Estrutura dos Arquivos

O repositório contém as duas versões do programa, cada uma focada em uma estratégia de sincronização:

* `Duas_listas.c`: Implementação para o cenário estático com **2 listas** e **regiões críticas nomeadas**. Demonstra uma solução de alto nível para um número fixo de recursos.
* `N_listas.c`: Implementação generalizada para **M listas** com **locks explícitos**. Demonstra uma solução flexível e escalável para um número dinâmico de recursos.

## Como Compilar e Executar

O projeto foi desenvolvido em C e utiliza a biblioteca OpenMP. É necessário um compilador com suporte a OpenMP (como o GCC) e a flag `-fopenmp`.

### Compilação

```bash
# Para a versão de 2 listas (estática)
gcc -fopenmp -o duas_listas Duas_listas.c

# Para a versão de M listas (dinâmica)
gcc -fopenmp -o m_listas N_listas.c
````

### Execução

```bash
# Executar a versão estática
./duas_listas

# Executar a versão dinâmica (o programa solicitará o número de listas)
./m_listas
```

## Abordagens Implementadas

### Versão 1: Cenário Estático com Regiões Críticas Nomeadas

Esta versão aborda o problema para um número fixo de duas listas. A sincronização utiliza a diretiva `#pragma omp critical (name)`, fornecendo nomes distintos para os locks de cada lista (`lock_A` e `lock_B`). Isso permite que inserções em listas diferentes ocorram de forma totalmente concorrente, já que os locks são independentes.

### Versão 2: Cenário Dinâmico com Locks Explícitos

Esta versão generaliza o problema para um número `M` de listas definido pelo usuário em tempo de execução. Como as regiões críticas nomeadas não podem ser criadas dinamicamente, a solução utiliza um array de locks do tipo `omp_lock_t`. Um array de `M` locks é alocado, e o `lock[i]` é usado para proteger a `lista[i]`, permitindo um travamento granular e dinâmico.

## Análise e Conclusões

A análise das duas implementações, detalhada no relatório, leva às seguintes conclusões:

  * A escolha do mecanismo de sincronização é uma **decisão de design** ditada pela natureza do problema (recursos estáticos vs. dinâmicos).
  * **Regiões Críticas Nomeadas** são uma abstração simples e eficaz, mas sua aplicabilidade é restrita a cenários onde os recursos são fixos e conhecidos em tempo de compilação.
  * **Locks Explícitos** são essenciais para cenários dinâmicos, oferecendo flexibilidade e escalabilidade ao custo de uma maior complexidade de gerenciamento (ciclo de vida do lock).
  * Em ambos os casos, a estratégia de **alta granularidade** (um lock por lista) é crucial para minimizar a contenção e maximizar o desempenho, evitando a serialização desnecessária das tarefas.

## Autor

  * **Werbert Arles de Souza Barradas**

-----

**Disciplina:** DCA3703 - Programação Paralela - T01 (2025.2)  
**Docente:** Professor Doutor Samuel Xavier de Souza  
**Instituição:** Universidade Federal do Rio Grande do Norte (UFRN)

```
```
