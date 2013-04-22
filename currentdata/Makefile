all: gencurrentdata
	./gencurrentdata oscar_vel7052.nc  oscar_vel7057.nc | gzip > ../data/current01.gz 

gencurrentdata: gencurrentdata.cpp
	g++ -o gencurrentdata gencurrentdata.cpp -lnetcdf -lnetcdf_c++ -g
