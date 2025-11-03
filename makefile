CXX = g++ -std=c++17
CXXFLAGS = -I/home/linuxbrew/.linuxbrew/Cellar/sfml/3.0.2/include
LDFLAGS = -L/home/linuxbrew/.linuxbrew/Cellar/sfml/3.0.2/lib -lsfml-graphics.so.3.0.2 -lsfml-window.so.3.0.2 -lsfml-system.so.3.0.2

TARGET = main
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
