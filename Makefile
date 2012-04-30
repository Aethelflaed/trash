srcdir := src
lib_srcdir := $(srcdir)/lib
put_srcdir := $(srcdir)/put
restore_srcdir := $(srcdir)/restore
list_srcdir := $(srcdir)/list

objdir := obj
lib_objdir += $(objdir)/lib
put_objdir += $(objdir)/put
restore_objdir += $(objdir)/restore
list_objdir += $(objdir)/list

bindir := bin
libdir := lib
incdir := include

include Makefile.conf

lib_srcsdir := $(lib_srcdir)
lib_srcsdir += $(addprefix $(lib_srcdir)/, $(lib_modules))
put_srcsdir := $(put_srcdir)
put_srcsdir += $(addprefix $(put_srcdir)/, $(put_modules))
restore_srcsdir := $(restore_srcdir)
restore_srcsdir += $(addprefix $(restore_srcdir)/, $(restore_modules))
list_srcsdir := $(list_srcdir)
list_srcsdir += $(addprefix $(list_srcdir)/, $(list_modules))

lib_objsdir := $(lib_objdir)
lib_objsdir += $(addprefix $(lib_objdir)/, $(lib_modules))
put_objsdir := $(put_objdir)
put_objsdir += $(addprefix $(put_objdir)/, $(put_modules))
restore_objsdir := $(restore_objdir)
restore_objsdir += $(addprefix $(restore_objdir)/, $(restore_modules))
list_objsdir := $(list_objdir)
list_objsdir += $(addprefix $(list_objdir)/, $(list_modules))

lib_srcs := $(foreach sdir, $(lib_srcsdir), $(wildcard $(sdir)/*.cpp))
put_srcs := $(foreach sdir, $(put_srcsdir), $(wildcard $(sdir)/*.cpp))
restore_srcs := $(foreach sdir, $(restore_srcsdir), $(wildcard $(sdir)/*.cpp))
list_srcs := $(foreach sdir, $(list_srcsdir), $(wildcard $(sdir)/*.cpp))

lib_objs := $(patsubst $(lib_srcdir)/%.cpp, $(lib_objdir)/%.o, $(foreach sdir, $(lib_srcsdir), $(wildcard $(sdir)/*.cpp)))
put_objs := $(patsubst $(put_srcdir)/%.cpp, $(put_objdir)/%.o, $(foreach sdir, $(put_srcsdir), $(wildcard $(sdir)/*.cpp)))
restore_objs := $(patsubst $(restore_srcdir)/%.cpp, $(restore_objdir)/%.o, $(foreach sdir, $(restore_srcsdir), $(wildcard $(sdir)/*.cpp)))
list_objs := $(patsubst $(list_srcdir)/%.cpp, $(list_objdir)/%.o, $(foreach sdir, $(list_srcsdir), $(wildcard $(sdir)/*.cpp)))

localdirs :=\
	$(objsdir)\
	$(lib_objsdir)\
	$(put_objsdir)\
	$(restore_objsdir)\
	$(list_objsdir)\
	$(bindir)\
	$(libdir)\
	$(incdir)

vpath %.cpp $(lib_srcsdir)
vpath %.cpp $(put_srcsdir)
vpath %.cpp $(restore_srcsdir)
vpath %.cpp $(list_srcsdir)

define make-lib-objs-goal
$1/%.o: %.cpp
	$(CPP) -c $$< -o $$@ $(LIBCPPFLAGS) $(LIB_INCLUDES)
endef
define make-app-objs-goal
$1/%.o: %.cpp
	$(CPP) -c $$< -o $$@ $(CPPFLAGS) $(APP_INCLUDES)
endef

target_lib := $(libdir)/lib$(lib_name).so
target_put := $(bindir)/$(put_name)
target_restore := $(bindir)/$(restore_name)
target_list := $(bindir)/$(list_name)

apps :=\
	$(target_put)\
	$(target_restore)\
	$(target_list)

libs :=\
	$(target_lib)

all: $(localdirs) $(libs) $(apps)

clean:
	$(RM) -r $(objdir)
	$(RM) -r $(bindir)
	$(RM) -r $(libdir)

$(target_lib): $(lib_objs)
	$(LD) $(LIBLDFLAGS) $(LIB_LIBS) $^ -o $@

$(target_put): $(put_objs)
	$(LD) $(LDFLAGS) $(APP_LIBS) $^ -o $@

$(target_restore): $(restore_objs)
	$(LD) $(LDFLAGS) $(APP_LIBS) $^ -o $@

$(target_list): $(list_objs)
	$(LD) $(LDFLAGS) $(APP_LIBS) $^ -o $@

$(localdirs):
	mkdir -p $@

$(foreach odir, $(lib_objsdir), $(eval $(call make-lib-objs-goal, $(odir))))
$(foreach odir, $(put_objsdir), $(eval $(call make-app-objs-goal, $(odir))))
$(foreach odir, $(restore_objsdir), $(eval $(call make-app-objs-goal, $(odir))))
$(foreach odir, $(list_objsdir), $(eval $(call make-app-objs-goal, $(odir))))

