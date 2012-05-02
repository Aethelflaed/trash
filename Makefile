srcdir := src
lib_srcdir := $(srcdir)/lib
put_srcdir := $(srcdir)/put
restore_srcdir := $(srcdir)/restore
list_srcdir := $(srcdir)/list

objdir := obj
lib_objdir += $(objdir)/lib
shared_lib_objdir += $(lib_objdir)/shared
static_lib_objdir += $(lib_objdir)/static
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

shared_lib_objsdir := $(shared_lib_objdir)
shared_lib_objsdir += $(addprefix $(shared_lib_objdir)/, $(shared_lib_modules))
static_lib_objsdir := $(static_lib_objdir)
static_lib_objsdir += $(addprefix $(static_lib_objdir)/, $(static_lib_modules))
put_objsdir := $(put_objdir)
put_objsdir += $(addprefix $(put_objdir)/, $(put_modules))
restore_objsdir := $(restore_objdir)
restore_objsdir += $(addprefix $(restore_objdir)/, $(restore_modules))
list_objsdir := $(list_objdir)
list_objsdir += $(addprefix $(list_objdir)/, $(list_modules))

lib_srcs := $(foreach sdir, $(lib_srcsdir), $(wildcard $(sdir)/*.cpp))
lib_headers := $(patsubst $(lib_srcdir)/%.hpp, $(incdir)/%.hpp, $(foreach sdir, $(lib_srcsdir), $(wildcard $(sdir)/*.hpp)))
put_srcs := $(foreach sdir, $(put_srcsdir), $(wildcard $(sdir)/*.cpp))
restore_srcs := $(foreach sdir, $(restore_srcsdir), $(wildcard $(sdir)/*.cpp))
list_srcs := $(foreach sdir, $(list_srcsdir), $(wildcard $(sdir)/*.cpp))

shared_lib_objs := $(patsubst $(lib_srcdir)/%.cpp, $(shared_lib_objdir)/%.o, $(foreach sdir, $(lib_srcsdir), $(wildcard $(sdir)/*.cpp)))
static_lib_objs := $(patsubst $(lib_srcdir)/%.cpp, $(static_lib_objdir)/%.o, $(foreach sdir, $(lib_srcsdir), $(wildcard $(sdir)/*.cpp)))
put_objs := $(patsubst $(put_srcdir)/%.cpp, $(put_objdir)/%.o, $(foreach sdir, $(put_srcsdir), $(wildcard $(sdir)/*.cpp)))
restore_objs := $(patsubst $(restore_srcdir)/%.cpp, $(restore_objdir)/%.o, $(foreach sdir, $(restore_srcsdir), $(wildcard $(sdir)/*.cpp)))
list_objs := $(patsubst $(list_srcdir)/%.cpp, $(list_objdir)/%.o, $(foreach sdir, $(list_srcsdir), $(wildcard $(sdir)/*.cpp)))

localdirs :=\
	$(objsdir)\
	$(shared_lib_objsdir)\
	$(static_lib_objsdir)\
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

define make-build-lib-shared-obj-goal
$2: $1
	$(CPP) -c $$^ -o $$@ $(LIBCPPFLAGS) $(LIB_INCLUDES)
endef
define make-build-lib-static-obj-goal
$2: $1
	$(CPP) -c $$^ -o $$@ $(CPPFLAGS) $(LIB_INCLUDES)
endef
define make-build-app-obj-goal
$2: $1
	$(CPP) -c $$^ -o $$@ $(CPPFLAGS) $(APP_INCLUDES)
endef
define make-cp-lib-headers-goal
$2: $1
	cp $$^ $$@
endef

target_shared_lib := $(libdir)/lib$(lib_name).so
target_static_lib := $(libdir)/lib$(lib_name).a
target_put := $(bindir)/$(put_name)
target_restore := $(bindir)/$(restore_name)
target_list := $(bindir)/$(list_name)

apps :=\
	$(target_put)\
	$(target_restore)\
	$(target_list)

libs :=\
	$(target_shared_lib)\
	$(target_static_lib)

all: $(localdirs) $(libs) $(lib_headers) $(apps)

clean:
	$(RM) -r $(objdir)/*
	$(RM) -r $(bindir)/*
	$(RM) -r $(libdir)/*
	$(RM) -r $(incdir)/*

$(target_shared_lib): $(shared_lib_objs)
	$(LD) $(LIBLDFLAGS) $(LIB_LIBS) $^ -o $@

$(target_static_lib): $(static_lib_objs)
	ar rv $@ $^
	ranlib $@

$(target_put): $(put_objs)
	$(LD) $(LDFLAGS) $(APP_LIBS) $^ -o $@

$(target_restore): $(restore_objs)
	$(LD) $(LDFLAGS) $(APP_LIBS) $^ -o $@

$(target_list): $(list_objs)
	$(LD) $(LDFLAGS) $(APP_LIBS) $^ -o $@

$(localdirs):
	mkdir -p $@

$(foreach obj, $(shared_lib_objs), $(eval $(call make-build-lib-shared-obj-goal, $(patsubst $(shared_lib_objdir)/%.o, $(lib_srcdir)/%.cpp, $(obj)), $(obj))))
$(foreach obj, $(static_lib_objs), $(eval $(call make-build-lib-static-obj-goal, $(patsubst $(static_lib_objdir)/%.o, $(lib_srcdir)/%.cpp, $(obj)), $(obj))))
$(foreach obj, $(put_objs), $(eval $(call make-build-app-obj-goal, $(patsubst $(put_objdir)/%.o, $(put_srcdir)/%.cpp, $(obj)), $(obj))))
$(foreach obj, $(restore_objs), $(eval $(call make-build-app-obj-goal, $(patsubst $(restore_objdir)/%.o, $(restore_srcdir)/%.cpp, $(obj)), $(obj))))
$(foreach obj, $(list_objs), $(eval $(call make-build-app-obj-goal, $(patsubst $(list_objdir)/%.o, $(list_srcdir)/%.cpp, $(obj)), $(obj))))
$(foreach header, $(lib_headers), $(eval $(call make-cp-lib-headers-goal, $(patsubst $(incdir)/%.hpp, $(lib_srcdir)/%.hpp, $(header)), $(header))))

