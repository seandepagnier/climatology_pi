DATA = ../data

ALL_WIND = $(DATA)/wind01.gz $(DATA)/wind02.gz
ALL_CURRENT = $(DATA)/current01.gz
ALL_OTHER = $(DATA)/sealevelpressure $(DATA)/seasurfacetemperature $(DATA)/cloud
ALL_CYCLONE = $(DATA)/cyclone-atl $(DATA)/cyclone-bio $(DATA)/cyclone-bsh $(DATA)/cyclone-bwp $(DATA)/cyclone-epa $(DATA)/cyclone-she $(DATA)/cyclone-spa
ALL = $(ALL_WIND) $(ALL_CURRENT) $(ALL_OTHER) $(ALL_CYCLONE)

all: $(ALL)
clean:
	rm -rf $(ALL)
	rm -rf genclddata gencurrentdata gencyclonedata gencyclonedata1 genslpdata gensstdata genwinddata

$(DATA)/wind01.gz: uv200001* genwinddata
	./genwinddata uv200001* | gzip > $@

$(DATA)/wind02.gz: uv200002* genwinddata
	./genwinddata uv200002* | gzip > $@

JANUARY_CURRENTS  = oscar_vel7047.nc oscar_vel7032.nc oscar_vel7057.nc
JANUARY_CURRENTS += oscar_vel7042.nc oscar_vel7037.nc oscar_vel7027.nc
JANUARY_CURRENTS += oscar_vel7052.nc
$(DATA)/current01.gz: gencurrentdata
	./gencurrentdata $(JANUARY_CURRENTS) | gzip > $@

genwinddata: genwinddata.cpp zuFile.cpp
	g++ -o genwinddata genwinddata.cpp zuFile.cpp -lbz2 -lz -g -O2

gencurrentdata: gencurrentdata.cpp
	g++ -o gencurrentdata gencurrentdata.cpp -lnetcdf -lnetcdf_c++ -g

$(DATA)/sealevelpressure: slpcoadsclim5079.nc genslpdata
	./genslpdata > $@

slpcoadsclim5079.nc:
	wget -c http://www.jisao.washington.edu/data/coads_climatologies/slpcoadsclim5079.nc

genslpdata: genslpdata.cpp slpcoadsclim5079.nc
	g++ -o genslpdata genslpdata.cpp -lnetcdf -lnetcdf_c++ -g

$(DATA)/seasurfacetemperature: sstcoadsclim6079.1deg.nc gensstdata
	./gensstdata > $@

sstcoadsclim6079.1deg.nc:
	wget -c http://www.jisao.washington.edu/data/coads_climatologies/sstcoadsclim6079.1deg.nc

gensstdata: gensstdata.cpp sstcoadsclim6079.1deg.nc
	g++ -o gensstdata gensstdata.cpp -lnetcdf -lnetcdf_c++ -g

$(DATA)/cloud: cldccoadsclim5079.nc genclddata
	./genclddata > $@

cldccoadsclim5079.nc:
	wget -c http://www.jisao.washington.edu/data/coads_climatologies/cldccoadsclim5079.nc

genclddata: genclddata.cpp cldccoadsclim5079.nc
	g++ -o genclddata genclddata.cpp -lnetcdf -lnetcdf_c++ -g

$(DATA)/cyclone-%: tracks.% gencyclonedata
	./gencyclonedata $< > $@

$(DATA)/cyclone-%: tracks.%.dat gencyclonedata1
	./gencyclonedata1 $< > $@

gencyclonedata: gencyclonedata.cpp
	g++ -o gencyclonedata gencyclonedata.cpp -g

gencyclonedata1: gencyclonedata1.cpp
	g++ -o gencyclonedata1 gencyclonedata1.cpp -g
