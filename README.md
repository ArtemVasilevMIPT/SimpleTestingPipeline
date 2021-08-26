# SimpleTestingPipeline
Простая система автоматического тестирования, созданная для облегчения жизни тех, кто проходит курс ООП в МФТИ

## Dependencies
* GNU GCC 9.x.x
* Python 3.x

## Запуск
* Склонировать репозиторий
* Запустить файл run_tests.sh с параметрами %путь к .h файлу% %путь к файлу с тестами%. Например: ./run_tests.sh example/biginteger.h example/BI_test.cpp

## Формат написания тестов
```
#include "TestCase.h"
#include "%Ваш файл с кодом%"
  
//Пример теста
TEST_F(TestCase, %имя теста%)
{
  //Тело теста
}
```
