include config.mk

generate_lib: libdebug_c.a $(TARGET).a
	ar -t $(TARGET).a

generate_lib_debug: libdebug_c.a $(TARGET)_debug.a
	ar -t $(TARGET).a

all: generate_lib
	$(MAKE) -C . -f $(MAKE_NAME) examples

examples: generate_lib
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(PATH_EXAMPLES)/example.c $(LINKER_FLAGS) -o example.$(EXTENSION)
examples_debug: generate_lib_debug
	$(CC) $(CFLAGS_DEBUG) $(INCLUDE_FLAGS) $(PATH_EXAMPLES)/example.c $(LINKER_FLAGS) -o example.$(EXTENSION)

debug: generate_lib_debug examples_debug
	ar -t $(TARGET).a

libdebug_c.a:
	echo "generando librerias estatica... $@"
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME)

$(TARGET_DEBUG).a: $(OBJECTS)
	echo "generando librerias estatica... $@"
	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@

$(TARGET).a: $(OBJECTS)
	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@

$(TARGET)_debug.a: $(OBJECTS_DEBUG)
	$(ARR) $(ARR_FLAGS) $(TARGET).a $^
	ranlib $(TARGET).a

BigInt.o: $(PATH_SRC)/BigInt.c
	$(CC) $(CFLAGS) -c $^ -o $@

BigInt_debug.o: $(PATH_SRC)/BigInt.c
	$(CC) $(CFLAGS_DEBUG) -c $^ -o $@

cleanobj:
	$(RM) $(RMFLAGS) $(OBJECTS)
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME) cleanobj

cleanall: cleanobj
	$(RM) $(RMFLAGS) *.o $(TARGET).a \
	$(TARGET_DEBUG).a *.exe
	$(RM) $(RMFLAGS) *.$(EXTENSION) 
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME) cleanall

.SILENT: clean cleanobj cleanall
.IGNORE: cleanobj cleanall
.PHONY:  cleanobj cleanall