
# -fsanitize=address -Wall -Wextra -pedantic

CC = clang++

# <==========================> #
CFLAGS_DEBUG = -std=c++20 -Wall -Wextra -pedantic -g
CFLAGS_LAUNCH = -std=c++20
CFLAGS_RELEASE = -O3 -std=c++20
# <==========================> #

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# PROGRAM = $(BIN_DIR)/debug.exe

ifeq (${PROGRAM}, $(BIN_DIR)/debug.exe)
  CFLAGS = ${CFLAGS_DEBUG}
else ifeq (${PROGRAM}, $(BIN_DIR)/main.exe)
  CFLAGS = ${CFLAGS_LAUNCH}
else
  CFLAGS = ${CFLAGS_RELEASE}
endif

$(PROGRAM):$(OBJS)
	@mkdir -p "$(BIN_DIR)"
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p "$(OBJ_DIR)"
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)