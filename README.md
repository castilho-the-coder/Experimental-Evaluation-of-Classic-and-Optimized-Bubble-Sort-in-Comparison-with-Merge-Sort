# Experimental evaluation of sorting algorithms

C experiment comparing classic Bubble Sort, optimized Bubble Sort with early exit, and Merge Sort on random, ascending, and descending vectors.

## Requirements

- Windows
- LLVM-MinGW UCRT with GCC
- PowerShell

## Build and run

The source file is named `main.c`. Run:

```powershell
.\run_benchmark.ps1
```

The build uses `gcc -x c -O2`. Timing uses `QueryPerformanceCounter`. Each scenario is executed 10 times and the results are averaged in milliseconds.

## Files

- `main.c`: C implementation and benchmark.
- `run_benchmark.ps1`: compiles and runs the benchmark.
- `results.csv`: latest execution results, using `;` as separator.
