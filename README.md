# LabMatrix

Лабораторная работа №1 «Матрицы».

Проект содержит:

- статическую библиотеку с шаблонными классами `TMemData` и `TVector`;
- консольное приложение;
- модульные тесты GoogleTest.

## Сборка

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

