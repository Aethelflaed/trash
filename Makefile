include Makefile.conf

srcdir := src
bindir := bin
libdir := lib
objdir := obj
incdir := include

srcsdir := $(srcdir)
srcsdir += $(addprefix $(srcdir)/, $(modules))
objsdir := $(objdir)
objsdir += $(addprefix $(objdir)/, $(modules))

srcs := $(foreach sdir, $(srcsdir), $(wildcard $(sdir)/*.cpp))
objs := $(patsubst $(srcdir)/%.cpp, $(objdir)/%.o, $(foreach sdir, $(srcsdir), $(wildcard $(sdir)/*.cpp)))

localdirs :=\
	$(objdir)\
	$(bindir)\
	$(libdir)\
	$(incdir)

vpath %.cpp $(srcsdir)

define make-objs-goal
$1/%.o: %.cpp
	$(CPP) -c $$< -o $$@ $(CPPFLAGS) $(INCLUDES)
endef

program=$(bindir)/$(program_name)

all: debug $(localdirs) $(program)

debug:

clean:
	$(RM) -r $(objdir)
	$(RM) -r $(bindir)

$(program): $(objs)
	$(LD) $(LDFLAGS) $(LIBS) $^ -o $@
	

$(localdirs):
	mkdir -p $@

$(foreach odir, $(objsdir), $(eval $(call make-objs-goal, $(odir))))

