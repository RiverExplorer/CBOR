#
# Build the draft
#
#
.SUFFIXES:	.txt .svg .cpp .xml

TOP			=	../..

TITLE		=	draft-royer-cbor-language

VERSION	=	00

SOURCES.xml = \
	Abstract.xml \
	IANA.xml \
	Introduction.xml \
	RequirementsLanguage.xml

ARTWORK.src =

ARTWORK.txt = $(addprefix Drawings/,$(ARTWORK.src))
SVG					= $(ARTWORK.src:.txt=.svg)
ARTWORK.svg = $(addprefix Generated/,$(SVG))

REFERENCES.xml =

# These files get processed by:
#
# cpp -P -nostdinc -C -undef <InFileName> > <OutFileName>
#
# to producte the INCLUDE.xml files.
#
INCLUDE.cpp =

INCLUDE.xml = $(INCLUDE.cpp:.cpp=.xml)

#INCLUDES1 := $(shell /bin/ls include)
#INCLUDES	= $(addprefix include/,$(INCLUDES1))

ANTLR			:= $(shell /bin/ls ANTLR/*.g4)

# The XDR.cpp files are preprocessed with
# "cpp -P -nostdinc -C -undef"
# to produce GENERATED.XDR
#
# The .cpp files include data from the source code.
# this is done so that the source code and the documentation
# are in sync.
#
#XDR.cpp 			:= $(shell /bin/ls include/*-XDR.cpp)
GENERATED.XDR = $(XDR.cpp:.cpp=.xml)

DOCDIR			= .

TOPDOC.xml	= $(TITLE)-$(VERSION).xml

TARGET.txt	=	$(DOCDIR)/$(TITLE).txt

TARGET.html	=	$(DOCDIR)/$(TITLE).html

TARGET.pdf	=	$(DOCDIR)/$(TITLE).pdf

#
# The:  "2> >(grep -v 'Warning: Found SVG with width')" below
# stops a warning message I do not care about from beeing shown.
#
REMOVE_SVG_WARNING = 2> >(grep -v 'Warning: Found SVG with width')

CPP = $(TOP)/MyTools/SimpleCpp

XML2RFC_FLAGS = --v3 \
	--id-is-work-in-progress \
	--allow-local-file-access 

all : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)

txt : $(TARGET.txt)

pdf : $(TARGET.pdf)

html : $(TARGET.html)

$(TARGET.txt) : ALL-ANTLR.g4 ALL-XDR.xml $(ARTWORK.svg) $(ARTWORK.txt) $(TOPDOC.xml)  $(REFERENCES.xml) $(INCLUDES)
	xml2rfc $(XML2RFC_FLAGS) -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TARGET.pdf) : ALL-ANTLR.g4 ALL-XDR.xml $(ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc $(XML2RFC_FLAGS) --pdf -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TARGET.html): ALL-ANTLR.g4 ALL-XDR.xml $(ARTWORK.svg) $(TOPDOC.xml) $(REFERENCES.xml) $(INCLUDES)
	xml2rfc $(XML2RFC_FLAGS) --html -o $@ $(TOPDOC.xml) $(REMOVE_SVG_WARNING)

$(TOPDOC.xml) : draft-00.xml ALL-ANTLR.g4 ALL-XDR.xml $(SOURCES.xml) $(REFERENCES.xml) $(INCLUDES) $(ARTWORK.txt) $(ARTWORK.svg)
	soelim < draft-00.xml | sed '/^\.lf*/d' > temp.xml
	xmllint --xinclude --format temp.xml > $@

ALL-ANTLR.g4 : $(ANTLR)
ifneq ($(ANTLR),)
	cat $(ANTLR) >> $@
endif

ALL-XDR.xml : $(INCLUDE.xml) $(GENERATED.XDR)
ifneq ($(GENERATED.XDR),)
	echo "<section anchor='all-xdr'>" > $@
	echo "<name>Complete XDR</name>" >> $@
	cat $(GENERATED.XDR) >> $@
	echo "</section>" >> $@
endif

$(INCLUDE.xml) : $(INCLUDE.cpp)

scp : $(TARGET.html) $(TARGET.pdf) $(TARGET.txt)
	scp $(TARGET.html) riverexplorer.games:~/public_html/CBOR/$(TARGET.html)
	scp $(TARGET.pdf) riverexplorer.games:~/public_html/CBOR/$(TAEGET.pdf)
	scp $(TARGET.txt) riverexplorer.games:~/public_html/CBOR/$(TARGET.txt)

clean :
	$(RM) tmp.*
	$(RM) $(ARTWORK.svg)
	$(RM) $(TOPDOC.xml)
	$(RM) ALL-ANTLR.g4 ALL-XDR.xml
	$(RM) $(GENERATED.XDR)
	$(RM) temp.xml

Generated/%.svg : Drawings/%.txt
	aasvg < $< > $@ 

include/%.xml : include/%.cpp
	cpp -P -nostdinc -C -undef $< > $@
