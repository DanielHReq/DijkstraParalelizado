# DijkstraParalelizado
Trabalho final da disciplina de Introdução à Programação Paralela e Distribuída
# Solução
Inicialmente, tentamos paralelizar o algoritmo de Dijkstra criando tabelas de vizinhança para todos os vértices do grafo. No entanto, essa abordagem enfrentou **problemas de condição de corrida**. Como a tabela de caminhos mais curtos precisava ser compartilhada entre diferentes threads, conflitos surgiram quando múltiplas threads tentavam acessar e modificar o mesmo conteúdo simultaneamente. Isso comprometia a integridade dos resultados.

Para resolver esse problema, alteramos a abordagem. Em vez de iniciar operações independentes para cada vértice do grafo, adotamos uma estratégia baseada em uma única raiz. A partir dessa raiz, o algoritmo calcula os caminhos mais curtos para os demais vértices. Essa abordagem reduz a necessidade de compartilhamento de dados entre threads, mitigando o problema de condição de corrida e permitindo uma paralelização mais eficiente.

# Tempos de execução

| Tipo          | Vértices * Arestas | Tempo (ms)     |
|---------------|--------------------|----------------|
| **SEQUENCIAL**                                      |
|               | 10 * 40            | 0.009          |
|               | 100 * 400          | 5.044          |
|               | 1000 * 4000        | 4669.937       |
| **PARALELIZADO**                                    |
| 4 THREADS     | 10 * 40            | 0.177          |
|               | 100 * 400          | 1.844          |
|               | 1000 * 4000        | 1285.598       |
| 8 THREADS     | 10 * 40            | 0.394          |
|               | 100 * 400          | 1.615          |
|               | 1000 * 4000        | 672.938        |
| 16 THREADS    | 10 * 40            | 26.046         |
|               | 100 * 400          | 2.451          |
|               | 1000 * 4000        | 413.901        |

# Análise

---

### **Onde o Sequencial é Melhor**
- **Grafos Pequenos (10 * 40)**:
  - O sequencial é significativamente mais rápido para grafos pequenos. Com apenas 0.009 ms, ele supera todas as versões paralelizadas, mesmo a de 4 threads (0.177 ms).
  - O overhead de criar threads e sincronizar tarefas no modo paralelizado torna o custo maior do que simplesmente processar o grafo sequencialmente.

---

### **Onde o Paralelizado é Melhor**
- **Grafos Médios (100 * 400)**:
  - A paralelização com 8 threads (1.615 ms) e 4 threads (1.844 ms) apresenta tempos menores do que o sequencial (5.044 ms). Aqui, o aumento de tamanho justifica o benefício do paralelismo, e o overhead inicial das threads é diluído.
  - Usar 16 threads começa a ter um custo adicional (2.451 ms).

- **Grafos Grandes (1000 * 4000)**:
  - Para grafos maiores, a vantagem do paralelismo se torna evidente. A versão com 16 threads é a mais rápida (413.901 ms), seguida por 8 threads (672.938 ms). Ambas são muito mais eficientes do que o sequencial (4669.937 ms).
  - Com grafos maiores, o trabalho é naturalmente mais dividido entre as threads, utilizando melhor os recursos computacionais disponíveis.

---

### **Conclusões Gerais**
1. **Sequencial**:
   - Melhor para grafos pequenos onde o overhead do paralelismo é desproporcional ao benefício.
2. **Paralelizado**:
   - Eficiente para grafos médios a grandes, onde os benefícios de dividir o trabalho superam os custos adicionais do gerenciamento de threads.
   - O número ideal de threads depende do tamanho do grafo e do hardware:
     - **4 threads** são suficientes para ganhos em grafos médios.
     - **16 threads** brilham em grafos grandes, mas apresentam diminuição de eficiência para grafos menores devido ao maior overhead.
