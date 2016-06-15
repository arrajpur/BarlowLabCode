################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sflbits.c \
../sflcomp.c \
../sflcons.c \
../sflconv.c \
../sflcryp.c \
../sflcvbs.c \
../sflcvdp.c \
../sflcvds.c \
../sflcvns.c \
../sflcvsb.c \
../sflcvsd.c \
../sflcvsn.c \
../sflcvst.c \
../sflcvtp.c \
../sflcvts.c \
../sfldate.c \
../sfldbio.c \
../sfldir.c \
../sflenv.c \
../sflexdr.c \
../sflfile.c \
../sflfind.c \
../sflfort.c \
../sflhttp.c \
../sflini.c \
../sfllang.c \
../sfllbuf.c \
../sfllist.c \
../sflmail.c \
../sflmath.c \
../sflmem.c \
../sflmesg.c \
../sflmime.c \
../sflmysql.c \
../sflnode.c \
../sflprint.c \
../sflproc.c \
../sflslot.c \
../sflsock.c \
../sflstr.c \
../sflsymb.c \
../sflsyst.c \
../sfltok.c \
../sfltree.c \
../sfltron.c \
../sfluid.c \
../sflxml.c \
../sflxmll.c \
../testall.c \
../testbits.c \
../testcgi.c \
../testcomp.c \
../testconv.c \
../testcryp.c \
../testdate.c \
../testdes.c \
../testdir.c \
../testexdr.c \
../testfind.c \
../testfort.c \
../testmem.c \
../testmime.c \
../teststr.c \
../testsym.c \
../testtree.c \
../testtron.c \
../testuid.c \
../testxml.c 

OBJS += \
./sflbits.o \
./sflcomp.o \
./sflcons.o \
./sflconv.o \
./sflcryp.o \
./sflcvbs.o \
./sflcvdp.o \
./sflcvds.o \
./sflcvns.o \
./sflcvsb.o \
./sflcvsd.o \
./sflcvsn.o \
./sflcvst.o \
./sflcvtp.o \
./sflcvts.o \
./sfldate.o \
./sfldbio.o \
./sfldir.o \
./sflenv.o \
./sflexdr.o \
./sflfile.o \
./sflfind.o \
./sflfort.o \
./sflhttp.o \
./sflini.o \
./sfllang.o \
./sfllbuf.o \
./sfllist.o \
./sflmail.o \
./sflmath.o \
./sflmem.o \
./sflmesg.o \
./sflmime.o \
./sflmysql.o \
./sflnode.o \
./sflprint.o \
./sflproc.o \
./sflslot.o \
./sflsock.o \
./sflstr.o \
./sflsymb.o \
./sflsyst.o \
./sfltok.o \
./sfltree.o \
./sfltron.o \
./sfluid.o \
./sflxml.o \
./sflxmll.o \
./testall.o \
./testbits.o \
./testcgi.o \
./testcomp.o \
./testconv.o \
./testcryp.o \
./testdate.o \
./testdes.o \
./testdir.o \
./testexdr.o \
./testfind.o \
./testfort.o \
./testmem.o \
./testmime.o \
./teststr.o \
./testsym.o \
./testtree.o \
./testtron.o \
./testuid.o \
./testxml.o 

C_DEPS += \
./sflbits.d \
./sflcomp.d \
./sflcons.d \
./sflconv.d \
./sflcryp.d \
./sflcvbs.d \
./sflcvdp.d \
./sflcvds.d \
./sflcvns.d \
./sflcvsb.d \
./sflcvsd.d \
./sflcvsn.d \
./sflcvst.d \
./sflcvtp.d \
./sflcvts.d \
./sfldate.d \
./sfldbio.d \
./sfldir.d \
./sflenv.d \
./sflexdr.d \
./sflfile.d \
./sflfind.d \
./sflfort.d \
./sflhttp.d \
./sflini.d \
./sfllang.d \
./sfllbuf.d \
./sfllist.d \
./sflmail.d \
./sflmath.d \
./sflmem.d \
./sflmesg.d \
./sflmime.d \
./sflmysql.d \
./sflnode.d \
./sflprint.d \
./sflproc.d \
./sflslot.d \
./sflsock.d \
./sflstr.d \
./sflsymb.d \
./sflsyst.d \
./sfltok.d \
./sfltree.d \
./sfltron.d \
./sfluid.d \
./sflxml.d \
./sflxmll.d \
./testall.d \
./testbits.d \
./testcgi.d \
./testcomp.d \
./testconv.d \
./testcryp.d \
./testdate.d \
./testdes.d \
./testdir.d \
./testexdr.d \
./testfind.d \
./testfort.d \
./testmem.d \
./testmime.d \
./teststr.d \
./testsym.d \
./testtree.d \
./testtron.d \
./testuid.d \
./testxml.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D__UNIX__ -D__64BIT__ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


