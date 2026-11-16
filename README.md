# Avaliação experimental de algoritmos de ordenação

Experimento em C que compara o Bubble Sort clássico, o Bubble Sort otimizado com saída antecipada e o Merge Sort em vetores aleatórios, crescentes e decrescentes.

## Requisitos

- Windows
- LLVM-MinGW UCRT com GCC
- PowerShell

## Compilação e execução

O arquivo-fonte chama-se `main.c`. Execute:

```powershell
.\run_benchmark.ps1
```

A compilação usa `gcc -x c -O2`. A medição de tempo usa `QueryPerformanceCounter`. Cada cenário é executado 30 vezes. O programa calcula a média e o desvio padrão amostral dos tempos em milissegundos para cada algoritmo e tamanho de entrada, incluindo até 100.000 elementos.

## Arquivos

- `main.c`: implementação em C e benchmark.
- `run_benchmark.ps1`: compila e executa o benchmark.
- `results.csv`: resultados da execução mais recente, usando `;` como separador.
