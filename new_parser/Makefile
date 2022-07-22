#Copyright (C) 2008-9 Mark Shannon
#GNU General Public License
#For copyright terms see: http://www.gnu.org/copyleft/gpl.html
#
# It is essential to use the -r flag. ie: 
# Type: make -r hotpy NOT make hotpy
# This is because GNU make uses implicit rules for creating 
# object files from C source files.
#
CC= gcc -std=c99 -fomit-frame-pointer -Wall -Werror -DNATIVE_LINKAGE -UGVMT_LINKAGE -DGVMT_FASTCALL
OPT = -O3
NDBG=-DNDEBUG
DBG=-g

# You will need to redefine these to suit your machine
BUILD=$(HOTPY)/build
HOTPY=..
BIN=$(HOME)/bin
GVMT=/usr/local/lib/gvmt

INCLUDES= -I../include -I.
BOOT=$(BUILD)/bootstrap

HEADERS = $(HOTPY)/include $(BUILD)/typedefs.h $(BUILD)/structs.h 
GC = copy_threads

lexer_test: prepare $(BUILD)/link.o
	g++ -Wl-S -lm -lpthread -ldl -lrt -rdynamic $(DBG) -o $@ $(BUILD)/link.o /usr/local/lib/gvmt.o /usr/local/lib/gvmt_gc_$(GC).o

prepare: 
	mkdir -p $(BUILD)
    
GSOS = $(BUILD)/lexer_test.gso $(BUILD)/c_lexer.gso

$(BUILD)/%.gsc : ./%.c $(HEADERS)
	gvmtc $(NDBG) $(INCLUDES) -I$(BUILD) -o $@ $<
	
$(BUILD)/%.gso : $(BUILD)/%.gsc
	gvmtas $(OPT) -m$(GC) -o $@ $<
	
$(BUILD)/structs.h : ../vm/types.types
	export PYTHONPATH=/usr/local/lib/gvmt ; python /usr/local/lib/gvmt/objects.py -s -o $@ $<
	
$(BUILD)/typedefs.h : ../vm/types.types
	export PYTHONPATH=/usr/local/lib/gvmt ; python /usr/local/lib/gvmt/objects.py -t -o $@ $<
 
lexer: prepare $(BUILD)/link.o
	g++ -Wl-S -lm -lpthread -ldl -lrt -rdynamic $(DBG) -o $@ $(BUILD)/link.o /usr/local/lib/gvmt.o /usr/local/lib/gvmt_gc_$(GC).o

$(BUILD)/link.o: $(GSOS) 
	gvmtlink -o $@ $(GSOS)

clean :
	rm -rf $(BUILD) 

