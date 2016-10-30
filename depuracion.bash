#mkdir testMemoryLeaks && cd  testMemoryLeaks && cmake .. && make && valgrind --leak-check=full --track-origins=yes ./CustomOctree && rm -r * && cd .. && rmdir testMemoryLeaks
mkdir testMemoryLeaks
cd  testMemoryLeaks
cmake ..
make
cp ../keywords .
cp ../regression.d .
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all  ./analizadorLexico 
rm -r * 
cd .. 
rmdir testMemoryLeaks

