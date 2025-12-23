# Address Book

GUI-застосунок на **C++ / Qt 6** для керування базою контактів.

---

## Функціонал
- Створення, редагування та видалення контактів  
- Додавання фото (локальне збереження)  
- Пошук і сортування (А-Я, Я-А, за номером)  
- Імпорт / експорт: **JSON, CSV, TXT**  
- Валідація телефону та email (Regex)

---

## Вимоги
- **Qt 6.x** (Core, Gui, Widgets)  
- **CMake 3.16+**  
- Компілятор з підтримкою **C++17**

---

## Запуск на Windows

### Через Qt Creator
1. Відкрити **Qt Creator**
2. `File → Open File or Project`
3. Обрати `CMakeLists.txt`
4. Обрати Kit (Qt 6.x MinGW / MSVC)
5. Натиснути **Run**

### Через командний рядок

mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
AddressBook.exe

## Запуск на Linux (Ubuntu / Debian)

### Встановлення залежностей

sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-base-dev-tools

## Збірка та запуск

mkdir build
cd build
cmake ..
make
./AddressBook
