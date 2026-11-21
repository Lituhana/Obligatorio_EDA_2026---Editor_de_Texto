# Makefile - compila todo el proyecto. Uso: make  |  make clean
# main.c (del profe) contiene C++, por eso se compila con g++ -x c++.
CXX = g++
CXXFLAGS = -Wall -g
OBJS = main.o editor.o texto.o linea.o diccionario.o ultimas.o

editor: $(OBJS)
	$(CXX) $(CXXFLAGS) -o editor $(OBJS)

main.o: main.c editor.h definiciones.h
	$(CXX) $(CXXFLAGS) -x c++ -c main.c -o main.o

# Dependencias de headers: si cambia un .h se recompilan los .cpp que lo usan
editor.o: editor.cpp editor.h texto.h linea.h diccionario.h definiciones.h
texto.o: texto.cpp texto.h linea.h diccionario.h definiciones.h
linea.o: linea.cpp linea.h definiciones.h
diccionario.o: diccionario.cpp diccionario.h definiciones.h
ultimas.o: ultimas.cpp ultimas.h definiciones.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o editor