/*
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Contains definitions required for accessing the Omap DMA4 resources (ch0)
 * Created by: Giovanni Pavoni Sitek S.p.a.
 */

#ifndef DMA4_H
#define DMA4_H

/* DMA4 CH0 registers */
#define DMA4_GCR				0x48056078
#define DMA4_CSR0				0x4805608C
#define DMA4_CSDP0				0x48056090
#define DMA4_CEN0				0x48056094
#define DMA4_CFN0				0x48056098
#define DMA4_CSSA0				0x4805609C
#define DMA4_CDSA0				0x480560A0
#define DMA4_CSEl0				0x480560A4
#define DMA4_CSFl0				0x480560A8
#define DMA4_CDEl0				0x480560AC
#define DMA4_CDFl0				0x480560B0
#define DMA4_CCR0				0x48056080
#define DMA4_OCP_SYSCONFIG		0x4805602C


static inline void dma4_write_reg(int reg, u32 val)
{
       __raw_writel(val, reg);
}

static inline u32 dma4_read_reg(int reg)
{
       u32 l = __raw_readl(reg);
       return l;
}

#endif /* DMA4_H */
