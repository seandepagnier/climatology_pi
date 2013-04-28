all: genwinddata gencurrentdata genslpdata gensstdata genclddata gencyclonedata gencyclonedata1
	./genwinddata uv200001* | gzip > ../data/wind01.gz 
	./genwinddata uv200002* | gzip > ../data/wind02.gz 
	./gencurrentdata oscar_vel7052.nc  oscar_vel7057.nc | gzip > ../data/current01.gz 
	./genslpdata > ../data/sealevelpressure
	./gensstdata > ../data/seassurfacetemperature
	./genclddata > ../data/cloud
	./gencyclonedata tracks.atl > ../data/cyclone-atl
	./gencyclonedata tracks.bio > ../data/cyclone-bio
	./gencyclonedata tracks.bsh > ../data/cyclone-bsh
	./gencyclonedata tracks.bwp > ../data/cyclone-bwp
	./gencyclonedata tracks.epa > ../data/cyclone-epa
	./gencyclonedata tracks.nio > ../data/cyclone-nio
	./gencyclonedata tracks.she > ../data/cyclone-she
	./gencyclonedata1 tracks.spa.dat > ../data/cyclone-spa

genwinddata: genwinddata.cpp zuFile.cpp
	g++ -o genwinddata genwinddata.cpp zuFile.cpp -lbz2 -lz -g -O2

gencurrentdata: gencurrentdata.cpp
	g++ -o gencurrentdata gencurrentdata.cpp -lnetcdf -lnetcdf_c++ -g

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

gencyclonedata: gencyclonedata.cpp
	g++ -o gencyclonedata gencyclonedata.cpp -g

gencyclonedata1: gencyclonedata1.cpp
	g++ -o gencyclonedata1 gencyclonedata1.cpp -g
