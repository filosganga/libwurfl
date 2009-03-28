################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../collection/hashlib.c \
../collection/hashmap.c \
../collection/list.c \
../collection/pavl.c \
../collection/treeset.c 

OBJS += \
./collection/hashlib.o \
./collection/hashmap.o \
./collection/list.o \
./collection/pavl.o \
./collection/treeset.o 

C_DEPS += \
./collection/hashlib.d \
./collection/hashmap.d \
./collection/list.d \
./collection/pavl.d \
./collection/treeset.d 


# Each subdirectory must supply rules for building sources it contributes
collection/%.o: ../collection/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/libxml2 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


