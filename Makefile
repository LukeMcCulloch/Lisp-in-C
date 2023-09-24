##########################################################################################

# Specify library locations here (add or remove "#" marks to comment/uncomment lines for your platform)

# Mac OS X
# BNCpp_INCLUDE_PATH      =
# BNCpp_LIBRARY_PATH      =
# BNCpp_BLAS_LIBS         = -framework Accelerate
# BNCpp_SUITESPARSE_LIBS  = -lspqr -lumfpack -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -ltbb -lm -lsuitesparseconfig
# BNCpp_OPENGL_LIBS       = -framework OpenGL -framework GLUT

# # Linux
INCLUDE_PATH      =
LIBRARY_PATH      =
BLAS_LIBS         = -llapack -lblas -lgfortran
# BNCpp_SUITESPARSE_LIBS  = -lspqr -lcholmod -lmetis -lcolamd -lccolamd -lcamd -lamd -lm
# BNCpp_OPENGL_LIBS       = -lglut -lGL -lGLU -lX11
SUITESPARSE_LIBS  = -lspqr -lcholmod -lcolamd -lccolamd -lcamd -lamd -lm -lumfpack -lamd #-lmetis 
OPENGL_LIBS       = -lGL -lGLU -lglut -lGLEW #-lX11

# # Windows / Cygwin
# BNCpp_INCLUDE_PATH      = -I/usr/include/opengl -I/usr/include/suitesparse
# BNCpp_LIBRARY_PATH      = -L/usr/lib/w32api -L/usr/lib/suitesparse
# BNCpp_BLAS_LIBS         = -llapack -lblas
# BNCpp_SUITESPARSE_LIBS  = -lspqr -lcholmod -lcolamd -lccolamd -lcamd -lamd -lm
# BNCpp_OPENGL_LIBS       = -lglut32 -lglu32 -lopengl32

########################################################################################


#OBJDIR := obj
SRCDIR := src
DEPDIR := src
INCLUDEDIR := include
BUILDDIR := run

TARGET = run/CLispy
CC = g++
NXX = nvcc
CFLAGS =  -Wall -Wextra -Werror -MD -I./include -I./src #adding these includes is essential!# -Wall  -Werror -ansi -pedantic  $(BNCpp_INCLUDE_PATH) -I./include -I./src
LFLAGS = #--ansi -pedantic -fopenmp -O1 # -Wall  -Werror -ansi -pedantic $(BNCpp_LIBRARY_PATH)
NVFLAGS = # -g -G  -O0  
LIBS =  $(OPENGL_LIBS) $(SUITESPARSE_LIBS) $(BLAS_LIBS)
CUDAFLAGS = #-arch=sm_75 #use > nvidia-smi 
#                        to find your Card architecture 
#                        and CUDA verison quickly
# see also: https://stackoverflow.com/questions/35656294/cuda-how-to-use-arch-and-code-and-sm-vs-compute
#
# see: https://arnon.dk/matching-sm-architectures-arch-and-gencode-for-various-nvidia-cards/
# to find your cudaflags = -arch=sm_X version 
#
########################################################################################
## Feel free to edit below this line, if you know make.



all: $(TARGET)
# rule for the target comes first

# $<  The input file
# $@  The file that is being made right now 
# $^  The first prerequisite
# $^ copy dependencies from line above


$(TARGET): $(OBJECTS)
	@echo "target = " $(TARGET)
	@echo "headers = " $(HEADERS)
	@echo "sources = " $(SOURCES)
	@echo "objects = " $(OBJECTS)
#	$(CC) $(SOURCES) -MMD -o $@ $^ $(TARGET) $(CUDAFLAGS) $(NVFLAGS) $(CFLAGS)
	$(CC) $(SOURCES) -MMD -o $@ $^ $(TARGET) $(CUDAFLAGS) $(NVFLAGS) $(CFLAGS)

#$(OBJDIR)/%.o: src/%.cpp  ${HEADERS} $(DEPDIR)/%.d | $(DEPDIR)
#	$(CC) -c $< -o $@ $(CUDAFLAGS) $(NVFLAGS) $(CFLAGS)




HEADERS := $(wildcard include/*.h include/*.hpp include/*.cuh)
# SOURCES := $(wildcard src/*.cpp src/*.c src/*.cu)
SOURCES := $(wildcard src/*.cpp src/*.cu)
#SOURCES := $(shell find src -name "*.cpp")
OBJECTS := $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o))  $(notdir $(SOURCES:.cu=.o)))
#
# $(notdir names…): Extracts all but the directory-part of each 
#                   file name in names
# $(addprefix prefix,names…): The argument names is regarded as 
#                             a series of names, separated by whitespace; 
#                             prefix is used as a unit. The value of prefix is 
#                             prepended to the front of each individual name 
#                             and the resulting larger names are concatenated 
#                             with single spaces between them. 
#



#DEPS := $(SOURCES:.cpp=.d)
#.c.d:
#	$(NXX) -o $< -MM 
#-include $(DEPS)

# DEPS := $(SOURCES:.cpp=.d)
# .c.d:
# 	$(CC) -o $< -MM
# -include $(DEPS)

# DEPS := $(SOURCES:.o=.d)

# -include $(DEPS)

# %.o: %.c
#    $(CC) -c $(CFLAGS) -MM -MF $(patsubst %.o,%.d,$@) -o $@ $<


# phony improves performance here:
# otherwise make clean will clean even when unecessary
# and if a file called clean existed, it's commands would be executed
.PHONY: clean


# .PRECIOUS: $(OBJDIR)/%.o 



#$(OBJDIR)/%.o: src/%.cpp src/%.cu ${HEADERS}
#	$(NXX) -c $< -o $@ $(CUDAFLAGS) $(NVFLAGS) $(CFLAGS)

#$(OBJDIR)/%.o: ${SOURCES} ${HEADERS} $(DEPDIR)/%.d | $(DEPDIR)
#	$(CC) -c $< -o $@ $(CUDAFLAGS) $(NVFLAGS) $(CFLAGS) 

#$(OBJDIR)/%.o: src/%.cpp  ${HEADERS} $(DEPDIR)/%.d | $(DEPDIR)
#	$(CC) -c $< -o $@ $(CUDAFLAGS) $(NVFLAGS) $(CFLAGS)


# clean:
# 	@echo "cleaning object files"
# 	rm -f $(OBJECTS)


clean:
	@echo "cleaning object and executable files"
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f $(TARGET).exe



# DEPDIR := .deps
# DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# #COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

# %.o : %.c
# %.o : %.c $(DEPDIR)/%.d | $(DEPDIR)
# 		$(TARGET) $(OUTPUT_OPTION) $<

# $(DEPDIR): ; @mkdir -p $@

# DEPFILES := $(SRCS:%.c=$(DEPDIR)/%.d)
# $(DEPFILES):

# include $(wildcard $(DEPFILES))

# # pull in dependency info for *existing* .o files
# -include $(OBJS:.o=.d)

#
#DO NOT DELETE THIS LINE
#