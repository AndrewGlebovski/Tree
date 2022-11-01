# Путь к компилятору
COMPILER=g++

# Флаги компиляции
FLAGS=-Wno-unused-parameter -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_

# Папка с объектами
BIN_DIR=binary

# Папка с исходниками и заголовками
SRC_DIR=source

# Папка с исходниками и заголовками библиотек
LIB_DIR=$(SRC_DIR)/lib


all: run


# Завершает сборку
run: $(addprefix $(BIN_DIR)/, main.o tree.o io.o text.o)
	$(COMPILER) $^ -o run.exe


# Предварительная сборка main.cpp
$(BIN_DIR)/main.o: $(addprefix $(SRC_DIR)/, main.cpp tree.hpp)
	$(COMPILER) $(FLAGS) -c $< -o $@


# Предварительная сборка tree.cpp
$(BIN_DIR)/tree.o: $(addprefix $(SRC_DIR)/, tree.cpp tree.hpp)
	$(COMPILER) $(FLAGS) -c $< -o $@


# Предварительная сборка io.cpp
$(BIN_DIR)/io.o: $(addprefix $(SRC_DIR)/, io.cpp io.hpp tree.hpp lib/text.hpp)
	$(COMPILER) $(FLAGS) -c $< -o $@


# Предварительная сборка библиотек
$(BIN_DIR)/%.o: $(addprefix $(LIB_DIR)/, %.cpp %.hpp)
	$(COMPILER) $(FLAGS) -c $< -o $@
