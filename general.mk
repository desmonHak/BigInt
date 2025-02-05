include config.mk

all: generate_lib
	gcc $(CFLAGS) $(INCLUDE_FLAGS) $(PATH_EXAMPLES)/example.c $(LINKER_FLAGS) -o example.$(EXTENSION)

generate_lib: libdebug_c.a $(TARGET).a
	ar -t $(TARGET).a

debug: generate_lib
	ar -t $(TARGET).a
	gcc $(CFLAGS_DEBUG) $(INCLUDE_FLAGS) $(PATH_EXAMPLES)/example.c $(LINKER_FLAGS) -o example.$(EXTENSION)

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

BigInt.o: $(PATH_SRC)/BigInt.c
	$(CC) $(CFLAGS) -c $^ -o $@


cleanobj:
	$(RM) $(RMFLAGS) $(OBJECTS)
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME) cleanobj

cleanall: cleanobj
	$(RM) $(RMFLAGS) $(TARGET).o $(TARGET).a \
	$(TARGET_DEBUG).a
	$(RM) $(RMFLAGS) example.$(EXTENSION)
	$(MAKE) -C ./$(PATH_DEBUG) -f $(MAKE_NAME) cleanall

.SILENT: clean cleanobj cleanall
.IGNORE: cleanobj cleanall
.PHONY:  cleanobj cleanall