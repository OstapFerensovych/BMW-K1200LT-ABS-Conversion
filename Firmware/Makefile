include config.mk
-include sources.mk

DEBUG         ?= 1
VERBOSE_BUILD ?= 0
SEMIHOSTING   ?= 0

# scan-build --use-cc arm-none-eabi-gcc --use-c++ arm-none-eabi-g++ make SCAN_BUILD=1

CROSS_COMPILE := arm-none-eabi-

ifeq ($(SCAN_BUILD),)
CC       := $(CROSS_COMPILE)gcc
CXX      := $(CROSS_COMPILE)g++
endif

AS       := $(CROSS_COMPILE)gcc -x assembler-with-cpp
NM       := $(CROSS_COMPILE)nm
OBJCOPY  := $(CROSS_COMPILE)objcopy
OBJDUMP  := $(CROSS_COMPILE)objdump
SIZE     := $(CROSS_COMPILE)size

CC_OBJ   := $(CC_SRC:.c=.o)
CXX_OBJ  := $(CXX_SRC:.cpp=.o)
AS_OBJ   := $(AS_SRC:.s=.o)
OBJ      := $(CC_OBJ) $(CXX_OBJ) $(AS_OBJ)
DEPFILES := $(OBJ:.o=.d)

BUILD_DATE := $(shell LC_ALL=C date "+%D %H:%M")
OUT_DIR    := out

OPENOCD := openocd -f interface/jlink.cfg \
		-f target/stm32f0x.cfg

##############################################################################################################################
# Debug/Release options
##############################################################################################################################
DEBUG_OPT_FLAGS :=   -Og -ggdb3 -gdwarf-4 -fvar-tracking -fvar-tracking-assignments
RELEASE_OPT_FLAGS := -Os -flto -g1 -fno-fat-lto-objects

ifeq ($(SEMIHOSTING),0)
	SEMIHOSTING_FLAGS := --specs=nosys.specs
else
	SEMIHOSTING_FLAGS := --specs=rdimon.specs -lrdimon
endif

ifeq ($(DEBUG),0)
	GLOBAL_DEFS += -DNDEBUG
	ALWAYS_RELEASE :=
	OPTFLAGS := $(RELEASE_OPT_FLAGS)
	BUILD_DATE  := "$(BUILD_DATE) R"
else
	OPTFLAGS := $(DEBUG_OPT_FLAGS)
	BUILD_DATE  := "$(BUILD_DATE) D"
endif

GLOBAL_DEFS  += -DBUILD_DATE=\"$(BUILD_DATE)\"

##############################################################################################################################
# Warning options
##############################################################################################################################
CMN_WARN := -Wall -Wextra -Wdouble-promotion -Wpointer-arith -Wcast-qual -Wcast-align -Wuninitialized -Wmaybe-uninitialized \
			-Wswitch-default -Winline -Wunused-local-typedefs -Wshadow -Wunreachable-code -Wstrict-overflow=5 \
			-Warray-bounds=2 -Wformat=2 -Wshift-negative-value -Wshift-overflow -Wmisleading-indentation -Wstrict-aliasing=1 \
			-Wduplicated-cond -Wfloat-equal -Wparentheses -Wlogical-op -Wmissing-declarations -Wpacked \
			-Wdisabled-optimization -Wnull-dereference -Wattributes 
CC_WARN  := $(CMN_WARN) -Wbad-function-cast -Wnested-externs -Wjump-misses-init -Woverride-init-side-effects $(SILENCE_WARNINGS)
CXX_WARN := $(CMN_WARN) $(SILENCE_WARNINGS) -Wno-volatile

##############################################################################################################################
# Code generation options
##############################################################################################################################
CMN_CODEGEN := -ffunction-sections -fdata-sections -ffast-math -fmerge-all-constants -fstrict-overflow -fno-math-errno \
			-ffreestanding -foptimize-strlen -fno-common -fvisibility=hidden -fsingle-precision-constant -fstore-merging \
			-fstrict-aliasing

CC_CODEGEN  := $(CMN_CODEGEN)
CXX_CODEGEN := $(CMN_CODEGEN) -fno-rtti -fno-exceptions -fdevirtualize -fdevirtualize-speculatively -fdevirtualize-at-ltrans

##############################################################################################################################
# Complete command lines
##############################################################################################################################
CFLAGS		:= -std=gnu11   $(CC_WARN)  $(CC_CODEGEN)  $(CPU_FLAGS) $(INCLPATHS) $(GLOBAL_DEFS)
CXXFLAGS	:= -std=gnu++2a $(CXX_WARN) $(CXX_CODEGEN) $(CPU_FLAGS) $(INCLPATHS) $(GLOBAL_DEFS)
LDFLAGS		:= $(CPU_FLAGS) $(CXX_CODEGEN) $(GLOBAL_DEFS) $(LIBS) -T$(LDSCRIPT) -Wl,--gc-sections,--warn-common \
			--specs=nano.specs -static $(SEMIHOSTING_FLAGS)

##############################################################################################################################
# Build recipes
##############################################################################################################################
.PHONY: all clean flash

all: sources.mk \
	Qt_$(APPLICATION).creator \
	Qt_$(APPLICATION).includes \
	Qt_$(APPLICATION).files \
	Qt_$(APPLICATION).config \
	$(OUT_DIR)/$(APPLICATION).elf
ifeq ($(VERBOSE_BUILD), 1)
	$(eval SIZE_CMD1 = arm-none-eabi-nm $(OUT_DIR)/$(APPLICATION).elf -rSC --size-sort --radix=dec | grep)
	$(eval SIZE_CMD2 = | head -n 5 | awk '{printf "| %-5d | %s | %-39s|\n", $$$$2, $$$$3, $$$$4}' )
	@echo
	@echo "+-------+---+------------------------------- RW Data +"
	@$(SIZE_CMD1) ' [BbDd] ' $(SIZE_CMD2)
	@echo "+-------+---+------------------------------- RO Data +"
	@$(SIZE_CMD1) ' [Rr] ' $(SIZE_CMD2)
	@echo "+-------+---+---------------------------------- Text +"
	@$(SIZE_CMD1) ' [Tt] ' $(SIZE_CMD2)
	@echo "+-------+---+----------------------------------------+"
endif
	@$(SIZE) $(OUT_DIR)/$(APPLICATION).elf | tail -n 1 | awk '{printf "\nFlash: %8.8s (%.5s%%)\nRAM:   %8.8s (%.5s%%)\n\n", $$1+$$2, 100*($$1+$$2)/($(FLASH_SIZE)), $$2+$$3, 100*($$2+$$3)/($(RAM_SIZE))}'

flash: all
	$(OPENOCD) -c init \
		-c 'reset halt' \
		-c 'flash write_image erase $(OUT_DIR)/$(APPLICATION).elf' \
		-c 'reset run' \
		-c exit

clean:
	@echo Cleaning up...
	@find -L  -type f -name "*.[od]" -delete
	@rm -rf sources.mk $(DEPFILES) $(OUT_DIR)/*
	@echo > Qt_$(APPLICATION).creator
	@echo > Qt_$(APPLICATION).includes
	@echo > Qt_$(APPLICATION).files
	@echo > Qt_$(APPLICATION).config

$(OUT_DIR)/$(APPLICATION).elf:$(OBJ)
	@echo [ld ] $(APPLICATION).elf
	@mkdir -p $(OUT_DIR)
	@$(CXX) $(LDFLAGS) $(OPTFLAGS) $(OBJ) -o $(OUT_DIR)/$(APPLICATION).elf
	@$(OBJCOPY) -Oihex $(OUT_DIR)/$(APPLICATION).elf $(OUT_DIR)/$(APPLICATION).hex
	@$(OBJDUMP) -drwC $(OUT_DIR)/$(APPLICATION).elf > $(OUT_DIR)/$(APPLICATION).asm
	@echo $(OUT_DIR)/$(APPLICATION).hex >> Qt_$(APPLICATION).files
	@echo $(OUT_DIR)/$(APPLICATION).asm >> Qt_$(APPLICATION).files

define always_release_template =
$(1)/%.o:$(1)/%.c config.mk
	@echo [rel] `basename $$*.c`
	@$(CC) $(CFLAGS) $(RELEASE_OPT_FLAGS) -DNDEBUG -MMD -c $(1)/$$*.c -o $(1)/$$*.o

$(1)/%.o:$(1)/%.cpp config.mk
	@echo [rel] `basename $$*.cpp`
	@$(CXX) $(CXXFLAGS) $(RELEASE_OPT_FLAGS) -DNDEBUG -MMD -c $(1)/$$*.cpp -o $(1)/$$*.o
endef

$(foreach always_release,$(ALWAYS_RELEASE),$(eval $(call always_release_template,$(always_release))))

%.o:%.c config.mk
	@echo [gcc] `basename $*.c`
	@$(CC) $(CFLAGS) $(OPTFLAGS) -MMD -c $*.c -o $*.o

%.o:%.cpp config.mk
	@echo [g++] `basename $*.cpp`
	@$(CXX) $(CXXFLAGS) $(OPTFLAGS) -MMD -c $*.cpp -o $*.o

%.o:%.s config.mk
	@echo [as ] `basename $*.s`
	@$(AS) $(CFLAGS) $(OPTFLAGS) -MMD -c $*.s -o $*.o

Qt_$(APPLICATION).creator:
Qt_$(APPLICATION).includes:
Qt_$(APPLICATION).files:
Qt_$(APPLICATION).config:
sources.mk:
	@echo Bootstrapping...
	$(eval INCLPATHS := $(shell find -L . -type d -not -iwholename '*.git*' -exec echo -n "-I{} " \;))
	$(eval CC_SRC  := $(shell find -L . -type f -name "*.c"))
	$(eval CXX_SRC := $(shell find -L . -type f -name "*.cpp"))
	$(eval AS_SRC  := $(shell find -L . -type f -name "*.s"))
	$(eval HEADERS  := $(shell find -L . -type f -name "*.h" -or -name '*.hpp'))
	$(eval LDSCRIPT  := $(shell find -L . -type f -name "*.ld" | head -n 1))
	@echo "INCLPATHS := $(INCLPATHS)" > sources.mk
	@echo "CC_SRC    := $(CC_SRC)   ">> sources.mk
	@echo "CXX_SRC   := $(CXX_SRC)  ">> sources.mk
	@echo "AS_SRC    := $(AS_SRC)   ">> sources.mk
	@echo "LDSCRIPT  := $(LDSCRIPT) ">> sources.mk
	@touch Qt_$(APPLICATION).creator
	@echo $(CC_SRC) $(CXX_SRC) $(AS_SRC) $(HEADERS) $(LDSCRIPT) config.mk Makefile uncrustify.cfg | sed -e 's/\s\+/\n/g' > Qt_$(APPLICATION).files
	@find -L . -type d -not -iwholename '*.git*' > Qt_$(APPLICATION).includes
	@echo "/* Automatically detected defines */" > Qt_$(APPLICATION).config
	@touch Qt_$(APPLICATION).config
	@echo "" >> Qt_$(APPLICATION).config;
	@for DEFINE in $(GLOBAL_DEFS); do \
		echo $$DEFINE | sed "s/-D/#define /;s/=/ /" >> Qt_$(APPLICATION).config; \
	done

-include $(DEPFILES)
