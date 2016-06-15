################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./SplitPeaks/unzip.c 

CPP_SRCS += \
./SplitPeaks/BedGraphParser.cpp \
./SplitPeaks/Parser.cpp \
./SplitPeaks/PeakParser.cpp \
./SplitPeaks/Position.cpp \
./SplitPeaks/SequenceDownloader.cpp \
./SplitPeaks/Split.cpp \
./SplitPeaks/SplitPeaks.cpp \
./SplitPeaks/SubPeak.cpp \
./SplitPeaks/variableStepParser.cpp \
./SplitPeaks/wigPosition.cpp 

OBJS += \
./BedGraphParser.o \
./Parser.o \
./PeakParser.o \
./Position.o \
./SequenceDownloader.o \
./Split.o \
./SplitPeaks.o \
./SubPeak.o \
./unzip.o \
./variableStepParser.o \
./wigPosition.o 

C_DEPS += \
./unzip.d 

CPP_DEPS += \
./BedGraphParser.d \
./Parser.d \
./PeakParser.d \
./Position.d \
./SequenceDownloader.d \
./Split.d \
./SplitPeaks.d \
./SubPeak.d \
./variableStepParser.d \
./wigPosition.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ./SplitPeaks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__UNIX__ -D__64BIT__ -I./SplitPeaks -I./SFL -I./Zlib -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ./SplitPeaks/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__UNIX__ -D__64BIT__ -I./SFL -I./Zlib -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


