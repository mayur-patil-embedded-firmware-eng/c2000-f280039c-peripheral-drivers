################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-413476050: ../02_adc_cpu_triggered_interrupt.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs2041/ccs/utils/sysconfig_1.26.0/sysconfig_cli.bat" -s "/.metadata/sdk.json" -d "F28003x" -p "100PZ" -r "F28003x_100PZ" --script "C:/Users/Mayur Patil/F280039C_Ws/02_adc_cpu_triggered_interrupt/02_adc_cpu_triggered_interrupt.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/error.h: build-413476050 ../02_adc_cpu_triggered_interrupt.syscfg
syscfg: build-413476050

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/Mayur Patil/F280039C_Ws/02_adc_cpu_triggered_interrupt" --include_path="C:/Users/Mayur Patil/F280039C_Ws/02_adc_cpu_triggered_interrupt/device" --include_path="/driverlib/f28003x/driverlib/" --include_path="C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Mayur Patil/F280039C_Ws/02_adc_cpu_triggered_interrupt/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


