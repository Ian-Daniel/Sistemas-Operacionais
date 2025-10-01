CXX = g++
CXXFLAGS = -O2 -std=c++17 -pthread

all: auxiliar sequencial paralelo_threads paralelo_processos

auxiliar: auxiliar.cpp
	$(CXX) $(CXXFLAGS) -o auxiliar auxiliar.cpp

sequencial: sequencial.cpp
	$(CXX) $(CXXFLAGS) -o sequencial sequencial.cpp

paralelo_threads: paralelo_threads.cpp
	$(CXX) $(CXXFLAGS) -o paralelo_threads paralelo_threads.cpp

paralelo_processos: paralelo_processos.cpp
	$(CXX) $(CXXFLAGS) -o paralelo_processos paralelo_processos.cpp

clean:
	rm -f auxiliar sequencial paralelo_threads paralelo_processos *.o *.txt