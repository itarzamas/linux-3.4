obj-$(CONFIG_MTD_RALINK)	+= ralink-flash.o
obj-$(CONFIG_MTD_NAND_RALINK)	+= rt_nand.o
obj-$(CONFIG_MTD_NAND_MTK)	+= rt_nand.o
obj-$(CONFIG_MTD_SPI_RALINK)	+= rt_spi.o

ifeq ($(CONFIG_RALINK_RT6XXX_MP),y)
rt_spi-objs = ralink_spi_bbu.o
else
ifeq ($(CONFIG_RALINK_MT7621),y)
rt_spi-objs = ralink_spi_bbu.o
else
ifeq ($(CONFIG_RALINK_MT7628),y)
rt_spi-objs = ralink_spi_bbu.o
else
rt_spi-objs = ralink_spi.o
endif
endif
endif

ifeq ($(CONFIG_RALINK_RT3052),y)
rt_nand-objs = ralink_nand.o gdma.o
endif

ifeq ($(CONFIG_RALINK_RT3883),y)
rt_nand-objs = ralink_nand.o gdma.o
endif

ifeq ($(CONFIG_RALINK_RT6XXX_MP),y)
EXTRA_CFLAGS += -DRALINK_NAND_BMT
rt_nand-objs = rt6xxx_nand.o rt6xxx_bmt.o
endif

ifeq ($(CONFIG_RALINK_MT7620),y)
rt_nand-objs = ralink_nand.o
endif

ifeq ($(CONFIG_RALINK_MT7621),y)
rt_nand-objs = mtk_nand.o
endif
