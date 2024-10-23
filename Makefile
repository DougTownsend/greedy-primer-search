.PHONY: clean install ndebug debug .depend
BUILD_DIR:=build
EXE_DIR:=bin
SOURCE_DIR:=src
TARGET:=$(EXE_DIR)/threaded_gen_qcode
INCLUDE_DIR:=include
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS := $(patsubst $(SOURCE_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.cpp=.o))
SOURCES += main.cpp
OBJECTS += $(BUILD_DIR)/main.o
override CXXFLAGS += -std=c++11 -I$(INCLUDE_DIR) -mavx -Wall
LIBFLAGS := 

#Target specific variables for debugging
debug: CXXFLAGS += -O0 -DDEBUG -g
debug:install

release: CXXFLAGS += -O3
release: install

install:$(TARGET)
	$(info installed target)

$(TARGET):$(OBJECTS)
	$(info compiled and linked)
	mkdir -p $(EXE_DIR)
	$(CXX) $(LIBFLAGS) -o $@ $(filter %.o,$^) $(CXXFLAGS)

$(BUILD_DIR)/%.o:$(SOURCE_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR) $(EXE_DIR)
