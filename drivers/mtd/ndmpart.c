/*
 * Copyright © 2007 Eugene Konev <ejka@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * TI AR7 flash partition table.
 * Based on ar7 map by Felix Fietkau <nbd@openwrt.org>
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/bootmem.h>
#include <linux/magic.h>

#ifdef CONFIG_MTD_NDM_BOOT_UPDATE
#include <linux/crc32.h>
#include <linux/reboot.h>
#include <linux/xz.h>
#include "ndm_boot.h"
#endif

#ifndef SQUASHFS_MAGIC
#define SQUASHFS_MAGIC			0x73717368
#endif

#define NDMS_MAGIC			0x736D646E
#define CONFIG_MAGIC			cpu_to_be32(0x2e6e646d)
#define CONFIG_MAGIC_V1			cpu_to_be32(0x1f8b0801)

#define KERNEL_MAGIC			be32_to_cpu(0x27051956)
#define ROOTFS_MAGIC			SQUASHFS_MAGIC
#define PART_OPTIONAL_NUM		2		/* storage and dump */

#define MIN_FLASH_SIZE_FOR_STORAGE	0x800000	/* 8 MB */
#define MTD_MAX_RETRIES			3

enum {
	PART_U_BOOT,
	PART_U_CONFIG,
	PART_RF_EEPROM,
	PART_KERNEL,
	PART_ROOTFS,
	PART_FIRMWARE,
	PART_CONFIG,
	PART_STORAGE,	/* optional */
	PART_DUMP,	/* optional */
	PART_BACKUP,
	PART_FULL,
	PART_MAX
};

struct mtd_partition ndm_parts[PART_MAX] = {
	[PART_U_BOOT] = {
		name:			"U-Boot",  	/* mtdblock0 */
		size:			0,
		offset:			0
	},
	[PART_U_CONFIG] = {
		name:			"U-Config", 	/* mtdblock1 */
		size:			0,
		offset:			0
	},
	[PART_RF_EEPROM] = {
		name:			"RF-EEPROM", 	/* mtdblock2 */
		size:			0,
		offset:			0
	},
	[PART_KERNEL] = {
		name:			"Kernel", 	/* mtdblock3 */
		size:			0,
		offset:			0
	},
	[PART_ROOTFS] = {
		name:			"RootFS", 	/* mtdblock4 */
		size:			0,
		offset:			0
	},
	[PART_FIRMWARE] = {
		/* kernel and rootfs */
		name:			"Firmware", 	/* mtdblock5 */
		size:			0,
		offset:			0
	},
	[PART_CONFIG] = {
		name:			"Config", 	/* mtdblock6 */
		size:			0,
		offset:			0
	}
#if 0
	[PART_STORAGE]
	[PART_DUMP]
	[PART_BACKUP]	/* kernel, rootfs, config and storage */
	[PART_FULL]	/* full flash */
#endif
};

#if defined(CONFIG_MTD_NDM_BOOT_UPDATE) || \
    defined(CONFIG_MTD_NDM_CONFIG_TRANSITION)
static int mtd_write_retry(struct mtd_info *mtd, loff_t to, size_t len,
			   size_t *retlen, const u_char *buf)
{
	int ret, retries = MTD_MAX_RETRIES;

	do {
		ret = mtd_write(mtd, to, len, retlen, buf);
		if (ret) {
			printk("%s: write%s failed at 0x%012llx\n", __func__,
				"", (unsigned long long) to);
		}

		if (len != *retlen) {
			printk("%s: short write at 0x%012llx\n", __func__,
				(unsigned long long) to);
			ret = -EIO;
		}
	} while (ret && --retries);

	if (ret) {
		printk("%s: write%s failed at 0x%012llx\n", __func__,
			" completely", (unsigned long long) to);
	}

	return ret;
}

static int mtd_erase_retry(struct mtd_info *mtd, struct erase_info *instr)
{
	int ret, retries = MTD_MAX_RETRIES;

	do {
		ret = mtd_erase(mtd, instr);
		if (ret) {
			printk("%s: erase%s failed at 0x%012llx\n", __func__,
				"", (unsigned long long) instr->addr);
		}
	} while (ret && --retries);

	if (ret) {
		printk("%s: erase%s failed at 0x%012llx\n", __func__,
			" completely", (unsigned long long) instr->addr);
	}

	return ret;
}
#endif

#ifdef CONFIG_MTD_NDM_BOOT_UPDATE
static int ndm_flash_boot(struct mtd_info *master,
			  uint32_t p_off, uint32_t p_size)
{
	bool update_need;
	int res = -1, ret, retries;
	size_t len;
	struct xz_buf b;
	struct xz_dec *s;
	uint32_t off, size, es, ws;
	uint32_t dst_crc = 0, src_crc = 0;
	unsigned char *m, *v;

	es = master->erasesize;

	/* Check bootloader size */
	if (boot_bin_len > p_size) {
		printk(KERN_ERR "too big bootloader\n");
		goto out;
	}

	/* Read current bootloader */
	m = kmalloc(p_size, GFP_KERNEL);
	if (m == NULL)
		goto out;

	/* Alloc verify buffer */
	v = kmalloc(es, GFP_KERNEL);
	if (v == NULL)
		goto out_kfree;

	ret = mtd_read(master, p_off, p_size, &len, m);
	if (ret || len != p_size) {
		printk(KERN_ERR "read failed");
		goto out_kfree;
	}

	/* Detect and compare version */
	len = strlen(NDM_BOOT_VERSION);
	size = p_size;
	update_need = true;

	for (off = 0; off < size; off++) {
		if (size - off < len)
			break;
		else if (!memcmp(NDM_BOOT_VERSION, m + off, len)) {
			update_need = false;
			break;
		}
	}

	if (!update_need) {
		printk(KERN_INFO "Bootloader is up to date\n");
		res = 0;
		goto out_kfree;
	}

	printk(KERN_INFO "Updating bootloader...\n");

	/* Decompress bootloader */
	s = xz_dec_init(XZ_SINGLE, 0);
	if (s == NULL) {
		printk(KERN_ERR "xz_dec_init error\n");
		goto out_kfree;
	}

	b.in = boot_bin_xz;
	b.in_pos = 0;
	b.in_size = boot_bin_xz_len;

	b.out = m;
	b.out_pos = 0;
	b.out_size = boot_bin_len;

	ret = xz_dec_run(s, &b);
	if (ret != XZ_STREAM_END) {
		printk(KERN_ERR "zx_dec_run error (%d)\n", ret);
		goto out_xz_dec_end;
	}

	size = ALIGN(b.out_pos, master->writesize);

	/* fill padding */
	if (size > b.out_pos)
		memset(m + b.out_pos, 0xff, size - b.out_pos);

	/* erase & write -> verify */
	for (off = 0; off < size; off += es) {
		struct erase_info ei = {
			.mtd = master,
			.addr = off + p_off,
			.len = es
		};

		/* write size can be < erase size */
		ws = min(es, size - off);

		src_crc = crc32(0, m + off, ws);
		dst_crc = src_crc + 1;

		retries = MTD_MAX_RETRIES;
		do {
			ret = mtd_erase_retry(master, &ei);
			if (ret)
				goto out_write_fail;

			ret = mtd_write_retry(master, ei.addr, ws, &len,
					      m + off);
			if (ret)
				goto out_write_fail;

			memset(v, 0xff, ws);
			ret = mtd_read(master, ei.addr, ws, &len, v);
			if (ret == 0 && len == ws)
				dst_crc = crc32(0, v, ws);

		} while (src_crc != dst_crc && --retries);
	}

	if (src_crc == dst_crc) {
		res = 0;
		printk("Bootloader update complete, do reboot...\n");
		kernel_restart(NULL);
	}

out_write_fail:
	if (src_crc != dst_crc)
		printk(KERN_ERR "Bootloader update FAILED!"
			" Device may be bricked!\n");
out_xz_dec_end:
	xz_dec_end(s);
out_kfree:
	kfree(v);
	kfree(m);
out:
	return res;
}
#endif

#ifdef CONFIG_MTD_NDM_CONFIG_TRANSITION
static int config_find(struct mtd_info *master, u32 *offset)
{
	static const u32 TRANSITION_OFFSETS[] = {
#ifdef CONFIG_MTD_NDM_CONFIG_TRANSITION_OFFSET
		CONFIG_MTD_NDM_CONFIG_TRANSITION_OFFSET,
#endif
#ifdef CONFIG_MTD_NDM_CONFIG_TRANSITION_OFFSET_2
		CONFIG_MTD_NDM_CONFIG_TRANSITION_OFFSET_2,
#endif
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(TRANSITION_OFFSETS); i++) {
		const u32 offs = TRANSITION_OFFSETS[i];
		__le32 magic;
		size_t len;
		int ret;

		if (offs == 0)
			continue;

		ret = mtd_read(master, offs, sizeof magic, &len,
				   (u8 *) &magic);

		if (ret != 0 || len != sizeof magic) {
			printk(KERN_ERR "read failed at 0x%012llx\n",
			       (unsigned long long) offs);
			return -EIO;
		}

		if (magic == CONFIG_MAGIC || magic == CONFIG_MAGIC_V1) {
			*offset = offs;
			return 0;
		}
	}

	return -ENOENT;
}

/*
 * TODO:
 * - erase full partition (?);
 * - smart detecting size of config.
 */
static void config_move(struct mtd_info *master, uint32_t offset_new)
{
	int ret;
	u32 offset;
	size_t len;
	struct erase_info ei;
	unsigned char *iobuf;

	ret = config_find(master, &offset);
	if (ret != 0)
		goto out;

	printk(KERN_INFO "Found config in old partition at 0x%012llx, move it\n",
		(unsigned long long) offset);

	iobuf = kmalloc(master->erasesize, GFP_KERNEL);
	if (iobuf == NULL) {
		printk(KERN_ERR "no memory\n");
		goto out;
	}

	/* Read old config */
	ret = mtd_read(master, offset, master->erasesize,
			&len, iobuf);
	if (ret || len != master->erasesize) {
		printk(KERN_ERR "read failed at 0x%012llx\n",
			(unsigned long long) offset);
		goto out_kfree;
	}

	/* Erase new place */
	memset(&ei, 0, sizeof(struct erase_info));
	ei.mtd  = master;
	ei.addr = offset_new;
	ei.len  = master->erasesize;

	ret = mtd_erase_retry(master, &ei);
	if (ret)
		goto out_kfree;

	/* Write config to new place */
	ret = mtd_write_retry(master, offset_new,
			master->erasesize, &len, iobuf);
	if (ret)
		goto out_kfree;

	/* Erase old place */
	memset(&ei, 0, sizeof(struct erase_info));
	ei.mtd  = master;
	ei.addr = offset;
	ei.len  = master->erasesize;

	mtd_erase_retry(master, &ei);
out_kfree:
	kfree(iobuf);
out:
	return;
}
#endif

static inline unsigned part_u_boot_size(struct mtd_info *master)
{
	unsigned size;

	if (master->type == MTD_NANDFLASH)
#ifdef CONFIG_RALINK_MT7621
		size = master->erasesize << 2;
#else
		size = master->erasesize;
#endif
	else
		size = 3 * master->erasesize;

	return size;
}

static inline unsigned part_u_config_size(struct mtd_info *master)
{
	unsigned size;

#ifdef CONFIG_RALINK_MT7621
	if (master->type == MTD_NANDFLASH)
		size = master->erasesize << 2;
	else
#endif
	size = master->erasesize;

	return size;
}

static inline unsigned part_config_size(struct mtd_info *master)
{
	unsigned size;

#ifdef CONFIG_RALINK_MT7621
	if (master->type == MTD_NANDFLASH)
		size = master->erasesize << 2;
	else
#endif
	size = master->erasesize;

	return size;
}

static int create_mtd_partitions(struct mtd_info *master,
				 struct mtd_partition **pparts,
				 struct mtd_part_parser_data *data)
{
	bool use_dump, use_storage;
	int index, dump_index, part_num, storage_index;
	size_t len;
	uint32_t config_offset, offset, flash_size, flash_size_lim;
	__le32 magic;

	flash_size = master->size;

	flash_size_lim = CONFIG_MTD_NDM_FLASH_SIZE_LIMIT;
	if (!flash_size_lim)
		flash_size_lim = flash_size;

	printk(KERN_INFO "Current flash size = 0x%x\n", flash_size);

	/* U-Boot */
	ndm_parts[PART_U_BOOT].size = part_u_boot_size(master);

	/* U-Config */
	ndm_parts[PART_U_CONFIG].offset = ndm_parts[PART_U_BOOT].size;
	ndm_parts[PART_U_CONFIG].size = part_u_config_size(master);

	/* RF-EEPROM */
	ndm_parts[PART_RF_EEPROM].offset = ndm_parts[PART_U_CONFIG].offset +
					   ndm_parts[PART_U_CONFIG].size;

	/*
	 * TODO: Move to separate function.
	 */
	for (offset = ndm_parts[PART_RF_EEPROM].offset;
	     offset < flash_size_lim; offset += master->erasesize) {
		
		mtd_read(master, offset, sizeof(magic), &len,
		             (uint8_t *) &magic);
		if (magic == KERNEL_MAGIC){
			printk(KERN_INFO "Found kernel at offset 0x%x\n",
			       offset);

			ndm_parts[PART_RF_EEPROM].size = offset -
				ndm_parts[PART_RF_EEPROM].offset;
			ndm_parts[PART_KERNEL].offset = offset;
		}
		if ((le32_to_cpu(magic) == ROOTFS_MAGIC) ||
		    (le32_to_cpu(magic) == NDMS_MAGIC)) {
			printk(KERN_INFO "Found rootfs at offset 0x%x\n", offset);

			ndm_parts[PART_KERNEL].size = offset -
				ndm_parts[PART_KERNEL].offset;
			ndm_parts[PART_ROOTFS].offset = offset;
			break;
		}
	}
	
	index = PART_CONFIG + 1;

	/* Dump & Storage */
	use_dump = use_storage = false;
	dump_index = storage_index = 0;

	part_num = PART_MAX - PART_OPTIONAL_NUM;

	if (CONFIG_MTD_NDM_DUMP_SIZE)
		use_dump = true;

	if (CONFIG_MTD_NDM_STORAGE_SIZE &&
	    flash_size_lim >= MIN_FLASH_SIZE_FOR_STORAGE)
		use_storage = true;

	if (use_storage) {
		ndm_parts[index].name = "Storage";
		ndm_parts[index].size = CONFIG_MTD_NDM_STORAGE_SIZE;

		part_num++;
		storage_index = index;
		index++;
	}

	if (use_dump) {
		ndm_parts[index].name = "Dump";
		ndm_parts[index].size = CONFIG_MTD_NDM_DUMP_SIZE;
		ndm_parts[index].offset = flash_size_lim - CONFIG_MTD_NDM_DUMP_SIZE;

		part_num++;
		dump_index = index;
		index++;
	}

	ndm_parts[PART_CONFIG].size = part_config_size(master);

	if (use_dump && !use_storage) {
		config_offset = ndm_parts[dump_index].offset -
				ndm_parts[PART_CONFIG].size;
	} else if (!use_dump && use_storage) {
		ndm_parts[storage_index].offset = flash_size_lim -
						  CONFIG_MTD_NDM_STORAGE_SIZE;
		config_offset = ndm_parts[storage_index].offset -
				ndm_parts[PART_CONFIG].size;
	} else if (use_dump && use_storage) {
		ndm_parts[storage_index].offset = ndm_parts[dump_index].offset -
						  CONFIG_MTD_NDM_STORAGE_SIZE;
		config_offset = ndm_parts[storage_index].offset -
				ndm_parts[PART_CONFIG].size;
	} else {
		config_offset = flash_size_lim - ndm_parts[PART_CONFIG].size;
	}

	/* Config */
	ndm_parts[PART_CONFIG].offset = config_offset;
#ifdef CONFIG_MTD_NDM_CONFIG_TRANSITION
	config_move(master, ndm_parts[PART_CONFIG].offset);
#endif

	/* Backup */
	ndm_parts[index].name = "Backup";
	ndm_parts[index].offset = ndm_parts[PART_KERNEL].offset;
	ndm_parts[index].size = flash_size_lim - ndm_parts[index].offset;

	if (use_dump)
		ndm_parts[index].size -= ndm_parts[dump_index].size;

	index++;

	/* Full */
	ndm_parts[index].name = "Full";
	ndm_parts[index].size = MTDPART_SIZ_FULL;
	ndm_parts[index].offset = 0;
	ndm_parts[index].mask_flags = MTD_WRITEABLE;

	/* Firmware */
	ndm_parts[PART_FIRMWARE].offset = ndm_parts[PART_KERNEL].offset;
	ndm_parts[PART_FIRMWARE].size = ndm_parts[PART_CONFIG].offset -
					ndm_parts[PART_FIRMWARE].offset;

	/* Rootfs */
	ndm_parts[PART_ROOTFS].size = ndm_parts[PART_CONFIG].offset -
				      ndm_parts[PART_ROOTFS].offset;

	*pparts = kmemdup(ndm_parts, sizeof(struct mtd_partition) * part_num,
			  GFP_KERNEL);

#ifdef CONFIG_MTD_NDM_BOOT_UPDATE
	ndm_flash_boot(master, ndm_parts[PART_U_BOOT].offset,
		       ndm_parts[PART_U_BOOT].size);
#endif

	return part_num;
}

static struct mtd_part_parser ndm_parser = {
	.owner = THIS_MODULE,
	.parse_fn = create_mtd_partitions,
	.name = "ndmpart",
};

static int __init ndm_parser_init(void)
{
	printk(KERN_INFO "Registering NDM partitions parser\n");
	return register_mtd_parser(&ndm_parser);
}

module_init(ndm_parser_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NDM Systems Inc. <info@ndmsystems.com>");
MODULE_DESCRIPTION("MTD partitioning for NDM devices");
