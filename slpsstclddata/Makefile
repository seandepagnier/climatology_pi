all: genslpdata gensstdata genclddata
	./genslpdata | gzip > ../data/slp.gz 
	./gensstdata | gzip > ../data/sst.gz 
	./genclddata | gzip > ../data/cld.gz 

slpcoadsclim5079.nc:
	wget -c http://www.jisao.washington.edu/data/coads_climatologies/slpcoadsclim5079.nc

genslpdata: genslpdata.cpp slpcoadsclim5079.nc
	g++ -o genslpdata genslpdata.cpp -lnetcdf -lnetcdf_c++ -g

sstcoadsclim6079.1deg.nc:
	wget -c http://www.jisao.washington.edu/data/coads_climatologies/sstcoadsclim6079.1deg.nc

gensstdata: gensstdata.cpp sstcoadsclim6079.1deg.nc
	g++ -o gensstdata gensstdata.cpp -lnetcdf -lnetcdf_c++ -g


cldccoadsclim5079.nc:
	wget -c http://www.jisao.washington.edu/data/coads_climatologies/cldccoadsclim5079.nc

genclddata: genclddata.cpp cldccoadsclim5079.nc
	g++ -o genclddata genclddata.cpp -lnetcdf -lnetcdf_c++ -g
