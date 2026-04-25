# Sudoku Solver with Dancing Links

–еализаци€ алгоритма Dancing Links (DLX) дл€ решени€ судоку через сведение к задаче точного покрыти€.

## —труктура проекта

- `include/` Ч заголовочные файлы
  - `dancing_links/` Ч матрица Dancing Links
  - `sudoku/` Ч решатель судоку
- `src/` Ч исходные файлы
  - `dancing_links/` Ч реализаци€ DLX
  - `main.cpp` Ч кратка€ информаци€ о проекте
- `app/` Ч консольное приложение ([подробнее](app/README.md))
- `tests/` Ч юнит-тесты ([подробнее](tests/README.md))

## ƒокументаци€

[—генерированна€ документаци€ API](docs/doxygen/html/index.html)

## —борка

```bash
mkdir build && cd build
cmake ..
cmake --build
```

## «апуск
 - SudokuApp.exe Ч интерактивное приложение

 - test_dancing_links.exe Ч тесты матрицы

 - test_sudoku.exe Ч тесты решател€
