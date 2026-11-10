$ErrorActionPreference = 'Stop'
$project = $PSScriptRoot
$compiler = Get-ChildItem "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\MartinStorsjo.LLVM-MinGW.UCRT*" -Recurse -Filter gcc.exe | Select-Object -First 1 -ExpandProperty FullName
if (-not $compiler) { throw 'LLVM-MinGW GCC nao encontrado.' }
& $compiler -x c (Join-Path $project 'c') -O2 -o (Join-Path $project 'programa.exe')
if ($LASTEXITCODE -ne 0) { throw 'Falha na compilacao.' }
& (Join-Path $project 'programa.exe')
