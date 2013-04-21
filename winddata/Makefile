all: genwinddata
	./genwinddata uv200001* | gzip > ../data/wind01.gz 
	./genwinddata uv200002* | gzip > ../data/wind02.gz 

genwinddata: genwinddata.cpp zuFile.cpp
	g++ -o genwinddata genwinddata.cpp zuFile.cpp -lbz2 -lz -g -O2
