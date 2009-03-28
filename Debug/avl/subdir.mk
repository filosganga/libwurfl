################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../avl/avl.c \
../avl/bst.c \
../avl/rb.c 

OBJS += \
./avl/avl.o \
./avl/bst.o \
./avl/rb.o 

C_DEPS += \
./avl/avl.d \
./avl/bst.d \
./avl/rb.d 


# Each subdirectory must supply rules for building sources it contributes
avl/%.o: ../avl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/libxml2 -O2 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


