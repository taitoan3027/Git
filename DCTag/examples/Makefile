ifndef CROSS_COMPILE
CROSS_COMPILE	:= arm-linux-gnueabihf-
endif

ifndef ARCH_FLAG
ARCH_FLAG		:=
endif

ifndef CPU_FLAG
CPU_FLAG		:= -mcpu=cortex-a8
endif

ifndef OUT_DIR
OUT_DIR			:= $(shell pwd)/output
endif

ifndef INC_DIR
INC_DIR			:= -I. -I"$(shell pwd)/../include"
endif

ifndef LIB_DIR
LIB_DIR			:= -L"$(shell pwd)/../lib"
endif

ifndef MKDIR
ifdef SystemRoot 						# for Windows sytem
MKDIR			:= mkdir.exe
else
MKDIR			:= mkdir
endif
endif

ifndef MV
MV				:= mv -f
endif

all:dc_tag_owner readtag dc_tag_read dc_tag_write dc_tag_read_subscription dc_tag_update 

dc_tag_owner:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

sendfile:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

test1:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

manage_folder:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

read_tag:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c


dc_tag_read:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

dc_tag_write:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

dc_tag_read_subscription:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

dc_tag_update:
	@echo Compiling: $@
	@$(MKDIR) -p $(OUT_DIR)
	@$(CROSS_COMPILE)gcc $(ARCH_FLAG) $(CPU_FLAG) -O -g -Wall -lAPAL -lDCTag -lrt $(INC_DIR) $(LIB_DIR) -o $(OUT_DIR)/$@ $@.c

clean:
	@$(RM) -r $(OBJ_DIR) $(OUT_DIR)/*

.PHONY: all
