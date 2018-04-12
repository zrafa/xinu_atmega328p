##############################################################################
#                                                                            #
#  Makefile for the Embedded Xinu operating system.                          #
#                                                                            #
#  Please see README.compiling.                                              #
#                                                                            #
##############################################################################

APP := ej02
#
# Name of the platform to build Embedded Xinu for.  It must be the name of a
# directory in compile/platforms/.  The value below specifies the default
# platform for this Embedded Xinu distribution, but you can override this on the
# command line without modifying this Makefile; e.g. with
#
# $ make PLATFORM=arm-rpi
#
PLATFORM := atmega328p

# Filename of boot image to create (platform can override it if really needed)
BOOTIMAGE := $(APP)

# Top-level Embedded Xinu directory
TOPDIR  := ../../../..

# Set the C compilation flags common to all Embedded Xinu platforms.
# platformVars can add more flags if desired.
CFLAGS  := -fno-stack-protector 

# Do not perform linking until the end.
CFLAGS  += -c

# Optimize for size.
CFLAGS  += -Os

# Enable most useful compiler warnings.
CFLAGS  += -Wall

# Require full prototypes for all functions.
CFLAGS  += -Wstrict-prototypes

# Do not search standard system directories such as "/usr/include" for header
# files, since Embedded Xinu is freestanding and only uses its own headers.
CFLAGS  += -nostdinc

# Do not allow gcc to replace function calls with calls to gcc builtins, except
# when explicitly requested through a __builtin prefix.  This ensures that gcc
# does not attempt to replace any of our code with its own.
CFLAGS  += -fno-builtin

# Assume that the memory locations pointed to by any two pointers can alias,
# even if the types of the variables pointed to are not compatible as defined in
# the C standard.  Enabling this option is fairly common, since most programmers
# don't fully understand aliasing in C, and this forces the "expected" behavior.
CFLAGS  += -fno-strict-aliasing

# Do not allow multiple definitions of uninitialized global variables.
CFLAGS  += -fno-common

# Place each function in a separate section so that the linker can apply garbage
# collection to remove unused functions (the --gc-sections linker flag).
CFLAGS  += -ffunction-sections

# Do not generate position-independent code.  (This flag may be unneeded, since
# generally you have to specify -fPIC to *get* the compiler to generate
# position-independent code).
CFLAGS  += -fno-pic

# Treat signed overflow as fully defined as per two's complement arithmetic,
# even though the C standard specifies that signed overflow is undefined
# behavior.  Many programmers are not aware of this, so we force the expected
# behavior.
CFLAGS  += -fwrapv

# DEBUG is used internally by `make debug' to add debugging flags.
DEBUG   := 
CFLAGS  += $(DEBUG)

# Set assembler flags common to all platforms.  platformVars can add additional
# flags if needed.  Do not use the "-Wa," prefix, either here or in
# platformVars.
#
# Note: this -I option is not used for C headers; instead it's used in case the
# platform wants to include other files directly from assembly sources.
ASFLAGS := -I$(TOPDIR)/xinu/system/platforms/$(PLATFORM)

# Set linker flags common to all platforms.  platformVars can add additional
# flags if needed.  Do not use the "-Wl," prefix either here on in platformVars.
# LDFLAGS := --static --gc-sections
LDFLAGS := --static

# Set default external libraries.  Embedded Xinu is, of course, stand-alone and
# ordinarily does not need to be linked to any external libraries; however,
# platformVars can add -lgcc to this if needed by the platform.
LDLIBS  :=

# Set default include directories.  platformVars can add additional directories
# to this if really needed.
INCLUDE := -I$(TOPDIR)/xinu/include -I$(TOPDIR)/xinu/system/platforms/$(PLATFORM) -I$(TOPDIR)/xinu

# Override this as -DDETAIL to enable certain debugging messages in Embedded
# Xinu.
DETAIL  :=

# Set default additional defines.  platformVars can add extra defines if needed.
DEFS    := $(DETAIL)

# Set default libraries to build into Xinu
#
# Each library LIB is expected to be built from a directory $(LIBDIR)/$(LIB) and
# generate an archive $(LIBDIR)/$(LIB).a.  Unlike other Embedded Xinu
# directories, each library is expected to have an actual 'Makefile' and not
# just 'Makerules'.  However, this 'Makefile' will be executed with variables
# from this one in its environment.
#
# platformVars can add additional libraries to $(LIBS); however the C library
# (libxc) is always included by default.
LIBS    := libxc

###############################################################################

# Include the platform-specific Makefile.
include $(TOPDIR)/xinu/system/platforms/$(PLATFORM)/platformVars

###############################################################################

# Finalize ASFLAGS and CFLAGS.
ASFLAG_PREFIX := 
ASFLAGS       := $(addprefix $(ASFLAG_PREFIX),$(ASFLAGS))
ASFLAGS       += $(INCLUDE) $(DEFS) -c
CFLAGS        += $(INCLUDE) $(DEFS)

# Set linker script.  A script in platforms/ overrides the one in arch/, if
# present.
ifneq ($(TEMPLATE_ARCH),)
  ifeq ($(shell if [ -e $(TOPDIR)/xinu/system/arch/$(TEMPLATE_ARCH)/ld.script ]; then echo 1; fi),1)
    LDSCRIPT := $(TOPDIR)/xinu/system/arch/$(TEMPLATE_ARCH)/ld.script
  endif
endif

ifeq ($(shell if [ -e $(TOPDIR)/xinu/system/platforms/$(PLATFORM)/ld.script ]; then echo 1; fi),1)
  LDSCRIPT := $(TOPDIR)/xinu/system/platforms/$(PLATFORM)/ld.script
endif

ifeq ($(LDSCRIPT),)
  $(warning ld.script not found)
else
  LDFLAGS += -T$(LDSCRIPT)
endif

# Set the location of the architecture-specific cross-compiler and binutils.
# This can be overridden on the command line; e.g.
#
# make PLATFORM=arm-rpi COMPILER_ROOT=/opt/arm-dev/bin/arm-none-eabi-
#
COMPILER_ROOT := $(ARCH_ROOT)$(ARCH_PREFIX)

# Set the actual compiler and several binutils programs.
CC       := $(COMPILER_ROOT)gcc
AR       := $(COMPILER_ROOT)ar
LD       := $(COMPILER_ROOT)ld
STRIP    := $(COMPILER_ROOT)strip
OBJCOPY  := $(COMPILER_ROOT)objcopy

# Sanity check: does 'gcc' actually exist?
ifneq ($(shell if $(CC) --version > /dev/null 2>&1; then echo 1; fi),1)
  $(warning $(CC) not found; try install both gcc and binutils for target platform; also try overriding COMPILER_ROOT \
	    (see README.compiling) to fix the problem.)
endif

# Did platformVars add -lgcc to LDLIBS?  If so, we need to do the linking with
# gcc rather than with ld so that the appropriate libgcc can be found.  However,
# this also requires adjusting LDFLAGS to ensure they are passed directly to the
# linker and adding -nostdlib to prevent gcc from linking in the C runtime
# startup stub.
ifneq ($(filter -lgcc,$(LDLIBS)),)
  KERNEL_LD      := $(CC)
  LDFLAGS_PREFIX := -Wl,
  LDFLAGS        := $(addprefix $(LDFLAGS_PREFIX), $(LDFLAGS))
  LDFLAGS        += -nostdlib
else
  KERNEL_LD     := $(LD)
endif

KERNARC  := kernel.a
MAIN_SRC := $(APP).c

LIBDIR   := $(TOPDIR)/xinu/lib

# With GNU make, if you define the magic target .SILENT, `make' will not echo
# recipes as they are executed.  We use this mode by default and allow the
# normal mode to be switched on by overriding VERBOSE to any value.
ifeq ($(VERBOSE),)
.SILENT:
endif

###########################
# Source file information #
###########################

# Major system components to build into Xinu
# Each component is a directory in the source tree and should include a
# Makerules specifying what files to include
# (The loader should always be first, ensuring the image starts with it.)
SYSCOMPS := xinu/loader/platforms/$(PLATFORM) \
            xinu/system/platforms/$(PLATFORM) \
            xinu/system

# APPCOMPS, the list of application components to build into Xinu, should also
# have been set in platformVars.

# List of all Xinu components to include
COMPS    := $(SYSCOMPS)

# Include expected files, each should add its part to the compile source
COMP_SRC :=
include $(COMPS:%=$(TOPDIR)/%/Makerules)

COMP_OBJ := $(patsubst %.S,%.o,$(filter %.S,$(COMP_SRC))) \
            $(patsubst %.c,%.o,$(filter %.c,$(COMP_SRC)))

LIB_ARC  := $(LIBS:%=$(LIBDIR)/%.a)

MAIN_OBJ := $(MAIN_SRC:%.c=%.o)

# Flags to get GCC to generate dependency information (in .d files) as a side
# effect of compilation or assembly.
DEPFLAGS := -MMD -MP

# Export variables for recursive make calls (such as when building libraries)
export
################
# Make targets #
################

# Note: the default target is actually $(BOOTIMAGE) and is defined in
# "platformVars".  But it will depend on "xinu.elf".

$(APP).elf: $(COMP_OBJ) $(MAIN_OBJ) $(LIB_ARC)
	@echo -e "\tLinking" $@
	$(KERNEL_LD) -o $@ $(LDFLAGS) $^ $(LDLIBS) -Map=$(APP).map
	@echo -e "\tObject Copy" $@
	@echo $(OCFLAGS)
	$(OBJCOPY) $(OCFLAGS) $(APP).elf $(APP).hex


$(COMP_OBJ): 

$(KERNARC): $(COMP_OBJ)
	rm -f $(KERNARC)
	$(AR) cr $(KERNARC) $^

objects: $(COMP_OBJ)

%.o: %.c
	@echo -e "\tCompiling" $@
	$(CC) $(DEPFLAGS) $(CFLAGS) -o $@ $<

%.o: %.S
	@echo -e "\tAssembling" $@
	$(CC) $(ASFLAGS) -o $@ $<

clean:
	@echo -e "\tCleaning all objects"
	rm -f *.o $(COMP_OBJ) $(MAIN_OBJ)
	rm -f $(DEPFILES)
	rm -f $(APP) $(APP).hex $(APP).elf $(APP).map $(APP).d 

debug:
	@echo -e "\tBuilding debug mode"
	$(MAKE) DEBUG="-DDEBUG $(BUGFLAG)"

help:
	$(PAGER) README.compiling

libclean: $(LIBS:%=%-clean)


realclean: clean libclean $(PLATCLEAN)
	@echo -e "\tCleaning EVERYTHING"
	rm -f Makefile.bak

###################
# Special targets #
###################
$(MAIN_OBJ): $(MAIN_SRC)

$(COMPS):
	@echo -e "\tBuilding" $@
	$(MAKE) COMPS="$@" objects

$(LIBS): 
	@echo -e "\tBuilding" $@
	$(MAKE) -C $(LIBDIR)/$@

$(LIB_ARC): 
	@echo -e "\tInstalling" $@
	$(MAKE) -C $(@:%.a=%) install

$(LIBS:%=%-clean):
	@echo -e "\tCleaning" $@
	$(MAKE) -C $(LIBDIR)/$(@:%-clean=%) clean

# Include generated dependency information for C and assembly files, if it
# exists.
DEPFILES := $(COMP_SRC:%.c=%.d)
DEPFILES := $(DEPFILES:%.S=%.d)

-include $(DEPFILES)

# Ensure the default `make' target is set correctly--- it's supposed to be
# $(BOOTIMAGE) and defined in "platformVars".
.DEFAULT_GOAL := $(BOOTIMAGE)
