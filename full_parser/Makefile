# Компиляторы и инструменты
CC = C:/msys64/mingw64/bin/gcc.exe
CXX = C:/msys64/mingw64/bin/g++.exe
LD = $(CXX)
BISON = bison
FLEX = flex
CMAKE ?= cmake

# Try to find cmake in common MSYS2 paths
MSYS2_CMAKE := $(firstword $(wildcard C:/msys64/mingw64/bin/cmake.exe C:/msys64/usr/bin/cmake.exe))
ifneq ($(MSYS2_CMAKE),)
  CMAKE := $(MSYS2_CMAKE)
endif

# Флаги компилятора
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm -static -static-libgcc -static-libstdc++

# JVM Class Builder (C++ static library)
JVMCB_DIR ?= JVMClassBuilder
JVMCB_BUILD_DIR ?= $(JVMCB_DIR)/out/build/x64-Debug
JVMCB_LIB ?= $(JVMCB_BUILD_DIR)/libjvm-class-builder.a
JVMCB_INCLUDES ?= -I$(JVMCB_DIR)/include

# Целевой исполняемый файл
TARGET = compiler.exe

# Исходные файлы (главные)
MAIN_SOURCES = main.c ast_builder.c ast_visualizer.c codegen_jvm.c
SEMANTIC_SOURCES = semantic/semantic_analyzer.c semantic/semantic_passes.c \
	semantic/name_resolution.c semantic/type_inference.c \
	semantic/error_reporting.c semantic/jvm_layout.c \
	semantic/semantic_transforms.c semantic/jvm_codegen_helpers.c

# Сгенерированные файлы парсером и лексером
GENERATED_SOURCES = d_parser.tab.c lex.yy.c

# Все исходные файлы
ALL_SOURCES = $(MAIN_SOURCES) $(SEMANTIC_SOURCES) $(GENERATED_SOURCES)

# Объектные файлы
OBJECTS = $(ALL_SOURCES:.c=.o)

# Заголовочные файлы
HEADERS = tree_nodes.h ast_builder.h d_parser.tab.h

# ----- Основные правила -----

# Правило по умолчанию (make без аргументов)
all: $(TARGET)

# Построение исполняемого файла
$(TARGET): $(OBJECTS) $(JVMCB_LIB)
	@echo ""
	@echo "========================================"
	@echo "Linking..."
	@echo "========================================"
	$(LD) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo ""
	@echo "OK Compilation successful!"
	@echo "Executable: $(TARGET)"
	@echo ""

# ----- JVM Class Builder -----

$(JVMCB_LIB): $(JVMCB_DIR)/CMakeLists.txt \
	$(JVMCB_DIR)/src/jvmc_wrapper.cpp \
	$(JVMCB_DIR)/include/jvmc/jvmc.h
	@echo ""
	@echo "========================================"
	@echo "Building JVM Class Builder..."
	@echo "========================================"
	@if [ -f "$(JVMCB_BUILD_DIR)/CMakeCache.txt" ]; then rm -rf "$(JVMCB_BUILD_DIR)"; fi
	$(CMAKE) -S $(JVMCB_DIR) -B $(JVMCB_BUILD_DIR) -DJVMC_ENABLE_JAVA_FIX=OFF
	$(CMAKE) --build $(JVMCB_BUILD_DIR)
	@echo ""
	@echo "OK JVM Class Builder built: $(JVMCB_LIB)"
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
	$(CC) $(CFLAGS) $(JVMCB_INCLUDES) -c $< -o $@

# ----- Утилиты -----

# Очистка временных файлов
clean:
	@echo ""
	@echo "========================================"
	@echo "Cleaning up..."
	@echo "========================================"
	find . -name "*.o" -type f -delete
	find . -name "*.obj" -type f -delete
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
