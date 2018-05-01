CC=g++
LIBS=-lcurl
FLAGS=-I. -I./src -std=c++11 -O3 -I./src/ -ggdb $(LIBS)

obj/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(FLAGS)

tvctl: obj/main.o obj/download.o obj/series.o obj/database.o obj/actions.o
	$(CC) -o tvctl obj/main.o obj/download.o obj/series.o obj/database.o obj/actions.o $(LIBS)

clean:
	rm -rf obj/*.o
	rm tvctl

