#
# Copyright (C) 2007, Guennadi Liakhovetski <lg@denx.de>
#
# (C) Copyright 2011-2014 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y  := mx6dl_usom.o

extra-$(CONFIG_USE_PLUGIN) :=  plugin.bin
$(obj)/plugin.bin: $(obj)/plugin.o
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@
