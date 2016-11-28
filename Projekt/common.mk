# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#===== USEFILE - the file containing the usage message for the application. 
USEFILE=

# Next lines are for C++ projects only

EXTRA_SUFFIXES+=cxx cpp

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+= \
	$(PROJECT_ROOT)/src  \
	$(PROJECT_ROOT)/src/lib  \
	$(PROJECT_ROOT)/Logger  \
	$(PROJECT_ROOT)/stateMachine  \
	$(PROJECT_ROOT)/ConfigManagement  \
	$(PROJECT_ROOT)/Hal  \
	$(PROJECT_ROOT)/Hal/ISR  \
	$(PROJECT_ROOT)/Hal/SerialInterface  \
	$(PROJECT_ROOT)/Entity  \
	$(PROJECT_ROOT)/Thread  \
	$(PROJECT_ROOT)/Controller  \
	$(PROJECT_ROOT)/Serializer  \
	$(PROJECT_ROOT)/FSM  \
	$(QNX_TARGET)/usr/include

#===== LDFLAGS - add the flags to the linker command line.
LDFLAGS+=-lang-c++ -Y _gpp

#===== CCFLAGS - add the flags to the C compiler command line. 
CCFLAGS+=-Y _gpp


include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

