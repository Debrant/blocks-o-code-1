INSTALL=install
MKDIR=install -d
MAKE=make

ROOTOPT=/opt/abc
OPTABC=$(ROOTOPT)/abc
OPTBIN=$(OPTABC)/interpreter
OPTOUTPUTS=$(ROOTOPT)/outputs

LOCALABC=abc/interpreter/
BINTARGETS=$(LOCALABC)/abc

ABCTARGETS=$(wildcard abc/*.py) 
OUTPUTTARGETS=$(wildcard outputs/*.py)
ROOTTARGETS=$(wildcard *.py)

SERVICEDTARGET=abc.service
SERVICEDROOT=/etc/systemd/system/

all: $(BINTARGETS)

install: all
	$(MKDIR) '$(ROOTOPT)'
	$(INSTALL) $(ROOTTARGETS) '$(ROOTOPT)/'

	$(MKDIR) '$(OPTABC)'
	$(INSTALL) $(ABCTARGETS) '$(OPTABC)/'

	$(MKDIR) '$(OPTBIN)'
	$(INSTALL) $(BINTARGETS) '$(OPTBIN)/'

	$(MKDIR) '$(OPTOUTPUTS)'
	$(INSTALL) $(OUTPUTTARGETS) '$(OPTOUTPUTS)/'

	$(INSTALL) $(SERVICEDTARGET) '$(SERVICEDROOT)/'

clean:
	$(MAKE) -C '$(LOCALABC)' clean

$(BINTARGETS):
	$(MAKE) -C '$(LOCALABC)'
