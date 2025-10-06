# =========================================
# Cross-platform Makefile for latex-compiler
# Builds bin/app (Linux) or bin/app.exe (Windows)
# =========================================

CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinc -MMD -MP

SRC_DIR  := src
BIN_DIR  := bin
BUILD_DIR:= build

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET := $(BIN_DIR)/app.exe
    MKDIR   = if not exist $(1) mkdir $(1)
    RM      = del /Q
    RMDIR   = rmdir /S /Q
    SEP     = \\
    CD_RUN  = cd $(BIN_DIR) && app.exe
else
    TARGET := $(BIN_DIR)/app
    MKDIR   = mkdir -p $(1)
    RM      = rm -f
    RMDIR   = rm -rf
    SEP     = /
    CD_RUN  = cd $(BIN_DIR) && ./app
endif

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	$(call MKDIR,$(BIN_DIR))

$(BUILD_DIR):
	$(call MKDIR,$(BUILD_DIR))

run: $(TARGET)
	$(CD_RUN)

clean:
	-$(RM) $(BUILD_DIR)$(SEP)* 2>nul || true
	-$(RM) $(TARGET) 2>nul || true
	-$(RMDIR) $(BUILD_DIR) 2>nul || true
	-$(RMDIR) $(BIN_DIR) 2>nul || true

help:
	@echo "Available targets:"
	@echo "  make / make all   - Build the project"
	@echo "  make run          - Build and run the project (cd into bin/)"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make help         - Show this help message"

-include $(DEPS)

.PHONY: all run clean help
