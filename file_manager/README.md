# Контрольная работа №4 Суринов Артём ЭФБО-03-24
# Двухпанельный файловый менеджер

## Краткая суть проекта

Приложение представляет собой двухпанельный файловый менеджер для macOS, разработанный на C++ с использованием Qt6. Программа позволяет выполнять навигацию по файловой системе и основные операции с файлами и папками (копирование, перемещение, удаление, создание папок, переименование) с помощью горячих клавиш в стиле Total Commander.

### Реализованные паттерны проектирования
- **Singleton** – для единственного экземпляра FileOperationsFacade
- **Command** – для всех файловых операций
- **Facade** – для упрощённого доступа к файловой системе
- **Observer** – для автоматического обновления панелей
- **Mediator** – для координации между двумя панелями

### Основные возможности
- Две независимые панели для просмотра каталогов
- Переключение между панелями (Tab)
- Навигация по папкам (Enter, Backspace)
- Копирование файлов и папок (F5)
- Перемещение файлов и папок (F6)
- Удаление с подтверждением (F8)
- Создание папок (F7)
- Переименование (F2)
- Открытие файлов в приложении по умолчанию (Enter)
- вставка файлов и папок (F9)
- 
## Инструкция по сборке

### Требования
- CMake версии 3.16 или выше
- Компилятор C++ с поддержкой стандарта C++17
- Qt6 (Core, Widgets, Gui, Test)
- Catch2 v3
- macOS (проект нацелен на macOS)

### Установка Qt6 на macOS
```bash
brew install qt@6
```

### Установка Catch2 на macOS
```bash
brew install catch2
```

## Тестирование (Catch2)

### Запуск unit-тестов
```bash
cmake --build /Users/artem/CLionProjects/file_manager/cmake-build-debug
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/file_manager_tests
```

### Список тестовых сценариев
1. Копирование файла, перемещение результата и удаление.
2. Создание папки, переименование файла и удаление папки.
3. Копирование каталога с вложениями, перемещение и удаление.
4. Проверка обработки ошибок (копирование отсутствующего файла).

### Запуск сценарных мини-программ
```bash
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_copy_move
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_create_rename
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_directory_transfer
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_error_handling
```

### Список unit-тестов по функциям (5–7 на каждую основную)

**FileOperationsFacade** (`tests/FileOperationsFacadeTest.cpp`)
- `copy` — 5 тестов (копия файла, создание каталогов, перезапись, рекурсивная папка, отсутствующий источник).
- `move` — 5 тестов (перемещение, создание каталогов, перезапись, рекурсивная папка, отсутствующий источник).
- `remove` — 5 тестов (удаление файла, пустой папки, папки с содержимым, дерева, отсутствующего пути).
- `createDir` — 5 тестов (создание, вложенные, уже существует, путь-файл, пробелы в имени).
- `getLastError` — 5 тестов (сообщения ошибок для copy/move/remove/createDir и создание каталога назначения).

**FileCommand** (`tests/FileCommandTest.cpp`)
- `CopyCommand::execute/undo` — 5 тестов (копия файла/папки, создание каталогов, перезапись, undo).
- `MoveCommand::execute/undo` — 5 тестов (перемещение файла/папки, создание каталогов, перезапись, undo).
- `DeleteCommand::execute/undo` — 5 тестов (удаление файла/папки/дерева, отсутствующий путь, undo не поддерживается).
- `CreateDirCommand::execute/undo` — 5 тестов (создание, вложенные, уже существует, путь-файл, undo).
- `RenameCommand::execute/undo` — 5 тестов (переименование файла/папки, отсутствующий источник, undo, создание каталога назначения).

**FilePanel** (`tests/FilePanelTest.cpp`)
- `setPath` — 5 тестов (обновление пути, root, смена каталога, отсутствующий путь, повторная установка).
- `refresh` — 5 тестов (".." для не-root, отсутствие ".." в root, выбор первого элемента, обновление списка, папки).
- `getSelectedItem` — 5 тестов (текущий элемент, пустой выбор, "..", файл, пустой список).
- `setActive` — 5 тестов (активный/неактивный стиль, цвета, переключение туда‑обратно).
- обработка ввода (`event`/`keyPressEvent`) — 6 тестов (Enter, Backspace, F5, F2, Tab, Backtab).

**PanelMediator** (`tests/PanelMediatorTest.cpp`)
- `switchActivePanel/getActivePanel/getInactivePanel` — 6 тестов (начальное состояние, переключения, стили, корректность inactive).
- `handleBackspace` — 5 тестов (root, родитель, глубокий путь, home, повторный root).
- `handleEnter` — 5 тестов ("..", каталог, пустой выбор, каталог с пробелами, пустая селекция).
- `refreshAllPanels` — 5 тестов (файл, каталог, разные пути, удаление, "..").

**MainWindow** (`tests/MainWindowTest.cpp`)
- конструктор и инициализация UI — 5 тестов (title, 2 панели, стартовые пути, статус‑строка, активная панель).

## Соответствие заданию (по пунктам)

1) **Подключение Catch2 и сборка тестов в отдельную директорию**
   - Используется Catch2 v3, точка входа в `tests/TestMain.cpp`.
   - Тестовый бинарник собирается отдельно: `cmake-build-debug/tests/file_manager_tests`.
   - Сценарные мини‑программы также лежат в `cmake-build-debug/tests/`.

2) **Unit‑тесты для каждого класса и основных функций (5–7 проверок)**

**FileOperationsFacade** (`tests/FileOperationsFacadeTest.cpp`)
- `copy` — копирование файла и содержимого, создание папки назначения, перезапись, рекурсивная папка, отсутствующий источник.
- `move` — перемещение файла, создание папки назначения, перезапись, рекурсивная папка, отсутствующий источник.
- `remove` — удаление файла, пустой папки, папки с содержимым, дерева, отсутствующего пути.
- `createDir` — создание, вложенные папки, уже существует, путь‑файл, пробелы в имени.
- `getLastError` — сообщения об ошибках для `copy/move/remove/createDir`, отдельная проверка ошибки создания папки назначения.

**FileCommand** (`tests/FileCommandTest.cpp`)
- `CopyCommand::execute/undo` — копия файла, создание каталогов, перезапись, рекурсивная папка, undo.
- `MoveCommand::execute/undo` — перемещение файла, создание каталогов, перезапись, рекурсивная папка, undo.
- `DeleteCommand::execute/undo` — удаление файла, папки, дерева, отсутствующий путь, undo не поддерживается.
- `CreateDirCommand::execute/undo` — создание, вложенные папки, уже существует, путь‑файл, undo.
- `RenameCommand::execute/undo` — переименование файла и папки, отсутствующий источник, undo, создание папки назначения.

**FilePanel** (`tests/FilePanelTest.cpp`)
- `setPath` — обновление пути, root, смена каталога, отсутствующий путь, повторная установка.
- `refresh` — ".." для не‑root, отсутствие ".." в root, выбор первого элемента, обновление списка после создания файла, отображение папок.
- `getSelectedItem` — текущий элемент, пустой выбор, "..", выбранный файл, пустой список.
- `setActive` — активный/неактивный стиль и переключение туда‑обратно.
- обработка ввода (`event`/`keyPressEvent`) — Enter, Backspace, F5, F2, Tab, Backtab.

**PanelMediator** (`tests/PanelMediatorTest.cpp`)
- `switchActivePanel/getActivePanel/getInactivePanel` — начальное состояние, переключения, стили, корректность inactive.
- `handleBackspace` — root, родитель, глубокий путь, home, повторный root.
- `handleEnter` — "..", каталог, пустой выбор, каталог с пробелами, очищенная селекция.
- `refreshAllPanels` — новый файл, новая папка, разные пути, удаление, сохранение "..".

**MainWindow** (`tests/MainWindowTest.cpp`)
- Конструктор/инициализация UI — заголовок, наличие 2 панелей, стартовые пути, статус‑подсказка, одна активная панель.

3) **Список тестовых сценариев**
- Копирование файла, затем перемещение результата и удаление.
- Создание папки, переименование файла и удаление папки.
- Копирование каталога с вложениями, перемещение и удаление.
- Обработка ошибки при копировании отсутствующего файла.

4) **Реализация сценариев в отдельных мини‑программах**
- `tests/scenarios/ScenarioCopyMove.cpp` — сценарий копирование → перемещение → удаление.
- `tests/scenarios/ScenarioCreateRename.cpp` — создание папки → переименование файла → удаление папки.
- `tests/scenarios/ScenarioDirectoryTransfer.cpp` — копирование каталога → перемещение → удаление.
- `tests/scenarios/ScenarioErrorHandling.cpp` — проверка ошибки копирования несуществующего файла.

Запуск сценариев:
```bash
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_copy_move
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_create_rename
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_directory_transfer
/Users/artem/CLionProjects/file_manager/cmake-build-debug/tests/scenario_error_handling
```
