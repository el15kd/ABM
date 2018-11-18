
include ./env

.PHONY: clean_compiled_files
clean_compiled_files:
	rm -f *.exe
	rm -f ./bin/*.exe
	rm -f *.o
	rm -f ./object/*.o

.PHONY: compile
compile: clean_compiled_files
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Demo_02_Main.cpp -o ./objects/Demo_02_Main.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Demo_02_Model.cpp -o ./objects/Demo_02_Model.o
	$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE) -I./include -c ./src/Demo_02_Agent.cpp -o ./objects/Demo_02_Agent.o
	$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR) -o ./bin/Demo_02.exe  ./objects/Demo_02_Main.o ./objects/Demo_02_Model.o ./objects/Demo_02_Agent.o $(REPAST_HPC_LIB) $(BOOST_LIBS)


