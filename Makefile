.DELETE_ON_ERROR:

TARGET	:= app
CC		:= gcc
LINK	:= gcc

IR		= true
RM 		= rm -f

SRC_DIR ?= src
OBJ_DIR ?= obj
BIN_DIR ?= bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
INCS := $(wildcard $(SRC_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)

ifeq ($(CC), gcc)
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wmissing-prototypes -Wshadow -MMD -MP
LDFLAGS += -lSDL2main -lSDL2

ifeq ($(IR), true)
CFLAGS += -save-temps=$(OBJ_DIR)
endif

ifeq ($(OS), Windows_NT)
LDFLAGS += -lmingw32
endif

OPT += -O3 -flto -fomit-frame-pointer
endif

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(LINK) $(OPT) $^ $(LDFLAGS) -o $@
	@echo "Linking complete."

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INCS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully."

.PHONY: clean remove
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "Generated files removed."

remove: clean
	@$(RM) -r $(BIN_DIR)
	@echo "Executable removed."

-include $(DEPS)
