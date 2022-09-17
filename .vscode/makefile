#source file search paths
VPATH = ./src 

	# compiler include search paths
INCS = -I./include \
	-I../ravencode/pub/src \
	-I../raven-set \
	-I../boost/boost1_78 

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \
	-L../boost/boost1_78/lib \
		-lboost_system-mgw11-mt-x64-1_78 \
		-lboost_program_options-mgw11-mt-x64-1_78 \
		# -lboost_thread-mgw11-mt-x64-1_7 \
	#-mwindows

static_link_flags = -static-libstdc++ -static-libgcc -static

# folders
ODIR = ./.vscode/obj
RDIR = ../ravencode/pub/src

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_hhct = \
	$(ODIR)/hhct.o \
	$(ODIR)/cEntity.o \

OBJ_graph = \
	$(ODIR)/graph.o \
	$(ODIR)/cEntity.o \

OBJ_ron = \
	$(ODIR)/ronnie.o \
	$(ODIR)/cEntity.o \

### COMPILE

$(ODIR)/%.o: %.cpp cEntity.h
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

### LINK

hhtc: $(OBJ_hhct)
	g++ $(OPT) \
	-o bin/hhtc.exe $^  \
	$(static_link_flags) $(LIBS)

graph: $(OBJ_graph)
	g++ $(OPT) \
	-o bin/graph.exe $^  \
	$(static_link_flags) $(LIBS)

ronnie: $(OBJ_ron)
	g++ $(OPT) \
	-o bin/ronnie.exe $^  \
	$(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

