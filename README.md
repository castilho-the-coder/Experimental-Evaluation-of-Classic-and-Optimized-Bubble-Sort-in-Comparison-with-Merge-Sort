# Avaliacao experimental de algoritmos de ordenacao

Experimento em C que compara Bubble Sort classico, Bubble Sort otimizado por parada antecipada e Merge Sort em vetores aleatorios, crescentes e decrescentes.

## Requisitos

- Windows
- LLVM-MinGW UCRT com GCC
- PowerShell

## Compilar e executar

O arquivo-fonte se chama `c` e nao possui extensao. Execute:

```powershell
.\run_benchmark.ps1
```

A compilacao usa `gcc -x c -O2`. A medicao usa `QueryPerformanceCounter`. Cada combinacao e executada 10 vezes e os resultados sao medias em milissegundos.

## Arquivos

- `c`: implementacoes e benchmark em C.
- `run_benchmark.ps1`: compila e executa o benchmark.
- `resultados.csv`: resultados da execucao mais recente, com separador `;`.
- `resultado_alta_resolucao.txt`: saida bruta da execucao mais recente.
- `artigo.pdf`: artigo com metodologia, resultados e discussao.

O PDF deve ser colocado na raiz com o nome `artigo.pdf` antes da publicacao.
