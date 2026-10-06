.PHONY: all clean	
all: numPrimos serial parallel

serial: numPrimos.cpp
	g++ -fopenmp numPrimos.cpp -o serial

parallel:
	g++ numPrimos.cpp -o numPrimos -fopenmp

clean: 
	rm -fr serial parallel numPrimos
