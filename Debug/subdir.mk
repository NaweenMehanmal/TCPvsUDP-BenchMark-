################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Assignment1.c \
../TCPClient.c \
../TCPServer.c \
../UDPClient.c \
../UDPServer.c 

OBJS += \
./Assignment1.o \
./TCPClient.o \
./TCPServer.o \
./UDPClient.o \
./UDPServer.o 

C_DEPS += \
./Assignment1.d \
./TCPClient.d \
./TCPServer.d \
./UDPClient.d \
./UDPServer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


