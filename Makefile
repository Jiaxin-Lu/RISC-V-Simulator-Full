CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = code
OBJ = main.cpp Decode.cpp Executor.cpp Fetch.cpp MemoryManager.cpp Simulation.cpp Tool.cpp WriteBack.cpp Predictor.cpp

$(TARGET) : $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ) $(CXXFLAGS)
	
clean : 
	rm -f
