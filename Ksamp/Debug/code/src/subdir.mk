################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/src/cpu_info.c \
../code/src/kernel_info.c \
../code/src/main.c \
../code/src/manipulacion_archivos.c \
../code/src/sistema_info.c 

OBJS += \
./code/src/cpu_info.o \
./code/src/kernel_info.o \
./code/src/main.o \
./code/src/manipulacion_archivos.o \
./code/src/sistema_info.o 

C_DEPS += \
./code/src/cpu_info.d \
./code/src/kernel_info.d \
./code/src/main.d \
./code/src/manipulacion_archivos.d \
./code/src/sistema_info.d 


# Each subdirectory must supply rules for building sources it contributes
code/src/%.o: ../code/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


