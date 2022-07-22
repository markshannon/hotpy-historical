#Copyright (C) 2008-9 Mark Shannon
#GNU General Public License
#For copyright terms see: http://www.gnu.org/copyleft/gpl.html
#
#
CC= gcc -std=c99 -Wall -Werror -fomit-frame-pointer -DNATIVE_LINKAGE -UGVMT_LINKAGE -DGVMT_FASTCALL
OPT = -O3
OPT_PP = -Os
#Use this for gvmt debug mode.
#OPT = -g
#NDBG=-DNDEBUG -DNO_LOCKING
DBG=-g
TKN=-T

# You may need to redefine these to suit your machine
PYTHON=python2.5
BUILD=$(HOTPY)/build
HOTPY=.
BIN=$(HOME)/bin
GVMT=/usr/local/lib/gvmt
I=include
INCLUDES= -Iinclude -I$(BUILD) -I.
BOOT=$(BUILD)/bootstrap
PARSER_SRC=$(BUILD)/src/parser
HEADERS = $I/attributes.h $I/dictobject.h $I/intobject.h $I/sequence.h \
          $I/boolobject.h $I/exception.h $I/listobject.h $I/setobject.h \
          $I/c_lexer.h $I/floatobject.h $I/moduleobject.h $I/sliceobject.h \
          $I/core_objects.h $I/function.h $I/object.h $I/strobject.h \
          $I/descriptors.h $I/guard.h $I/optimise.h $I/thread.h \
          $I/dict_algorithms.h $I/gvmt_user.h $I/os.h $I/typeobject.h \
          $I/dictionary.h $I/hotpy.h $I/rangeobject.h $I/tupleobject.h \
          $I/version.h $I/types.h $I/version.h $I/typeobject.h \
          $I/frameobject.h $I/fileobject.h $I/py_api/object.h \
          $(BUILD)/operators.h $I/typedefs.h structs.h $I/longobject.h \
          $(BUILD)/standard_names.h $I/abstract.h $(BUILD)/parser.h \
		  $I/Python-ast.h $I/bytesobject.h $I/bytearrayobject.h \
		  $I/py_api/structs.h $I/genobject.h $I/symtable.h \
		  $I/deferred_object.h $I/str_bufobject.h $I/hashtable.h \
		  $I/specialiser.h $I/complexobject.h

GC = genimmix2

.SECONDARY:

.PHONY : hotpy interpreter compiler jparser clean prepare

hotpy: interpreter compiler jparser

interpreter: prepare $(BIN)/hotpy

compiler: $(BIN)/hotpy_compiler.so

#Front end stuff to build gsc files:

$(BOOT)/%.gsc : $(HOTPY)/%.c $(HEADERS) $(BUILD)/opcodes.h
	gvmtc $(INCLUDES) -DBOOTSTRAP -DBIN_PATH="\"$(BIN)\"" -o $@ $<

$(BUILD)/%.gsc : $(HOTPY)/%.c $(HEADERS) $(BUILD)/opcodes.h $I/specialiser.h
	gvmtc $(NDBG) -z $(INCLUDES) -DBIN_PATH="\"$(BIN)\"" -o $@ $<

$(BOOT)/%.gso : $(BOOT)/%.gsc
	gvmtas $(TKN) $(DBG) -o $@ $<

$(BUILD)/%.gso : $(BUILD)/%.gsc
	gvmtas $(OPT) $(TKN) -m$(GC) -o $@ $<

$(BOOT)/operators.gsc : $(BUILD)/operators.c
	gvmtc $(INCLUDES) -DBOOTSTRAP -DBIN_PATH="\"$(BIN)\"" -o $@ $<

$(BUILD)/operators.gsc : $(BUILD)/operators.c
	gvmtc $(NDBG) -z $(INCLUDES) -DBIN_PATH="\"$(BIN)\"" -o $@ $<

$(BUILD)/%.gsc : $(BUILD)/%.c $(HEADERS) $(BUILD)/opcodes.h $I/specialiser.h
	gvmtc $(NDBG) -z $(INCLUDES) -DBIN_PATH="\"$(BIN)\"" -o $@ $<

$(BIN)/%: $(BUILD)/%
	cp -f $< $(BIN)

$(BUILD)/operators.h $(BUILD)/operators.c: operator_names.txt make_operators.py
	mkdir -p $(BUILD)/src/parser
	$(PYTHON) make_operators.py $< $(BUILD)/operators $(BUILD)/src/parser/Operators

prepare: ./builtin/operator.py ./builtin/operator.c
	mkdir -p $(BOOT)/vm $(BOOT)/objects $(BOOT)/builtin $(BUILD)/vm $(BUILD)/objects $(BUILD)/builtin \
	$(BUILD)/native/vm $(BUILD)/native/new_parser $(BUILD)/new_parser $(BOOT)/new_parser \
	$(BUILD)/native/objects $(BUILD)/py_api $(BOOT)/py_api $(BUILD)/native/py_api \
	$(BUILD)/native/shared
	rm -f */*.hotpy.pyc

#Parser

jparser: $(PARSER_SRC)/PythonParser.java ${BUILD}/src/parser/Opcodes.java ${BUILD}/src/parser/StandardNames.java
	javac -classpath /usr/share/java/antlr.jar -sourcepath .:$(BUILD)/src -d $(BUILD) $(PARSER_SRC)/PythonLexer.java  $(PARSER_SRC)/PythonParser.java  $(PARSER_SRC)/PythonParserTokenTypes.java parser/Python.java
	cp -r $(BUILD)/parser $(BIN)

${BUILD}/src/parser/Opcodes.java: ${BUILD}/opcodes.h
	$(PYTHON) parser/opcode_gen.py $< $@

${BUILD}/src/parser/StandardNames.java: standard_names.txt
	$(PYTHON) parser/standard_names.py -j $< $@

${BUILD}/standard_names.h: standard_names.txt
	$(PYTHON) parser/standard_names.py -h $< $@

include/version.h: version.txt
	$(PYTHON) version.py
	mv version.h include

$(HOTPY)/py_api/native.c $(HOTPY)/py_api/not_implemented.c: $(HOTPY)/py_api/gen_api.py
	cd $(HOTPY)/py_api; python gen_api.py rst_files

# This needs antlr version 2.7 (Probably any 2.x, not 3.x)
$(PARSER_SRC)/PythonParser.java: parser/python.g
	java -classpath /usr/share/java/antlr.jar antlr.Tool -o $(PARSER_SRC) parser/python.g

$(BUILD)/dis.gso: $(BUILD)/dis.gsc
	gvmtas $(OPT) $(TKN) -m$(GC) -o $@ $<

$(BUILD)/trace_interpreter.gso : $(BUILD)/trace_interpreter.gsc
	gvmtas $(OPT) $(TKN) -m$(GC) -o $@ $<

%.so : %.c

%.o : %.c

$(BOOT)/builtin/%.so : $(BOOT)/builtin/%.gsc
	gvmtas -l $(DBG) -o $(BOOT)/builtin/$*.gso $<
	gvmtlink -2 -l -o $@ $(BOOT)/builtin/$*.gso

$(BUILD)/builtin/%.so : $(BUILD)/builtin/%.gsc
	gvmtas $(OPT) $(TKN) -m$(GC) -l -o $(BUILD)/builtin/$*.gso $<
	gvmtlink -2 -l -o $@ $(BUILD)/builtin/$*.gso

$(BUILD)/opcodes.h : ./vm/interpreter.vmc $I/types.h $(BUILD)/operators.h $I/Python-ast.h
	gvmtic -z $(NDBG) $(INCLUDES) -b $(BUILD)/opcodes.h -o /dev/null $<

$(BUILD)/interpreter.gsc : ./vm/interpreter.vmc $(HEADERS) $(BUILD)/opcodes.h
	gvmtic -z $(NDBG) $(INCLUDES) -o $@ $<

$(BUILD)/trace_interpreter.gsc : ./vm/interpreter.vmc $(BUILD)/interpreter.gsc
	gvmtxc -e -z -DTRACING -ntracing_interpreter $(NDBG) $(INCLUDES) -o $@ $< $(BUILD)/interpreter.gsc

$(BOOT)/interpreter.gsc : ./vm/interpreter.vmc $(HEADERS) $(BUILD)/opcodes.h
	gvmtic -z -DBOOTSTRAP $(NDBG) $(INCLUDES) -o $@ $<

./builtin/operator.py : ./builtin/op_gen.py ./builtin/op_lib.py
	$(PYTHON) ./builtin/op_gen.py ./builtin/op_lib.py > ./builtin/operator.py

./builtin/operator.c : ./builtin/op_genc.py ./builtin/op_lib.c
	$(PYTHON) ./builtin/op_genc.py ./builtin/op_lib.c > ./builtin/operator.c

$I/Python-ast.h: $(HOTPY)/new_parser/Python.asdl $(HOTPY)/new_parser/asdl_c.py
	cd $(HOTPY)/new_parser; python asdl_c.py -h ../include Python.asdl

$(HOTPY)/ast.py: $(HOTPY)/new_parser/Python.asdl $(HOTPY)/new_parser/asdl_c.py
	cd $(HOTPY)/new_parser; python asdl_c.py -p .. Python.asdl

$(HOTPY)/new_parser/Python-ast.c: $(HOTPY)/new_parser/Python.asdl $(HOTPY)/new_parser/asdl_c.py
	cd $(HOTPY)/new_parser; python asdl_c.py -c . Python.asdl

$(BUILD)/parser.h $(HOTPY)/new_parser/parser.c: $(HOTPY)/new_parser/Grammar $(HOTPY)/new_parser/types $(HOTPY)/new_parser/parser_gen.py
	cd new_parser; python parser_gen.py Grammar types parser
	cp new_parser/parser.h $(BUILD)

GSOS = $(BUILD)/vm/attributes.gso $(BUILD)/vm/descriptors.gso \
      $(BUILD)/vm/dictionary.gso $(BUILD)/vm/function.gso $(BUILD)/vm/load.gso \
      $(BUILD)/interpreter.gso $(BUILD)/vm/base.gso \
      $(BUILD)/vm/debug.gso $(BUILD)/trace_interpreter.gso  \
      $(BUILD)/vm/guard.gso $(BUILD)/vm/types.gso $(BUILD)/vm/module.gso \
      $(BUILD)/objects/intobject.gso $(BUILD)/objects/typeobject.gso \
      $(BUILD)/operators.gso $(BUILD)/dis_trace.gso $(BUILD)/peephole.gso \
      $(BUILD)/objects/setobject.gso $(BUILD)/objects/genobject.gso \
      $(BUILD)/objects/strobject.gso $(BUILD)/objects/tupleobject.gso \
      $(BUILD)/objects/moduleobject.gso $(BUILD)/objects/boolobject.gso \
      $(BUILD)/objects/floatobject.gso $(BUILD)/objects/funcobject.gso \
      $(BUILD)/objects/dictobject.gso $(BUILD)/objects/listobject.gso \
      $(BUILD)/objects/sliceobject.gso $(BUILD)/post_specialise.gso \
      $(BUILD)/objects/operatorobject.gso $(BUILD)/objects/object.gso \
	  $(BUILD)/objects/exceptionobject.gso $(BUILD)/objects/threadobject.gso \
	  $(BUILD)/objects/frameobject.gso $(BUILD)/objects/str_bufobject.gso \
      $(BUILD)/vm/disassembler.gso $(BUILD)/dis.gso \
      $(BUILD)/objects/fileobject.gso $(BUILD)/vm/hashtable.gso \
      $(BUILD)/vm/core_objects.gso $(BUILD)/vm/array.gso \
      $(BUILD)/vm/tracing.gso $(BUILD)/objects/bytearrayobject.gso \
      $(BUILD)/py_api/py_type.gso $(BUILD)/specialiser.gso \
      $(BUILD)/vm/specialiser_support.gso $(BUILD)/vm/type_info.gso \
      $(BUILD)/prep_compile.gso $(BUILD)/objects/longobject.gso \
      $(BUILD)/objects/rangeobject.gso $(BUILD)/objects/abstract.gso \
      $(BUILD)/vm/flow_to_dot.gso $(BUILD)/trace2dot.gso \
      $(BUILD)/vm/deferred_object.gso $(BUILD)/deferred_gen.gso \
      $(BUILD)/deferred_cleanup.gso $(BUILD)/py_api/proxies.gso \
	  $(BUILD)/py_api/not_implemented.gso  $(BUILD)/new_parser/c_lexer.gso \
	  $(BUILD)/new_parser/ast.gso $(BUILD)/new_parser/parser.gso \
	  $(BUILD)/new_parser/symtable.gso $(BUILD)/new_parser/codegen.gso \
	  $(BUILD)/new_parser/Python-ast.gso $(BUILD)/new_parser/tokens.gso \
      $(BUILD)/globals_to_consts.gso $(BUILD)/load_cache_use.gso \
      $(BUILD)/insert_tail_calls.gso

BOOT_GSOS = $(BOOT)/vm/attributes.gso $(BOOT)/vm/base.gso \
      $(BOOT)/vm/dictionary.gso $(BOOT)/vm/function.gso $(BOOT)/vm/load.gso \
      $(BOOT)/vm/debug.gso $(BOOT)/interpreter.gso \
      $(BOOT)/objects/intobject.gso $(BOOT)/objects/typeobject.gso \
      $(BOOT)/operators.gso $(BOOT)/vm/descriptors.gso \
      $(BOOT)/objects/setobject.gso $(BOOT)/objects/genobject.gso \
      $(BOOT)/objects/strobject.gso $(BOOT)/objects/tupleobject.gso \
      $(BOOT)/objects/moduleobject.gso $(BOOT)/objects/boolobject.gso \
      $(BOOT)/objects/floatobject.gso $(BOOT)/objects/funcobject.gso \
      $(BOOT)/objects/dictobject.gso $(BOOT)/objects/listobject.gso \
      $(BOOT)/objects/sliceobject.gso  $(BOOT)/vm/type_info.gso\
      $(BOOT)/objects/operatorobject.gso $(BOOT)/objects/object.gso \
      $(BOOT)/vm/guard.gso $(BOOT)/vm/types.gso $(BOOT)/vm/module.gso \
	  $(BOOT)/objects/exceptionobject.gso $(BOOT)/objects/threadobject.gso \
	  $(BOOT)/objects/frameobject.gso $(BOOT)/objects/str_bufobject.gso \
      $(BOOT)/objects/fileobject.gso $(BOOT)/vm/hashtable.gso \
      $(BOOT)/vm/core_objects.gso $(BOOT)/vm/array.gso \
      $(BOOT)/vm/tracing.gso $(BOOT)/objects/bytearrayobject.gso \
      $(BOOT)/py_api/py_type.gso $(BOOT)/objects/longobject.gso  \
      $(BOOT)/objects/rangeobject.gso $(BOOT)/objects/abstract.gso \
	  $(BOOT)/py_api/proxies.gso $(BOOT)/py_api/not_implemented.gso \
	  $(BOOT)/new_parser/ast.gso $(BOOT)/new_parser/parser.gso \
	  $(BOOT)/new_parser/symtable.gso $(BOOT)/new_parser/codegen.gso \
	  $(BOOT)/new_parser/tokens.gso $(BOOT)/new_parser/c_lexer.gso \
	  $(BOOT)/new_parser/Python-ast.gso $(BOOT)/globals_to_consts.gso\
      $(BOOT)/insert_tail_calls.gso

$(BOOT)/base_image.gsc: make_base.py $(HEADERS) $(HOTPY)/ast.py
	export PYTHONPATH=/usr/local/lib/gvmt ; $(PYTHON) make_base.py -o $(BOOT)/base_image.gsc $(HEADERS)

$(BOOT)/marshal.c: $(HEADERS)
	$(PYTHON) /usr/local/lib/gvmt/objects.py -m -o $@ $(HEADERS)

$(BOOT)/scan_funcs.c: $(BOOT)/marshal.c
	echo '#include "scan_funcs.h"' | cat - $< > $@

BOOT_LIB_SO = $(BOOT)/builtin/builtins.so $(BOOT)/builtin/_engine.so $(BOOT)/builtin/sys.so \
         $(BOOT)/builtin/time.so $(BOOT)/builtin/operator.so $(BOOT)/builtin/math.so \
		 $(BOOT)/builtin/_thread.so $(BOOT)/builtin/threading.so $(BOOT)/builtin/__unsafe.so

LIB_SO = $(BUILD)/builtin/builtins.so $(BUILD)/builtin/_engine.so $(BUILD)/builtin/sys.so \
         $(BUILD)/builtin/time.so $(BUILD)/builtin/operator.so $(BUILD)/builtin/math.so \
		 $(BUILD)/builtin/_thread.so $(BUILD)/builtin/threading.so $(BUILD)/builtin/__unsafe.so

BOOT_LIB_GSO = $(BOOT)/builtin/builtins.gso $(BOOT)/builtin/_engine.gso $(BOOT)/builtin/sys.gso \
         $(BOOT)/builtin/time.gso $(BOOT)/builtin/operator.gso $(BUILD)/builtin/math.gso \
		 $(BOOT)/builtin/_thread.gso $(BOOT)/builtin/threading.gso $(BOOT)/builtin/__unsafe.gso

LIB_GSO = $(BUILD)/builtin/builtins.gso $(BUILD)/builtin/_engine.gso $(BUILD)/builtin/sys.gso \
         $(BUILD)/builtin/time.gso $(BUILD)/builtin/operator.gso $(BUILD)/builtin/math.gso \
		 $(BUILD)/builtin/_thread.gso $(BUILD)/builtin/threading.gso $(BUILD)/builtin/__unsafe.gso

NATIVE = vm/native.c vm/os.c objects/mpz.c py_api/api.c py_api/native.c

NATIVE_OBJS = $(BUILD)/native/vm/native.o $(BUILD)/native/vm/os.o \
			  $(BUILD)/native/objects/mpz.o $(BUILD)/native/py_api/api.o \
			  $(BUILD)/native/py_api/native.o $(BUILD)/native/shared/unicodectype.o \
			  $(BUILD)/native/new_parser/parser_native.o

$(BUILD)/native/%.o : %.c
	gcc -g -fomit-frame-pointer -Wall -Werror -DNATIVE_LINKAGE -UGVMT_LINKAGE $(OPT) $(NDBG) -Iinclude -I$(BUILD) -I/usr/include -Iinclude/py_api -c -o $@ $<

$(BUILD)/full_image.o: $(BUILD)/full_image.gso $(BUILD)/vm/engine.gso $(GSOS) $(LIB_GSO)
	gvmtlink -2 -o $@ $(GSOS) $(BUILD)/full_image.gso $(BUILD)/vm/engine.gso $(LIB_GSO)

#Compiler/optimiser parts

$(BUILD)/compiler.cpp: $(BUILD)/interpreter.gsc
	gvmtcc -m$(GC) -o $@ $<

$(BUILD)/globals_to_consts.gsc: $(BUILD)/interpreter.gsc vm/globals_to_consts.vmc  $(HEADERS)
	gvmtxc $(NDBG) $(INCLUDES) -nglobals_to_consts -o $@ vm/globals_to_consts.vmc $(BUILD)/interpreter.gsc

$(BOOT)/globals_to_consts.gsc: $(BOOT)/interpreter.gsc vm/globals_to_consts.vmc  $(HEADERS)
	gvmtxc $(NDBG) $(INCLUDES) -nglobals_to_consts -o $@ vm/globals_to_consts.vmc $(BOOT)/interpreter.gsc

$(BUILD)/insert_tail_calls.gsc: $(BUILD)/interpreter.gsc vm/insert_tail_calls.vmc  $(HEADERS)
	gvmtxc $(NDBG) $(INCLUDES) -ninsert_tail_calls -o $@ vm/insert_tail_calls.vmc $(BUILD)/interpreter.gsc

$(BOOT)/insert_tail_calls.gsc: $(BOOT)/interpreter.gsc vm/insert_tail_calls.vmc  $(HEADERS)
	gvmtxc $(NDBG) $(INCLUDES) -ninsert_tail_calls -o $@ vm/insert_tail_calls.vmc $(BOOT)/interpreter.gsc

$(BUILD)/dis.gsc: $(BUILD)/interpreter.gsc vm/dis.vmc  $(HEADERS)
	gvmtxc -e $(NDBG) $(INCLUDES) -ndisassembler_pass -o $@ vm/dis.vmc $(BUILD)/interpreter.gsc

$(BUILD)/trace2dot.gsc: $(BUILD)/interpreter.gsc vm/trace2dot.vmc  $(HEADERS)
	gvmtxc $(NDBG) $(INCLUDES) -ntrace2dot -o $@ vm/trace2dot.vmc $(BUILD)/interpreter.gsc

$(BUILD)/dis_trace.gsc: $(BUILD)/interpreter.gsc vm/dis_trace.vmc  $(HEADERS)
	gvmtxc -e $(NDBG) $(INCLUDES) -ndisassemble_trace -o $@ vm/dis_trace.vmc $(BUILD)/interpreter.gsc

$(BUILD)/specialiser.gsc: $(BUILD)/interpreter.gsc vm/specialiser.vmc  $(HEADERS) $I/specialiser.h
	gvmtxc -e $(NDBG) $(INCLUDES) -nspecialiser -o $@ vm/specialiser.vmc $(BUILD)/interpreter.gsc

$(BUILD)/prep_compile.gsc: $(BUILD)/interpreter.gsc vm/prep_compile.vmc  $(HEADERS)
	gvmtxc $(NDBG) $(INCLUDES) -nprep_compile -o $@ vm/prep_compile.vmc $(BUILD)/interpreter.gsc

$(BUILD)/peephole.gsc: $(BUILD)/interpreter.gsc vm/peephole.vmc  $(HEADERS) $I/specialiser.h
	gvmtxc $(NDBG) $(INCLUDES) -npeephole -o $@ vm/peephole.vmc $(BUILD)/interpreter.gsc

$(BUILD)/post_specialise.gsc: $(BUILD)/interpreter.gsc vm/post_specialise.vmc  $(HEADERS) $I/specialiser.h
	gvmtxc $(NDBG) $(INCLUDES) -npost_specialise -o $@ vm/post_specialise.vmc $(BUILD)/interpreter.gsc

$(BUILD)/deferred_gen.gsc: $(BUILD)/interpreter.gsc vm/deferred_gen.vmc  $(HEADERS) $I/specialiser.h
	gvmtxc $(NDBG) $(INCLUDES) -ndeferred_gen -o $@ vm/deferred_gen.vmc $(BUILD)/interpreter.gsc

$(BUILD)/deferred_cleanup.gsc: $(BUILD)/interpreter.gsc vm/deferred_cleanup.vmc  $(HEADERS) $I/specialiser.h
	gvmtxc $(NDBG) $(INCLUDES) -ndeferred_cleanup -o $@ vm/deferred_cleanup.vmc $(BUILD)/interpreter.gsc

$(BUILD)/load_cache_use.gsc: $(BUILD)/interpreter.gsc vm/load_cache_use.vmc  $(HEADERS) $I/deferred_object.h
	gvmtxc $(NDBG) $(INCLUDES) -nload_cache_use -o $@ vm/load_cache_use.vmc $(BUILD)/interpreter.gsc

$(BUILD)/%.o: $(BUILD)/%.cpp
	c++ $(PROF) $(OPT_PP) $(NDBG) -c -g -D__STDC_LIMIT_MACROS -Iinclude -I$(BUILD) -o $@ $<

LLVM_LIBS =  -L/usr/local/lib /usr/local/lib/LLVMX86AsmPrinter.o /usr/local/lib/LLVMX86CodeGen.o -lLLVMSelectionDAG -lLLVMAsmPrinter /usr/local/lib/LLVMExecutionEngine.o /usr/local/lib/LLVMJIT.o -lLLVMCodeGen -lLLVMScalarOpts -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMCore -lLLVMSupport -lLLVMSystem

$(BUILD)/make_image.o: $(BOOT_GSOS) $(BOOT)/vm/bootstrap.gso $(BOOT)/make_image.gso $(BOOT)/base_image.gso $(BOOT)/scan_funcs.gso $(BOOT)/vm/user_marshal.gso
	gvmtlink -2 -o $@ $(BOOT_GSOS) $(BOOT)/vm/bootstrap.gso $(BOOT)/make_image.gso $(BOOT)/base_image.gso $(BOOT)/scan_funcs.gso $(BOOT)/vm/user_marshal.gso

new_parser/tokens.c $I/tokens.h : new_parser/token_gen.py
	cd new_parser; $(PYTHON) token_gen.py
	cp new_parser/tokens.h $I

$(BUILD)/lexer_test.o: $(BOOT_GSOS) $(BOOT)/vm/bootstrap.gso $(BOOT)/new_parser/lexer_test.gso $(BOOT)/base_image.gso $(BOOT)/scan_funcs.gso $(BOOT)/vm/user_marshal.gso
	gvmtlink -2 -o $@ $(BOOT_GSOS) $(BOOT)/vm/bootstrap.gso $(BOOT)/new_parser/lexer_test.gso $(BOOT)/base_image.gso $(BOOT)/scan_funcs.gso $(BOOT)/vm/user_marshal.gso

$(BUILD)/ast_test.o: $(BOOT_GSOS) $(BOOT)/vm/bootstrap.gso $(BOOT)/base_image.gso $(BOOT)/new_parser/ast_test.gso $(BOOT)/scan_funcs.gso $(BOOT)/vm/user_marshal.gso
	gvmtlink -2 -o $@ $(BOOT_GSOS) $(BOOT)/vm/bootstrap.gso $(BOOT)/base_image.gso $(BOOT)/new_parser/ast_test.gso $(BOOT)/scan_funcs.gso $(BOOT)/vm/user_marshal.gso

$(BUILD)/make_image: prepare $(BUILD)/make_image.o $(NATIVE_OBJS) $(BOOT_LIB_SO) jparser
	cp $(BOOT_LIB_SO) builtin
	g++ $(PROF) -rdynamic  -o $@ -lm -lgmp -lpthread -ldl -lrt /usr/local/lib/gvmt_debug.o /usr/local/lib/gvmt_gc_none.o $(NATIVE_OBJS) $(BUILD)/make_image.o

$(BUILD)/lexer_test: prepare $(BUILD)/lexer_test.o $(NATIVE_OBJS) $(BOOT_LIB_SO)
	g++ $(PROF) -rdynamic -o $@ -lm -lgmp -lpthread -ldl -lrt /usr/local/lib/gvmt_debug.o /usr/local/lib/gvmt_gc_none.o $(NATIVE_OBJS) $(BUILD)/lexer_test.o

$(BUILD)/ast_test: prepare $(BUILD)/ast_test.o $(NATIVE_OBJS) $(BOOT_LIB_SO)
	g++ $(PROF) -rdynamic -o $@ -lm -lgmp -lpthread -ldl -lrt /usr/local/lib/gvmt_debug.o /usr/local/lib/gvmt_gc_none.o $(NATIVE_OBJS) $(BUILD)/ast_test.o

$(BUILD)/full_image.gsc : $(BUILD)/make_image builtin/operator.py builtin/builtins.py
	$< $@
	$(PYTHON) make_image_post.py $@

$(BUILD)/hotpy_compiler.so: prepare $(BUILD)/compiler.o
	g++ $(PROF) -shared -lm -lpthread -ldl -rdynamic $(DBG) -o $@ $(BUILD)/compiler.o /usr/local/lib/gvmt_compiler.o -lc -lrt $(LLVM_LIBS)

$(BUILD)/hotpy: prepare $(BUILD)/full_image.o $(NATIVE_OBJS) jparser
	g++ $(PROF) -Wl-S -lm -lpthread -ldl -lrt -lgmp -rdynamic $(DBG) -o $@  $(NATIVE_OBJS) $(BUILD)/full_image.o /usr/local/lib/gvmt.o /usr/local/lib/gvmt_gc_$(GC)_tagged.a

clean:
	rm -rf $(BUILD)
	rm -f ./builtin/*.hotpy.pyc ./builtin/*.gso ./builtin/*.so ./builtin/*.o
	rm -f ./builtin/operator.py ./builtin/operator.c
	rm -f ./benchmarks/*.pyc ./benchmarks/*.hotpy.pyc
	rm -f ./Lib/*.hotpy.pyc
#	rm -f ./include/types.h

