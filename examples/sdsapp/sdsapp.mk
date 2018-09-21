NAME := sdsapp

# at_support_alisds := 1

$(NAME)_SOURCES := sdsapp.c

$(NAME)_COMPONENTS := yloop cli mxathost

GLOBAL_DEFINES += AOS_NO_WIFI MX_DEBUG

ifeq ($(at_support_alisds),1)
GLOBAL_DEFINES += AT_SUPPORT_ALISDS
endif
