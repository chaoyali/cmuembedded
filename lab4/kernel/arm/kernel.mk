ARM_OBJS := reg.o psr.o int_asm.o c_irqhandler.o init_interrupt.o init_timer.o load_user.o swihandler.o C_SWI_hander.o
ARM_OBJS := $(ARM_OBJS:%=$(KDIR)/arm/%)

KOBJS += $(ARM_OBJS)
