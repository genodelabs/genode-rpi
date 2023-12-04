/*
 * \brief  Linux emulation environment specific to this driver
 * \author Stefan Kalkowski
 * \date   2021-08-31
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <lx_emul.h>
#include <linux/slab.h>


#include <linux/dmapool.h>

struct dma_pool { size_t size; };

void * dma_pool_alloc(struct dma_pool * pool, gfp_t mem_flags, dma_addr_t * handle)
{
	void * ret =
		lx_emul_mem_alloc_aligned_uncached(pool->size, PAGE_SIZE);
	*handle = lx_emul_mem_dma_addr(ret);
	return ret;
}


struct dma_pool * dma_pool_create(const char * name,
                                  struct device * dev,
                                  size_t size,
                                  size_t align,
                                  size_t boundary)
{
	struct dma_pool * pool = kmalloc(sizeof(struct dma_pool), GFP_KERNEL);
	pool->size = size;
	return pool;
}


void dma_pool_free(struct dma_pool * pool,void * vaddr,dma_addr_t dma)
{
	lx_emul_mem_free(vaddr);
}


#include <asm/uaccess.h>

unsigned long arm_copy_from_user(void *to, const void *from, unsigned long n)
{
	memcpy(to, from, n);
	return 0;
}


#include <asm/uaccess.h>

unsigned long arm_copy_to_user(void *to, const void *from, unsigned long n)
{
	memcpy(to, from, n);
	return 0;
}


#include <asm-generic/logic_io.h>
#include <lx_emul/io_mem.h>

void __iomem * ioremap(resource_size_t phys_addr, size_t size)
{
	return lx_emul_io_mem_map(phys_addr, size);
}


extern void arm_heavy_mb(void);
void arm_heavy_mb(void)
{
}


#include <linux/irqchip.h>

static void dde_irq_unmask(struct irq_data *d)
{
	lx_emul_irq_unmask(d->hwirq);
}


static void dde_irq_mask(struct irq_data *d)
{
	lx_emul_irq_mask(d->hwirq);
}


static void dde_irq_eoi(struct irq_data *d)
{
	lx_emul_irq_eoi(d->hwirq);
}

static struct irq_chip bcm_irq_chip = {
	.name       = "ARMCTRL-level",
	.irq_mask   = dde_irq_mask,
	.irq_unmask = dde_irq_unmask,
	.irq_eoi    = dde_irq_eoi,
};


static int bcm_irq_xlate(struct irq_domain *d, struct device_node *ctrlr,
	const u32 *intspec, unsigned int intsize,
	unsigned long *out_hwirq, unsigned int *out_type)
{
	if (WARN_ON(intsize != 2))
		return -EINVAL;

	*out_hwirq = intspec[1] ? intspec[1] : 9;
	*out_type = IRQ_TYPE_NONE;
	return 0;
}


static const struct irq_domain_ops bcm_irq_ops = {
	.xlate = bcm_irq_xlate
};


extern struct irq_domain * dde_irq_domain;
static int __init __bcm_irq_init(struct device_node *node, struct device_node *parent)
{
	int irq, i, max = 96;
	dde_irq_domain = irq_domain_add_linear(node, max, &bcm_irq_ops, NULL);

	for (i = 0; i < max; i++) {
		irq = irq_create_mapping(dde_irq_domain, i);
		irq_set_chip_and_handler(irq, &bcm_irq_chip, handle_fasteoi_irq);
		irq_set_probe(irq);
	}

	irq_set_default_host(dde_irq_domain);
	return 0;
}

IRQCHIP_DECLARE(dde_bcm2835, "brcm,bcm2835-armctrl-ic", __bcm_irq_init);


unsigned long loops_per_jiffy = (1<<12);


#include <linux/gfp.h>
#include <linux/slab.h>

unsigned long get_zeroed_page(gfp_t gfp_mask)
{
	void * const ptr = lx_emul_mem_alloc_aligned(PAGE_SIZE, PAGE_SIZE);
	return (unsigned long)lx_emul_virt_to_pages(ptr, 1)->virtual;
}


#include <linux/smp.h>

int smp_call_function_single(int cpu,void (* func)(void * info),void * info,int wait)
{
	func(info);
	return 0;
}


#include <linux/interrupt.h>

extern int irq_activate(struct irq_desc *desc);
extern void enable_fiq(int fiq);
void enable_fiq(int fiq)
{
	struct irq_desc *desc = irq_to_desc(fiq);
	irq_activate(desc);
	enable_irq(fiq);
}
