
CXX = clang
CXXFLAGS = -Wall -Wextra -Wpedantic  -std=c++23 -ftemplate-backtrace-limit=0
SRC_DIR = src
INC_DIR = src
LD_FLAGS = 
TOUCH_FILE =
HEADERS = $(wildcard src/*.h src/*/*.h)
CLEAN = 
ifeq ($(RELEASE), 1)
	CXXFLAGS += -O3  -ffunction-sections -fdata-sections -frtti 
else 
	CXXFLAGS += -glldb
endif
ifeq ($(OS), Windows_NT)
	CXXFLAGS += -DWIN32
	OBJ_EXT = obj
	BUILD_DIR = ./bin/win64
	EXEC = $(BUILD_DIR)/main.exe
	CLEAN = powershell "Get-ChildItem -Path $(BUILD_DIR) -Include *.obj, *.exe -Recurse -Force | Remove-Item -Force "
else
	CXXFLAGS += -DUNIX
	LD_FLAGS += -lstdc++ -lm
	OBJ_EXT = o
	BUILD_DIR = bin/linux64
	EXEC = $(BUILD_DIR)/jsoncpp
	CXX = clang
	CLEAN = rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/* $(BUILD_DIR)/*.out 
endif


SRC = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp) 
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .$(OBJ_EXT), $(basename $(SRC)))) 


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC) $(LD_FLAGS)

$(BUILD_DIR)/%.$(OBJ_EXT): %.cpp $(HEADERS) Makefile
	@mkdir -p $(dir $@)
	@echo $<
	@$(CXX) -c $< $(CXXFLAGS) -o $@

clean:
	$(CLEAN)

compile_objects: $(OBJ)

run: $(EXEC) $(OBJ) $(HEADERS)
	$(EXEC)

test:
	echo "sdf"
