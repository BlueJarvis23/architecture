####################################################
# Students' Makefile for the CS:APP Architecture Lab
####################################################

# Default team name and version number
TEAM = YHan 
VERSION = 1

# Where are the different parts of the lab should be copied to when they
# are handed in.

all: handin-parta handin-partb handin-partc pack

handin-parta:
	mkdir $(TEAM)
	mkdir $(TEAM)/parta
	cp sim/misc/sum.ys $(TEAM)/parta/$(TEAM)-$(VERSION)-sum.ys
	cp sim/misc/rsum.ys $(TEAM)/parta/$(TEAM)-$(VERSION)-rsum.ys
	cp sim/misc/copy.ys $(TEAM)/parta/$(TEAM)-$(VERSION)-copy.ys


handin-partb:
	mkdir $(TEAM)/partb
	cp sim/seq/seq-full.hcl $(TEAM)/partb/$(TEAM)-$(VERSION)-seq-full.hcl


handin-partc:
	mkdir $(TEAM)/partc
	cp sim/pipe/ncopy.ys $(TEAM)/partc/$(TEAM)-$(VERSION)-ncopy.ys
	cp sim/pipe/pipe-full.hcl $(TEAM)/partc/$(TEAM)-$(VERSION)-pipe-full.hcl

pack:
	tar cvf $(TEAM).tar $(TEAM)

clean:
	(rm -f *~ *.o $(TEAM).tar;rm -rf $(TEAM) ) 
	



