# Компиляторы и инструменты
CC = C:/msys64/mingw64/bin/gcc.exe
BISON = bison
FLEX = flex

# Флаги компилятора
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm -static -static-libgcc

# Целевой исполняемый файл
TARGET = compiler.exe

# Исходные файлы (главные)
MAIN_SOURCES = main.c ast_builder.c ast_visualizer.c

# Сгенерированные файлы парсером и лексером
GENERATED_SOURCES = d_parser.tab.c lex.yy.c

# Все исходные файлы
ALL_SOURCES = $(MAIN_SOURCES) $(GENERATED_SOURCES)

# Объектные файлы
OBJECTS = $(ALL_SOURCES:.c=.o)

# Заголовочные файлы
HEADERS = tree_nodes.h ast_builder.h d_parser.tab.h

# ----- Основные правила -----

# Правило по умолчанию (make без аргументов)
all: $(TARGET)

# Построение исполняемого файла
$(TARGET): $(OBJECTS)
	@echo ""
	@echo "========================================"
	@echo "Linking..."
	@echo "========================================"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo ""
	@echo "OK Compilation successful!"
	@echo "Executable: $(TARGET)"
	@echo ""

# ----- Генерация файлов -----

# Генерация парсера с помощью Bison
d_parser.tab.c d_parser.tab.h: d_parser.y
	@echo ""
	@echo "========================================"
	@echo "Running Bison..."
	@echo "========================================"
	$(BISON) -d d_parser.y
	@echo "OK Generated: d_parser.tab.c and d_parser.tab.h"
	@echo ""

# Генерация лексера с помощью Flex
# Зависит от d_parser.tab.h (лексер должен знать о токенах)
lex.yy.c: d_lexer_par.l d_parser.tab.h
	@echo ""
	@echo "========================================"
	@echo "Running Flex..."
	@echo "========================================"
	$(FLEX) d_lexer_par.l
	@echo "OK Generated: lex.yy.c"
	@echo ""

# ----- Компиляция объектных файлов

# Правило для компиляции всех .c файлов в .o
%.o: %.c $(HEADERS)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# ----- Утилиты -----

# Очистка временных файлов
clean:
	@echo ""
	@echo "========================================"
	@echo "Cleaning up..."
	@echo "========================================"
	rm -f *.o
	rm -f d_parser.tab.c d_parser.tab.h
	rm -f lex.yy.c
	rm -f $(TARGET)
	@echo "OK Cleaned up"
	@echo ""

# Полная очистка (включая зависимости)
distclean: clean
	@echo "Full cleanup complete"

# Тестирование (сборка + запуск на test.d)
test: $(TARGET)
	@echo ""
	@echo "========================================"
	@echo "Running test..."
	@echo "========================================"
	@echo ""
	$(TARGET) test.d
	@echo ""

# Пересборка с нуля
rebuild: clean all

# Помощь
help:
	@echo ""
	@echo "========================================"
	@echo "Available targets:"
	@echo "========================================"
	@echo "  all      - Build compiler (default)"
	@echo "  clean    - Remove generated files"
	@echo "  test     - Build and run on test.d"
	@echo "  rebuild  - Clean and build"
	@echo "  help     - Show this help"
	@echo ""

# Объявить фиксированные правила (не зависящие от файлов)
.PHONY: all clean distclean test rebuild help
