CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# 加入 Market.cpp
SRC = main.cpp src/Stock.cpp src/Account.cpp src/Trade.cpp src/Market.cpp

OBJDIR = build

# 取得檔名（無路徑）
OBJFILES = $(notdir $(SRC))
# 物件檔的完整路徑
OBJ = $(patsubst %.cpp,$(OBJDIR)/%.o,$(OBJFILES))

TARGET = StockSimulator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# 根目錄 .cpp 編譯到 build/*.o
$(OBJDIR)/main.o: main.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

# src/ 底下的 .cpp 編譯到 build/*.o
$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)