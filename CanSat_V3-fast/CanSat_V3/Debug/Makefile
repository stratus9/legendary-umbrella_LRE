################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../FATFS/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../AD7195.c \
../CanSat.c \
../FATFS/ff.c \
../I2C.c \
../RealTime.c \
../SD.c \
../SPI.c \
../Initialization.c \
../util.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
AD7195.o \
CanSat.o \
FATFS/ff.o \
I2C.o \
RealTime.o \
SD.o \
SPI.o \
Initialization.o \
util.o

OBJS_AS_ARGS +=  \
AD7195.o \
CanSat.o \
FATFS/ff.o \
I2C.o \
RealTime.o \
SD.o \
SPI.o \
Initialization.o \
util.o

C_DEPS +=  \
AD7195.d \
CanSat.d \
FATFS/ff.d \
I2C.d \
RealTime.d \
SD.d \
SPI.d \
Initialization.d \
util.d

C_DEPS_AS_ARGS +=  \
AD7195.d \
CanSat.d \
FATFS/ff.d \
I2C.d \
RealTime.d \
SD.d \
SPI.d \
Initialization.d \
util.d

OUTPUT_FILE_PATH +=V1.0.elf

OUTPUT_FILE_PATH_AS_ARGS +=V1.0.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000  -I"E:\Atmel\AS7\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\include"  -O1 -ffunction-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atxmega128a3u -B "E:\Atmel\AS7\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\gcc\dev\atxmega128a3u" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

FATFS/%.o: ../FATFS/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=32000000  -I"E:\Atmel\AS7\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\include"  -O1 -ffunction-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atxmega128a3u -B "E:\Atmel\AS7\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\gcc\dev\atxmega128a3u" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="V1.0.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atxmega128a3u -B "E:\Atmel\AS7\7.0\Packs\atmel\XMEGAA_DFP\1.0.36\gcc\dev\atxmega128a3u"  
	@echo Finished building target: $@
	"E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "V1.0.elf" "V1.0.hex"
	"E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "V1.0.elf" "V1.0.eep" || exit 0
	"E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "V1.0.elf" > "V1.0.lss"
	"E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "V1.0.elf" "V1.0.srec"
	"E:\Atmel\AS7\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "V1.0.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "V1.0.elf" "V1.0.a" "V1.0.hex" "V1.0.lss" "V1.0.eep" "V1.0.map" "V1.0.srec" "V1.0.usersignatures"
	