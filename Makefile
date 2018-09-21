######################################
# target
######################################
.PHONY:test ilopicaapp iloprawapp sdsapp

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
			platform/linux/mx_serial.c \
			platform/linux/mx_stdio.c \
			platform/linux/mx_tick.c \
			platform/linux/mx_uart.c \
			src/helper/cli/mx_cli.c \
			src/helper/emhost/ATCmdParser/ATCmdParser.c \
			src/helper/emhost/emh_alisds.c \
			src/helper/emhost/emh_arg.c \
			src/helper/emhost/emh_ilop.c \
			src/helper/emhost/emh_qlink.c \
			src/helper/emhost/emh_module.c \
			src/helper/emhost/emh_wlan.c \
			src/helper/mx_utils/mx_ringbuffer.c


# ASM sources
ASM_SOURCES =


######################################
# firmware library
######################################
PERIFLIB_SOURCES = 


#######################################
# binaries
#######################################
BINPATH = 
PREFIX = 
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU =

# fpu
FPU =

# float-abi
FLOAT-ABI =

# mcu
MCU =

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS = -DMX_DEBUG -DMX_CLI_ENABLE

# AS includes
AS_INCLUDES = 


# C includes
C_INCLUDES =  \
-Iplatform \
-Isrc/helper/cli \
-Isrc/helper/emhost/ATCmdParser \
-Isrc/helper/emhost \
-Isrc/helper/mx_utils \
-Isrc/helper


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD


#######################################
# LDFLAGS
#######################################

# libraries
LIBS =
LIBDIR =
LDFLAGS = -lpthread -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# TARGET
#######################################
ifeq (test,$(findstring test,$(MAKECMDGOALS)))
TARGET = iot-test
C_SOURCES += examples/test/test.c
endif

ifeq (ilopicaapp,$(findstring ilopicaapp,$(MAKECMDGOALS)))
TARGET = iot-ilopicaapp
C_SOURCES += examples/ilopicaapp/app_event.c \
			 examples/ilopicaapp/app_prop.c \
			 examples/ilopicaapp/app_service.c \
			 examples/ilopicaapp/ilopmain.c \
			 examples/ilopicaapp/main.c \
			 src/alicloud_ilop/alicloud_ilop.c \
			 src/alicloud_ilop/ica_protocol.c

C_INCLUDES += -Iexamples/ilopicaapp -Isrc/alicloud_ilop

CFLAGS += -DAT_SUPPORT_ILOP -DILOP_USE_ICA
endif

ifeq (iloprawapp,$(findstring iloprawapp,$(MAKECMDGOALS)))
TARGET = iot-iloprawapp
C_SOURCES += examples/iloprawapp/app_event.c \
			 examples/iloprawapp/app_prop_light.c \
			 examples/iloprawapp/app_prop_wifi.c \
			 examples/iloprawapp/app_service.c \
			 examples/iloprawapp/ilopmain.c \
			 examples/iloprawapp/main.c \
			 src/alicloud_ilop/alicloud_ilop.c \
			 src/alicloud_ilop/raw_protocol.c

C_INCLUDES += -Iexamples/iloprawapp -Isrc/alicloud_ilop

CFLAGS += -DAT_SUPPORT_ILOP -DILOP_USE_RAW
endif

ifeq (sdsapp,$(findstring sdsapp,$(MAKECMDGOALS)))
TARGET = iot-sdsapp
C_SOURCES += examples/sdsapp/sdsapp.c \
			 src/alicloud_sds/alicloud_sds.c \
			 src/helper/jsmn/jsmn.c \
			 src/helper/jsmn/json_escape_str.c \
			 src/helper/jsmn/json_generator.c \
			 src/helper/jsmn/json_utils.c \
			 src/helper/jsmn/json_wrappers.c

C_INCLUDES += -Isrc/alicloud_sds -Isrc/helper/jsmn

CFLAGS += -DAT_SUPPORT_ALISDS
endif

ifeq (qlinkapp,$(findstring qlinkapp,$(MAKECMDGOALS)))
TARGET = iot-qlinkapp
C_SOURCES += examples/qlinkapp/main.c \
			 src/cloud_qlink/cloud_qlink.c \
			 src/helper/jsmn/jsmn.c \
			 src/helper/jsmn/json_escape_str.c \
			 src/helper/jsmn/json_generator.c \
			 src/helper/jsmn/json_utils.c \
			 src/helper/jsmn/json_wrappers.c

C_INCLUDES += -Iexamples/qlinkapp -Isrc/cloud_qlink -Isrc/helper/jsmn

CFLAGS += -DAT_SUPPORT_QLINK -DQLINK_USE_JSON
endif

# default action: build all
all test ilopicaapp iloprawapp sdsapp qlinkapp: $(BUILD_DIR)/$(TARGET).elf

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

-include $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.d)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo compiling $<
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo compiling $<
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo generating $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	@echo cleaning...
	@-rm -fR .dep $(BUILD_DIR)
	@echo done

#######################################
# dependencies
#######################################
# -include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
