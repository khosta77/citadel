# Тестовое задание С++ в 

* Сделал: Степан Филимонов(khosta77)
* telegram: https://t.me/stepan_philimonov
* VK: https://vk.me/stepanphilimonv
* Почта: stepanfilimnov@mail.ru
* github: https://github.com/khosta77
* На связи 25/8

## Задание

### Краткий фоллоу-ап задания



### Из дополнительного можно сделать



### Причина выбора boost::fusion + nlohmann/json

Через метапрограммирование можно быстро считать данные в структуру данных, для удобного использования в
дальнейшем. Так же важно, что у нас будут заранее заданы конкретные форматы структур данных.

## Сборка и запуск

Проект собирается и компилируется! Проверил на `mac OS 15` и `Ubuntu 24`.

1. Работа с Json реализована через [nlohmann/json](https://github.com/nlohmann/json?ysclid=m9h6e6grnw955784922)
 просто:

```cmd
git clone git@github.com:nlohmann/json.git
```

2. Надо проверить зависимости, должен быть `cmake>=3.28`, `clang++>=16.0`, `gtest` и `boost` (На `mac OS`
установлен через homebrew) и еще

* cmake `cmake --version`

* make `make --version`

* lcov `lcov --version`

* genhtml `genhtml --version`

* clang++ `clang++ --version`

3. Сборка & скомпилировать все

```cmd
cmake -DENABLE_COVERAGE=ON -B build
cd build
make
```

4. Подготовка файлов покрытия

```cmd
make coverage
```

5. Вызов программ:

// TODO:

#### Примечание

Возможно такое, что тесты сервера будут выдавать ошибку с `bind`. В системе не успевает пройтий нужная
очистка, просто перезапустите еще раз.

## Об архитектуре решения



### Важно отметить



## Тесты



## P.s.

* Если возникли вопросы по работе, готов на них ответить всегда ответить.

