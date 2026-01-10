# DEV | DEBUG | RELEASE
MODE ?= DEV
# Arguments for `make run`:
ARGS ?=

TARGET := gladius
CC := gcc-14
MKDIR_P := mkdir -p
RM_RF := rm -rf

LIBS :=
LDLIBS := $(addprefix -l,$(LIBS))
LDFLAGS :=

STANDARD := -std=c23
CFLAGS_DEV := $(STANDARD) -Og -g3 -Wall -Wextra -Wpedantic -Werror \
              -fstack-protector-strong -fsanitize=address,undefined -fno-omit-frame-pointer \
			  -Walloca -Wcast-qual -Wconversion -Wdouble-promotion -Wfloat-equal -Wformat=2 \
			  -Wshadow -Wstack-protector -Wswitch-default -Wswitch-enum -Wundef
CFLAGS_DEBUG := $(STANDARD) -O0 -g3
CFLAGS_RELEASE := $(STANDARD) -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(MODE))

BUILD_DIR := build
TARGET_DIR := $(BUILD_DIR)/$(MODE)
SRC_DIR := src

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(TARGET_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
-include $(DEPS)
CPPFLAGS := -MMD -MP

.DELETE_ON_ERROR:
.PHONY: all run clean

all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c | $(TARGET_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TARGET_DIR):
	$(MKDIR_P) $@

run: $(TARGET_DIR)/$(TARGET)
	@$< $(ARGS)

clean:
	$(RM_RF) $(BUILD_DIR)
