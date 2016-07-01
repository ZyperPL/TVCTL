CC=g++
LIBS=-lcurl
FLAGS=-I. -I./src -std=c++11 -O3 -I./src/ $(LIBS)

obj/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(FLAGS)

tvctl: obj/main.o obj/download.o obj/series.o obj/database.o
	$(CC) -o tvctl obj/main.o obj/download.o obj/series.o obj/database.o $(LIBS)

clean:
	rm -rf obj/*.o
	rm tvctl
