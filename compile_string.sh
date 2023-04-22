export LD_LIBRARY_PATH=/usr/local/lib/
rm test_string
rm *.o
g++ -c test_string.cpp
g++ test_string.o -o test_string -lsfeMovie -lsfml-graphics -lsfml-window -lsfml-system
