################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adler32.c \
../compress.c \
../crc32.c \
../deflate.c \
../gzio.c \
../infback.c \
../inffast.c \
../inflate.c \
../inftrees.c \
../ioapi.c \
../trees.c \
../uncompr.c \
../zutil.c 

OBJS += \
./adler32.o \
./compress.o \
./crc32.o \
./deflate.o \
./gzio.o \
./infback.o \
./inffast.o \
./inflate.o \
./inftrees.o \
./ioapi.o \
./trees.o \
./uncompr.o \
./zutil.o 

C_DEPS += \
./adler32.d \
./compress.d \
./crc32.d \
./deflate.d \
./gzio.d \
./infback.d \
./inffast.d \
./inflate.d \
./inftrees.d \
./ioapi.d \
./trees.d \
./uncompr.d \
./zutil.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


