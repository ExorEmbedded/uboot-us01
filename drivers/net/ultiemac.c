/******************************************************************************
 * (C) Copyright 2010 SITEK
 * All rights reserved.
 *
 * NOTE: Simple single TX buffer + single RX buffer implementation is done.
 ******************************************************************************/

#include <common.h>
#include <net.h>
#include <config.h>
#include <asm/io.h>

#define WriteAddr(b, a) (*((volatile u32 *)(b)) = ((volatile u32)a))
#define WriteAddr16(b, a) (*((volatile u16 *)(b)) = ((volatile u16)a))
#define ReadAddr(a) (*((volatile u32 *)(a)))
#define ReadAddr16(a) (*((volatile u16 *)(a)))

#define GRETH_AUTO_NEGOTIATING_ATTEMPTS 1000
#define PHY_ADDR 1
#define GRETH_PHY_DETECTION_TIMEOUT_MS 100
#define GRETH_WAITBUSY_TIMEOUT_MS 100
#define ULTIEMAC_BUF_SIZE 1514
#define ENET_ADDR_LENGTH 6
#define GRETH_RESET_TIMEOUT_MS 100

#define GRETH_FD 0x10
#define GRETH_SP 0x80
#define GRETH_RESET 0x40
#define GRETH_MII_BUSY 0x8
#define GRETH_MII_NVALID 0x10
#define GRETH_MII_LF 0x04

#define GRETH_BD_EN 0x800
#define GRETH_BD_WR 0x1000
#define GRETH_BD_IE 0x2000
#define GRETH_BD_LEN 0x7FF

#define GRETH_INT_TX 0x8
#define GRETH_TXEN 0x1
#define GRETH_TXI 0x4
#define GRETH_TXBD_STATUS 0xFFFFC000
#define GRETH_TXBD_ERR_UE 0x4000
#define GRETH_TXBD_ERR_AL 0x8000
#define GRETH_TXBD_NUM CYGINT_DEVS_ETH_GRETH_TxNUM
#define GRETH_TXBD_NUM_MASK (GRETH_TXBD_NUM-1)

#define GRETH_INT_RX 0x4
#define GRETH_RXEN 0x2
#define GRETH_RXI 0x8
#define GRETH_RXBD_STATUS 0xFFFFC000
#define GRETH_RXBD_ERR_AE 0x4000
#define GRETH_RXBD_ERR_FT 0x8000
#define GRETH_RXBD_ERR_CRC 0x10000
#define GRETH_RXBD_ERR_OE 0x20000
#define GRETH_RXBD_NUM CYGINT_DEVS_ETH_GRETH_RxNUM
#define GRETH_RXBD_NUM_MASK (GRETH_RXBD_NUM-1)

/* UltiEmac registers */
#define UEMAC_CONTROL (CONFIG_ULTIEMAC_BASE + 0x0000)
#define UEMAC_STATUS  (CONFIG_ULTIEMAC_BASE + 0x0004)
#define UEMAC_MAC_MSB (CONFIG_ULTIEMAC_BASE + 0x0008)
#define UEMAC_MAC_LSB (CONFIG_ULTIEMAC_BASE + 0x000C)
#define UEMAC_MDIO_CTRL (CONFIG_ULTIEMAC_BASE + 0x0010)
#define UEMAC_TX_DESC (CONFIG_ULTIEMAC_BASE + 0x0014)
#define UEMAC_RX_DESC (CONFIG_ULTIEMAC_BASE + 0x0018)
#define UEMAC_INT_MASK (CONFIG_ULTIEMAC_BASE + 0x0020)
/* TX and RX buffers descriptors */
#define UEMAC_RXBD_CFG (CONFIG_ULTIEMAC_BASE + 0x0400)
#define UEMAC_RXBD_ADD (CONFIG_ULTIEMAC_BASE + 0x0404)
#define UEMAC_TXBD_CFG (CONFIG_ULTIEMAC_BASE + 0x0408)
#define UEMAC_TXBD_ADD (CONFIG_ULTIEMAC_BASE + 0x040C)
/* TX and RX buffer offsets */
#define TX_BUFFER_OFFSET 0x00000000
#define RX_BUFFER_OFFSET 0x00001000

/* MAC ADDRESS bytes */
#define MACADDR0 emacaddr[0]
#define MACADDR1 emacaddr[1]
#define MACADDR2 emacaddr[2]
#define MACADDR3 emacaddr[3]
#define MACADDR4 emacaddr[4]
#define MACADDR5 emacaddr[5]

/* Exported function to init MAC address */ 
void ultiemac_setmac(bd_t *);

/* Static helper functions */
static ulong read_mii(ulong addr);
static int test_mii_presence(void);
static void write_mii(ulong addr, ulong data);
static int ultiemac_txbufferavailable(void);

/* Receive buffer to be passed to upper level */
static u32 etherrxbuff[PKTSIZE_ALIGN/4]; /* Receive buffer */

/* MAC address for the UltiEMAC Core */
static u8 emacaddr[ENET_ADDR_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ----------------------------------------------------------------------*
 * Static helper functions
 * ----------------------------------------------------------------------*/

/* returns 1 if TX buffer is available */
static int ultiemac_txbufferavailable(void)
{
  ulong tmp;
  
  tmp = ReadAddr(UEMAC_TXBD_CFG);
  if(tmp & GRETH_BD_EN) return 0;
  return 1;
}


/* Read from the PHY */
static ulong read_mii(ulong addr)
{
  ulong tmp;

  do 
  {
	tmp = ReadAddr(UEMAC_MDIO_CTRL); 
  } while (tmp & GRETH_MII_BUSY);

  tmp = (PHY_ADDR << 11) | ((addr&0x1F) << 6) | 2;
  WriteAddr(UEMAC_MDIO_CTRL, tmp);
  
  do 
  {
	tmp = ReadAddr(UEMAC_MDIO_CTRL); 
  } while (tmp & GRETH_MII_BUSY);

  if (!(tmp & GRETH_MII_NVALID)) 
  {
	tmp = ReadAddr(UEMAC_MDIO_CTRL); 
    debug ("read_mii -> 0x%x    0x%x\n",addr,(tmp>>16)&0xFFFF);
    return (tmp>>16)&0xFFFF;
  }
  else 
  {
    printf("GRETH: failed to read mii\n");
    return (0);
  }
}

/* Check if PHY installed */
static int test_mii_presence(void)
{
  ulong tmp;
  ulong downCounter = GRETH_PHY_DETECTION_TIMEOUT_MS;

#ifndef CONFIG_SCB01  
  do 
  {
	tmp = ReadAddr(UEMAC_MDIO_CTRL); 
  } while (tmp & GRETH_MII_BUSY);

  while (downCounter--)
  {
    tmp = (PHY_ADDR << 11) | ((0&0x1F) << 6) | 2;
 	WriteAddr(UEMAC_MDIO_CTRL, tmp);

    do 
	{
	  tmp = ReadAddr(UEMAC_MDIO_CTRL); 
    } while (tmp & GRETH_MII_BUSY);
	
    if (!(tmp & GRETH_MII_LF)) 
      return (1);
      
    udelay(1000);
  }

  debug("NO PHY detected\n");
  return (0);
#else
  return (1); //success
#endif  
}

/* Write to the PHY */
static void write_mii(ulong addr, ulong data)
{
  uint tmp;
  do 
  {
	tmp = ReadAddr(UEMAC_MDIO_CTRL); 
  } while (tmp & GRETH_MII_BUSY);

  tmp = ((data&0xFFFF)<<16) | (PHY_ADDR << 11) | ((addr&0x1F) << 6) | 1;
  WriteAddr(UEMAC_MDIO_CTRL, tmp);

  do 
  {
	tmp = ReadAddr(UEMAC_MDIO_CTRL); 
  } while (tmp & GRETH_MII_BUSY);
}

/* ----------------------------------------------------------------------*
 * Exported functions for higher level routines
 * ----------------------------------------------------------------------*/

/* eth_halt
 * --------
 * Exported function used to stop the eth controller
 */
void eth_halt (void)
{
	debug ("eth_halt\n");
}

/* ultiemac_setmac
 * ---------------
 * Exported function to set mac address to ULTIEMAC registers
 */
void ultiemac_setmac(bd_t * bis)
{
  	u8 hw_addr[ENET_ADDR_LENGTH];

	if (eth_getenv_enetaddr("ethaddr", hw_addr))
	  memcpy(emacaddr, hw_addr, ENET_ADDR_LENGTH);
 
	WriteAddr(UEMAC_MAC_MSB , MACADDR0 << 8 | MACADDR1);
    WriteAddr(UEMAC_MAC_LSB , MACADDR2 << 24 | MACADDR3 << 16 | MACADDR4 << 8 | MACADDR5);
}

/* eth_init
 * --------
 * Exported function used to initialize and activate the eth controller
 */
int eth_init (bd_t * bis)
{
  unsigned long i, tmp;
  u8 hw_addr[ENET_ADDR_LENGTH];
  
  if (eth_getenv_enetaddr("ethaddr", hw_addr))
	memcpy(emacaddr, hw_addr, ENET_ADDR_LENGTH);

  /* Reset the controller. */
  debug ("reset controller 0x%x\n",UEMAC_CONTROL);
  WriteAddr(UEMAC_CONTROL, GRETH_RESET);
  do 
  {
    tmp = ReadAddr(UEMAC_CONTROL);
  } while (tmp & GRETH_RESET);

  /* Sets TX and RX pointers to the respective descriptors 
	 NOTE: We have only 1 Tx buffer and 1 RX buffer
	 so RX descriptor is at offset 0, tx descriptor is
	 at offset 8. */
  WriteAddr(UEMAC_RX_DESC, (unsigned long)0);
  WriteAddr(UEMAC_TX_DESC, (unsigned long)8);

  /* Test and configure PHY  */
  if (test_mii_presence())
  {
#ifndef CONFIG_SCB01   
    int downCnter = GRETH_RESET_TIMEOUT_MS;
    write_mii(0, 0x8000); /* reset phy */
    while ( (tmp=read_mii(0)) & 0x8000)
    { 
      if (downCnter-- == 0)
        break;
      udelay(1000);
    }
    if (downCnter == -1) printf("greth_device_init: Failed resetting phy\n");
  
    i = 0;
    if (tmp & 0x1000) 
	{ /* autgo neg */
       while ( !(read_mii(1) & 0x20 ) ) 
	   {
         i++;
         if (i>GRETH_AUTO_NEGOTIATING_ATTEMPTS) 
		 {
   	      printf("Auto-negotiation failed (link is down)\n");
   	      break;
         }
		udelay(10000);
	   }
     }
     
     if(i<=GRETH_AUTO_NEGOTIATING_ATTEMPTS) 
	   printf("Auto-negotiation succeeded\n"); 
	 else
	   return 1;
   
     tmp = read_mii(0);
    
     printf("Running %d Mbps %s duplex\n", ((tmp&0x2040) == 0x2000) ? 100:10, (tmp&0x0100) ? "full":"half");
#endif //CONFIG_SCB01 

#ifdef CONFIG_SCB01   
     printf("GRETH(%d) Ethernet MAC at [0x%x] irq %d. Running 100 Mbps full duplex\n", idx, (unsigned int)(gi->regs), (unsigned int)(gi->irq));
#endif //CONFIG_SCB01 

#ifndef CONFIG_SCB01                     
     if (tmp & 0x0100) 
	 {
       /* set full duplex */
	   WriteAddr(UEMAC_CONTROL, GRETH_FD);
     }
#else
	 WriteAddr(UEMAC_CONTROL, GRETH_FD);
#endif        
     
     /* Initialize descriptor buffer addresses. */
	 WriteAddr(UEMAC_RXBD_CFG, (unsigned long)0);
	 WriteAddr(UEMAC_RXBD_ADD, (CONFIG_ULTIEMAC_BUFFER_AMBA + RX_BUFFER_OFFSET));
	 WriteAddr(UEMAC_TXBD_CFG, (unsigned long)0);
	 WriteAddr(UEMAC_TXBD_ADD, (CONFIG_ULTIEMAC_BUFFER_AMBA + TX_BUFFER_OFFSET));

	 /* Write MAC address */
     WriteAddr(UEMAC_MAC_MSB , MACADDR0 << 8 | MACADDR1);
     WriteAddr(UEMAC_MAC_LSB , MACADDR2 << 24 | MACADDR3 << 16 | MACADDR4 << 8 | MACADDR5);
   }
  else
  {
    printf("UltiEmac Init: PHY not detected\n"); 
	return 1;
  }

  /* Now we have to start UltiEmac */
  /* Configure RX descriptor (enable + wrap)*/
  WriteAddr(UEMAC_RXBD_CFG, GRETH_BD_EN | GRETH_BD_IE | GRETH_BD_WR);
 
  /* Clear status register */
  WriteAddr(UEMAC_STATUS , 0xFFFFFFFF);

  /* Activate UltiEmac core */
  tmp = ReadAddr(UEMAC_CONTROL);
  tmp |= GRETH_RXEN | GRETH_TXEN | GRETH_TXI | GRETH_RXI;
  WriteAddr(UEMAC_CONTROL, tmp);
  
  return 0;
}

/* eth_send
 * --------
 * Exported function used to send a packet
 */
int eth_send (volatile void *ptr, int len) 
{
	ulong maxtry = (100l*GRETH_WAITBUSY_TIMEOUT_MS);
	ulong tmp;
	ulong i;
	uchar* src;
	uchar* dst;

	debug ("eth_send    >  %d\n",len);

	if (len > ULTIEMAC_BUF_SIZE)
		len = ULTIEMAC_BUF_SIZE;

	/* Wait for previous TX completion */
	while (!ultiemac_txbufferavailable()) 
	{
		udelay (10);
		maxtry--;
	}

	if (!maxtry) {
		printf ("Error: Timeout waiting for ethernet TX buffer\n");
		return 0;
	}
	
	/* Copy data into TX buffer (little endian to big endian long alignment) */ 
	src = (uchar*)ptr;
	dst = (uchar*)(CONFIG_ULTIEMAC_BUFFER_CPU + TX_BUFFER_OFFSET);
	for(i=0;i<len;i+=4)
	{
	  *(dst+0) = *(src+3);
	  *(dst+1) = *(src+2);
	  *(dst+2) = *(src+1);
	  *(dst+3) = *(src+0);
	  dst+=4;
	  src+=4;
	} 

	/* Activate TX buffer descriptor */
	WriteAddr(UEMAC_TXBD_CFG, GRETH_BD_IE | (len & GRETH_BD_LEN) | GRETH_BD_WR );
	tmp = ReadAddr(UEMAC_TXBD_CFG);
    tmp |= GRETH_BD_EN;
	WriteAddr(UEMAC_TXBD_CFG, tmp);

	/* Activate UltiEmac TX core */
	tmp = ReadAddr(UEMAC_CONTROL);
	tmp |= GRETH_TXEN;
	WriteAddr(UEMAC_CONTROL, tmp);

	return 0;
}

int eth_rx (void)
{
	ulong tmp;
	ulong i;
	ulong pkt_len;
	uchar* src;
	uchar* dst;

	/* Get status of RX buffer, if no RX occurred, reactivate RX engine (just in case) and exit */
	tmp = ReadAddr(UEMAC_RXBD_CFG);
	if(tmp & GRETH_BD_EN) 
	{
	  tmp = ReadAddr(UEMAC_CONTROL);
	  tmp |= GRETH_RXEN | GRETH_RXI;
	  WriteAddr(UEMAC_CONTROL, tmp);
	  return 0;
	}
	pkt_len = tmp & GRETH_BD_LEN;

	/* Copy data into TRX buffer (little endian to big endian long alignment */ 
	dst = (uchar*)etherrxbuff;
	src = (uchar*)(CONFIG_ULTIEMAC_BUFFER_CPU + RX_BUFFER_OFFSET);
	for(i=0;i<pkt_len;i+=4) 
	{
	  *(dst+0) = *(src+3);
	  *(dst+1) = *(src+2);
	  *(dst+2) = *(src+1);
	  *(dst+3) = *(src+0);
	  dst+=4;
	  src+=4;
	} 
	
	/* Rearms RX buffer */
	WriteAddr(UEMAC_RXBD_CFG, GRETH_BD_IE | GRETH_BD_WR);
    tmp = ReadAddr(UEMAC_RXBD_CFG);
	tmp |= GRETH_BD_EN;
	WriteAddr(UEMAC_RXBD_CFG, tmp);
	
	/* Activate UltiEmac RX core */
	tmp = ReadAddr(UEMAC_CONTROL);
	tmp |= GRETH_RXEN;
	WriteAddr(UEMAC_CONTROL, tmp);
	
	debug ("eth_rx    > %d\n",pkt_len);	
	NetReceive ((uchar *) etherrxbuff, pkt_len);
	return 1;
}
