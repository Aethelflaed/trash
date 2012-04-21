include Makefile.conf

srcdir := src
bindir := bin
libdir := lib
objdir := build
incdir := include

srcdir += $(addprefix $(srcdir)/, $(modules))
objdir += $(addprefix $(objdir)/, $(modules))

srcs := $(foreach sdir, $(srcdir), $(wildcard $(sdir)/*.cpp))
objs := $(patsubst $(srcdir)/%.cpp, $(objdir)/%.o, $(foreach odir, $(objdir), $(wildcard $(odir)/*.cpp)))

localdirs :=\
	$(objdir)\
	$(bindir)\
	$(libdir)\
	$(incdir)

vpath %.cpp $(srcdir)

define make-objs-goal:
$1/%.o: %.cpp
	$(CPP) -c $$< -o $$@ $(CPPFLAGS) $(INCLUDES)
endef

program=$(build_bindir)/$(program_name)

all: $(localdirs) $(program)

clean:
	$(RM) build
	$(RM) bin

$(program): $(objs)
	$(LD) $(LDFLAGS) $(LIBS) $^ -o $@
	

$(localdirs):
	mkdir -p $@

$(foreach odir, $(objdir), $(eval $(call make-objs-goal, $(odir))))
