$ErrorActionPreference = 'Stop'
$project = $PSScriptRoot

$compiler = Get-Command gcc -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty Source
if (-not $compiler) {
    $msysGcc = Get-ChildItem 'C:\msys64\mingw64\bin\gcc.exe' -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
    if ($msysGcc) { $compiler = $msysGcc }
}
if (-not $compiler) {
    $compiler = Get-ChildItem "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\MartinStorsjo.LLVM-MinGW.UCRT*" -Recurse -Filter gcc.exe | Select-Object -First 1 -ExpandProperty FullName
}
if (-not $compiler) { throw 'GCC não encontrado. Instale o MinGW ou o LLVM-MinGW UCRT.' }

& $compiler -x c (Join-Path $project 'main.c') -O2 -o (Join-Path $project 'programa.exe') -lm
if ($LASTEXITCODE -ne 0) { throw 'A compilação falhou.' }

Push-Location $project
$output = & (Join-Path $project 'programa.exe') 2>&1
Pop-Location
$output | Set-Content -Encoding UTF8 (Join-Path $project 'execution_output.txt')
Copy-Item -Force (Join-Path $project 'execution_output.txt') (Join-Path $project 'high_resolution_output.txt')

$output
