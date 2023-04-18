export LD_LIBRARY_PATH=/usr/local/lib/
rm test
rm *.o
g++ -c test.cpp
g++ test.o -o test -lsfeMovie -lsfml-graphics -lsfml-window -lsfml-system
