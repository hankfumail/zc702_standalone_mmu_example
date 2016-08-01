/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */


#include <stdio.h>

//#include "sleep.h"


#include "xil_types.h"
#include "xil_io.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "xil_exception.h"

#include "Xpseudo_asm.h"

#include "mmu_table.h"
#include "dbg_print.h"


/***************** Macros (Inline Functions) Definitions *********************/


/**************************** Type Definitions *******************************/

/************************** Constant Definitions *****************************/

/************************** Variable Definitions *****************************/

extern u32 MMUTable;
u32 g_u32TEXRemapEnable=0;
u32 g_u32AFEEnable=0;


/************************** Function Prototypes ******************************/


/************************** Function Implementation ******************************/

/*
	Please set XREG_CP15_DOMAIN_ACCESS_CTRL if memory proctection is used.
	Default XREG_CP15_DOMAIN_ACCESS_CTRL make MMU has no efffect.
	mtcp(XREG_CP15_DOMAIN_ACCESS_CTRL, 0x55555555);
*/

void SetMmuTblCacheAttributes(u32 addr, u32 u32Inner, u32 u32Outer, u32 u32Share, u32 u32StronglyOrdered, u32 u32Device )
{
	u32 *ptr;
	u32 section;
	u32 attributes_old;
	u32 attributes_new;

	/*
	 B3.7.2 C, B, and TEX[2:0] encodings without TEX remap
	Table B3-7 shows the C, B, and TEX[2:0] encodings when TEX remap is disabled (TRE == 0).
	TEX[2:0] : 14:12 bits
	C   :  3 bit
	B   : 2 bit
	
	 00 Non-cacheable
	 01 Write-Back, Write-Allocate
	 10 Write-Through, no Write-Allocate
	 11 Write-Back, no Write-Allocate

	 */
	 
	section = addr / 0x100000;
	ptr = &MMUTable + section;
	
	attributes_old = (*ptr)& 0x000FFFFF;
	attributes_old	= attributes_old &( ~( (1<<16)|(7<<12)|(1<<3)|(1<<2)) );

	if( 1 == u32StronglyOrdered )
	{
		/*
		TEX[2:0] : 14:12 bits :  0 
		C	:  3 bit   :  0 
		B	: 2 bit    :  0 
		 */
		attributes_new  = attributes_old;
		dbg_print("Shareable Strongly Ordered Memory.\n\r");
	}
	else if( 1 == u32Device )
	{
		if( 1 == u32Share )
		{
			/*
			TEX[2:0] : 14:12 bits :  0 
			C	:  3 bit   :  0 
			B	: 2 bit    :  1 
			 */
			attributes_new	= attributes_old|(1<<2);
			dbg_print("Shareable Device Memory.\n\r");
		}
		else 
		{
			/*
			TEX[2:0] : 14:12 bits :  010 
			C	:  3 bit   :  0 
			B	: 2 bit    :  0 
			 */
			attributes_new	= attributes_old|(2<<12);
			dbg_print("Non-Shareable Device Memory.\n\r");
		}
	}
	else 
	{
		/* Normal memory */
		if( 1 == u32Share )
		{
			dbg_print("Shareable ");
		}
		else 
		{
			dbg_print("Non-Shareable ");
		}
		//printf("Normal Memory, Outer:%ld, Inner:%ld.\n\r", u32Outer, u32Inner);
		
		/*
		TEX[2:0] : 14:12 bits :  1BB 
		C	:  3 bit   :  A 
		B	: 2 bit    :  A 
		TEX[1:0]  Define the Outer cache attribute.
		C, B  Define the Inner cache attribute.		
		 */
		attributes_new	= attributes_old|((u32Share&0x1)<<16)|(1<<14)|((u32Outer&0x3)<<12)|((u32Inner&0x3)<<2);
	}
	

	Xil_SetTlbAttributes( addr, attributes_new );
	dsb();
	dsb();
	mtcp(XREG_CP15_INVAL_UTLB_UNLOCKED, 0);
	mtcp(XREG_CP15_INVAL_BRANCH_ARRAY, 0);
}

/*
	Please set XREG_CP15_DOMAIN_ACCESS_CTRL if memory proctection is used.
	Default XREG_CP15_DOMAIN_ACCESS_CTRL make MMU has no efffect.
	mtcp(XREG_CP15_DOMAIN_ACCESS_CTRL, 0x55555555);
*/


void SetMmuTblCacheAndAccessAttributes
(
	u32 addr, 
	u32 u32Inner, 
	u32 u32Outer, 
	u32 u32Share, 
	u32 u32StronglyOrdered, 
	u32 u32Device,
	u32 u32AccessFlag 
)
{
	u32 *ptr;
	u32 section;
	u32 attributes_old;
	u32 attributes_new;
	u32 u32AP2=0;
	u32 u32AP10=0;

	/*
	 B3.7.2 C, B, and TEX[2:0] encodings without TEX remap
	Table B3-7 shows the C, B, and TEX[2:0] encodings when TEX remap is disabled (TRE == 0).
	TEX[2:0] : 14:12 bits
	C   :  3 bit
	B   : 2 bit
	
	 00 Non-cacheable
	 01 Write-Back, Write-Allocate
	 10 Write-Through, no Write-Allocate
	 11 Write-Back, no Write-Allocate
	 */

	/*
	0 00     Privileged: No access        User: No access All accesses generate Permission faults
	0 01     Privileged: Read/write      User: No access Privileged access only
	0 10     Privileged: Read/write      User: Read-only Writes in User mode generate Permission faults
	0 11     Privileged: Read/write      User: Read/write Full access
	1 00     - - Reserved

	1 01     Privileged: Read-only      User: No access Privileged read-only
	1 10     Privileged: Read-only      User: Read-only Privileged and User read-only, deprecated in VMSAv7a
	1 11     Privileged: Read-only      User: Read-only Privileged and User read-onlyb
	*/
	
	u32AP2 = (u32AccessFlag>>2)&0x1;
	u32AP10 = u32AccessFlag&0x3;;
	 
	section = addr / 0x100000;
	ptr = &MMUTable + section;
	
	attributes_old = (*ptr)& 0x000FFFFF;
	attributes_old	= attributes_old &( ~( (1<<16)|(7<<12)|(1<<3)|(1<<2)) );

	if( 1 == u32StronglyOrdered )
	{
		/*
		TEX[2:0] : 14:12 bits :  0 
		C	:  3 bit   :  0 
		B	: 2 bit    :  0 
		 */
		attributes_new  = attributes_old;
		dbg_print("Shareable Strongly Ordered Memory.\n\r");
	}
	else if( 1 == u32Device )
	{
		if( 1 == u32Share )
		{
			/*
			TEX[2:0] : 14:12 bits :  0 
			C	:  3 bit   :  0 
			B	: 2 bit    :  1 
			 */
			attributes_new	= attributes_old|(1<<2);
			dbg_print("Shareable Device Memory.\n\r");
		}
		else 
		{
			/*
			TEX[2:0] : 14:12 bits :  010 
			C	:  3 bit   :  0 
			B	: 2 bit    :  0 
			 */
			attributes_new	= attributes_old|(2<<12);
			dbg_print("Non-Shareable Device Memory.\n\r");
		}
	}
	else 
	{
		/* Normal memory */
		if( 1 == u32Share )
		{
			dbg_print("Shareable ");
		}
		else 
		{
			dbg_print("Non-Shareable ");
		}
		dbg_print("Normal Memory, Outer:%ld, Inner:%ld.\n\r", u32Outer, u32Inner);
		
		/*
		TEX[2:0] : 14:12 bits :  1BB 
		C	:  3 bit   :  A 
		B	: 2 bit    :  A 
		TEX[1:0]  Define the Outer cache attribute.
		C, B  Define the Inner cache attribute.		
		 */
		attributes_new	= attributes_old|((u32Share&0x1)<<16)|(1<<14)|((u32Outer&0x3)<<12)|((u32Inner&0x3)<<2);
	}
	

	/*
	0 00     Privileged: No access        User: No access All accesses generate Permission faults
	0 01     Privileged: Read/write      User: No access Privileged access only
	0 10     Privileged: Read/write      User: Read-only Writes in User mode generate Permission faults
	0 11     Privileged: Read/write      User: Read/write Full access
	1 00     - - Reserved

	1 01     Privileged: Read-only      User: No access Privileged read-only
	1 10     Privileged: Read-only      User: Read-only Privileged and User read-only, deprecated in VMSAv7a
	1 11     Privileged: Read-only      User: Read-only Privileged and User read-onlyb
	*/

	attributes_new &= (~((0x3<<10)|(0x1<<15)));
	attributes_new |= (u32AP10<<10);
	attributes_new |= (u32AP2<<15);
	dbg_print_info_var_hex_non_zero( "New MMU table attribute value:", attributes_new );


	Xil_SetTlbAttributes( addr, attributes_new );
	dbg_print("The attribute is 0x%08x for address: 0x%08x.\n\r\n\r", attributes_new, addr);
	Xil_DCacheFlushRange( (unsigned int)ptr&0xffffffe0, 64);

	dsb();
	dsb();
	mtcp(XREG_CP15_INVAL_UTLB_UNLOCKED, 0);
	mtcp(XREG_CP15_INVAL_BRANCH_ARRAY, 0);
}


void ReportMmuTblAttributes( u32 addr )
{
	volatile u32 *ptr;
	u32 section;
	u32 attributes;
	u32 u32NormalSection=0;
	u32 u32C=0;
	u32 u32B=0;
	u32 u32CB=0;
	u32 u32Tex=0;
	u32 u32TexBit01=0;
	u32 u32S=0;
	u32 u32AP2=0;
	u32 u32AP10=0;
	
	u32 CtrlReg=0;
	u32 u32TEXRemapEnable=0;
	u32 u32AFEEnable=0;

	
	/* enable caches only if they are disabled */
#ifdef __GNUC__
	CtrlReg = mfcp(XREG_CP15_SYS_CONTROL);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_SYS_CONTROL);
	  CtrlReg = Reg; }
#endif
	if (CtrlReg & XREG_CP15_CONTROL_AFE_BIT)
	{
		u32AFEEnable=1;
	}
	else
	{
		u32AFEEnable=0;
	}

	if (CtrlReg & XREG_CP15_CONTROL_TRE_BIT)
	{
		u32TEXRemapEnable =1;
	}
	else
	{
		u32TEXRemapEnable =0;
	}


	section = addr / 0x100000;
	ptr = &MMUTable + section;

	
	//ReadSysControlMmuSetting( );

	attributes = *ptr;
	printf("\n\rReport MMU TLB setting\n\r");
	printf("MMU TLB base address: 0x%p\n\r", &MMUTable);
	printf("MMU TLB setting: 0x%08lx for address: 0x%08lx\n\r", attributes, addr );

	/*
	 B3.7.2 C, B, and TEX[2:0] encodings without TEX remap
	Table B3-7 shows the C, B, and TEX[2:0] encodings when TEX remap is disabled (TRE == 0).

	B3.7.3 Memory region attribute descriptions when TEX remap is enabled
	However, many system software implementations do not need to use all of
	these options simultaneously. Instead a smaller subset of attributes can be enabled. This alternative
	functionality is called TEX remap, and permits software to hold software-interpreted values in the
	translation tables. When TEX remap is enabled
	
	 */

	
	/*
	B3.5.1 Short-descriptor translation table format descriptors 
	Short-descriptor translation table first-level descriptor formats
	Each entry in the first-level table describes the mapping of the associated 1MB MVA range.
	*/
	if( 0 == (attributes&0x3) )
	{
		printf("Incorrect MMU TLB setting\n\r" );
	} 
	else if( 1 == (attributes&0x3) )
	{
		printf("Page table\n\r" );
	} 
	else 
	{
		/*
		0b10, Section or Supersection 
		The descriptor gives the base address of the Section or Supersection. Bit[18] determines whether 
		the entry describes a Section or a Supersection.
		If the implementation supports the PXN attribute, this encoding also defines the PXN bit as 0.
		*/
		
		if( 0 == (attributes&0x1) )
		{
			printf("No address extension.\n\r" );
		} 
		else
		{
			printf("Address extension.\n\r" );
		} 


		if( 0 == (attributes&(1<<18)) )
		{
			u32NormalSection=1;
			printf("Normal section\n\r" );
		} 
		else 
		{
			printf("Super section\n\r" );
		} 
	} 

	if( 1 == u32NormalSection )
	{
		u32NormalSection=1;
		printf("        C:%lx\n\r", ( (attributes>>3)&1) );
		printf("        B:%lx\n\r", ( (attributes>>2)&1) );
		printf("       XN:%lx, The Execute-never bit.\n\r", ( (attributes>>4)&1) );
		printf("   Domain:%lx\n\r", ( (attributes>>5)&15) );
		printf("  AP[1:0]:%lx\n\r", ( (attributes>>10)&3) );
		printf(" TEX[2:0]:%lx\n\r", ( (attributes>>12)&7) );
		printf("    AP[2]:%lx\n\r", ( (attributes>>15)&1) );
		printf("        S:%lx, The Shareable bit. \n\r", ( (attributes>>16)&1) );
		printf("       nG:%lx, The not global bit\n\r", ( (attributes>>17)&1) );
		printf("       nS:%lx, Non-secure bit\n\r", ( (attributes>>19)&1) );
		u32C=( (attributes>>3)&1);
		u32B=( (attributes>>2)&1);
		u32CB = (u32C<<1)+u32B;
		u32Tex=( (attributes>>12)&7);
		u32TexBit01 =( (attributes>>12)&3);		
		u32S=( (attributes>>16)&1);
		
		printf("TEX[2:0], C, B: Memory region attribute bits, see Memory region attributes on page B3-1357.\n\r" );
		printf("AP[2], AP[1:0] Access Permissions bits, see Memory access control on page B3-1347.\n\r" );
		printf("AP[0] can be configured as the Access flag, see The Access flag on page B3-1353..\n\r" );
		
		if( 0 == u32TEXRemapEnable )
		{
			/*****************************************************/
			if( ( 0 == u32Tex ) && ( 0 == u32C ) && ( 0 == u32B ) )
			{
				printf("Strongly-ordered Memory, Shareable\n\r" );
			}

			if( ( 0 == u32Tex ) && ( 0 == u32C ) && ( 1 == u32B ) )
			{
				printf("Shareable Device, Device Memory, Shareable\n\r" );
			}

			if( ( 0 == u32Tex ) && ( 1 == u32C ) && ( 0 == u32B ) && ( 0 == u32S ) )
			{
				printf("Outer and Inner Write-Through, no Write-Allocate, Normal Memory, Non-Shareable\n\r" );
			}

			if( ( 0 == u32Tex ) && ( 1 == u32C ) && ( 0 == u32B ) && ( 1 == u32S ) )
			{
				printf("Outer and Inner Write-Through, no Write-Allocate, Normal Memory, Shareable\n\r" );
			}

			if( ( 0 == u32Tex ) && ( 1 == u32C ) && ( 1 == u32B ) && ( 0 == u32S ) )
			{
				printf("Outer and Inner Write-Back, no Write-Allocate, Normal Memory, Non-Shareable\n\r" );
			}

			if( ( 0 == u32Tex ) && ( 1 == u32C ) && ( 1 == u32B ) && ( 1 == u32S ) )
			{
				printf("Outer and Inner Write-Back, no Write-Allocate, Normal Memory, Shareable\n\r" );
			}

			/*****************************************************/
			if( ( 1 == u32Tex ) && ( 0 == u32C ) && ( 0 == u32B ) && ( 0 == u32S ) )
			{
				printf("Outer and Inner Non-cacheable, Normal Memory, Non-Shareable.\n\r" );
			}
			
			if( ( 1 == u32Tex ) && ( 0 == u32C ) && ( 0 == u32B ) && ( 1 == u32S ) )
			{
				printf("Outer and Inner Non-cacheable, Normal Memory, Shareable.\n\r" );
			}
			
			if( ( 1 == u32Tex ) && ( 0 == u32C ) && ( 1 == u32B ) )
			{
				printf("Incorrect setting.\n\r" );
			}
			
			if( ( 1 == u32Tex ) && ( 1 == u32C ) && ( 0 == u32B ) )
			{
				printf("IMPLEMENTATION DEFINED.\n\r" );
			}
			
			if( ( 1 == u32Tex ) && ( 1 == u32C ) && ( 1 == u32B ) && ( 0 == u32S ) )
			{
				printf("Outer and Inner Write-Back, Write-Allocate, Normal Memory, Non-Shareable.\n\r" );
			}
			
			if( ( 1 == u32Tex ) && ( 1 == u32C ) && ( 1 == u32B ) && ( 1 == u32S ) )
			{
				printf("Outer and Inner Write-Back, Write-Allocate, Normal Memory, Shareable.\n\r" );
			}


			/*****************************************************/
			if( ( 2 == u32Tex ) && ( 0 == u32C ) && ( 0 == u32B ) )
			{
				printf("Non-shareable Device, Device Memory. \n\r" );
			}

			if( ( 2 == u32Tex ) && ( 0 == u32C ) && ( 1 == u32B ) )
			{
				printf("Reserved. \n\r" );
			}

			if( ( 2 == u32Tex ) && ( 1 == u32C ) )
			{
				printf("Reserved. \n\r" );
			}

			/*****************************************************/
			if( ( 3 == u32Tex ) )
			{
				printf("Reserved. \n\r" );
			}

			/*****************************************************/
			if(  1 == ((u32Tex>>2)&1)  )
			{
				printf("Cacheable Normal memory:    " );

				/*
				Shareability and the S bit, without TEX remap
				The translation table entries also include an S bit. This bit:
				 Is ignored if the entry refers to Device or Strongly-ordered memory.
				 For Normal memory, determines whether the memory region is Shareable or Non-shareable:
				S == 0  Normal memory region is Non-shareable.
				S == 1  Normal memory region is Shareable.
				*/
				if( 0 == u32S )
				{
					printf("Non-Shareable.\n\r" );
				}
				else 
				{
					printf("Shareable.\n\r" );
				}

				if( 0 == u32TexBit01 )
				{
					printf("Outer cache attribute: Non-cacheable. \n\r" );
				}
				else if( 1 == u32TexBit01 )
				{
					printf("Outer cache attribute: Write-Back, Write-Allocate. \n\r" );
				}
				else if( 2 == u32TexBit01 )
				{
					printf("Outer cache attribute: Write-Through, no Write-Allocate. \n\r" );
				}
				else 
				{
					printf("Outer cache attribute: Write-Back, no Write-Allocate\n\r" );
				} 

				if( 0 == u32CB )
				{
					printf("Inner cache attribute: Non-cacheable. \n\r" );
				}
				else if( 1 == u32CB )
				{
					printf("Inner cache attribute: Write-Back, Write-Allocate. \n\r" );
				}
				else if( 2 == u32CB )
				{
					printf("Inner cache attribute: Write-Through, no Write-Allocate. \n\r" );
				}
				else 
				{
					printf("Inner cache attribute: Write-Back, no Write-Allocate\n\r" );
				} 

			}



			/*
				Access permission bits in a translation table descriptor control access to the corresponding memory region. 
				The Short-descriptor translation table format supports two options for defining the access permissions:
						 three bits, AP[2:0], define the access permissions
						 two bits, AP[2:1], define the access permissions, and AP[0] can be used as an Access flag.
					SCTLR.AFE selects the access permissions option. Setting this bit to 1, to enable the Access flag, also selects use 
					of AP[2:1] to define access permissions. 

       			ARM MMU: Section mode
       			AP[2], bit 15
       			AP[1:0]  bit 11:10
       			Access Permissions bits, see Memory access control on page B3-1347.
       			AP[0] can be configured as the Access flag, see The Access flag on page B3-1353.
       			These bits are not present in a Page table entry.
       
       			AP[2] AP[1:0] PL1 and PL2 access Unprivileged access Description
       			0 00 No access No access All accesses generate Permission faults
              			01 Read/write No access Access only at PL1 or higher
              			10 Read/write Read-only Writes at PL0 generate Permission faults
              			11 Read/write Read/write Full access
       			
       			1 00 - - Reserved
              			01 Read-only No access Read-only, only at PL1 or higher
              			10 Read-only Read-only Read-only at any privilege level, deprecateda
              			11 Read-only Read-only Read-only at any privilege levelb
			*/
			u32AP2=( (attributes>>15)&1);
			u32AP10=( (attributes>>10)&3);
			if( 0 == u32AFEEnable )
			{
				if( 0 == u32AP2 )
				{
					if( 0 == u32AP10 )
					{
						printf("PL1 and PL2 access: No access, Unprivileged access: No access. \n\r" );
					}
					else if( 1 == u32AP10 )
					{
						printf("PL1 and PL2 access: Read/write, Unprivileged access: No access. \n\r" );
					}
					else if( 2 == u32AP10 )
					{
						printf("PL1 and PL2 access: Read/write, Unprivileged access: Read-only. \n\r" );
					}
					else 
					{
						printf("PL1 and PL2 access: Read/write, Unprivileged access: Read/write. \n\r" );
					} 
				}
				else 
				{
					if( 0 == u32AP10 )
					{
						printf("Reserved.\n\r" );
					}
					else if( 1 == u32AP10 )
					{
						printf("PL1 and PL2 access: Read-only, Unprivileged access: No access. \n\r" );
					}
					else if( 2 == u32AP10 )
					{
						printf("PL1 and PL2 access: Read-only, Unprivileged access: Read-only. \n\r" );
					}
					else 
					{
						printf("PL1 and PL2 access: Read-only, Unprivileged access: Read-only. \n\r" );
					} 
				}
			}
			else 
			{
				printf("Simplified access permissions model\n\r" );
				printf("AP[0] is access flag.\n\r" );
				printf("when an Access Flag fault occurs, software sets the access flag to 1 in the translation table entry.\n\r" );
				printf("Software does not have to flush the entry from the TLB after setting the flag.\n\r" );
				
				u32AP10 = (u32AP10>>1)&0x1;
				if( 0 == u32AP2 )
				{
					if( 0 == u32AP10 )
					{
						printf("Kernel (privileged) read/write.\n\r" );
					}
					else 
					{
						printf("User read/writen\r" );
					} 
				}
				else 
				{
					if( 0 == u32AP10 )
					{
						printf("Kernel (privileged) read-only\n\r" );
					}
					else 
					{
						printf("User read-onl\n\r" );
					} 
				} 
				
			}
	

	
		}
		else
		{
			printf("TEX Remap enabled.\n\r");
		} 

	} 



}



/****************************************************************************
*
* Read  ARM system level register's MMU bit.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void ReadSysControlMmuSetting( void )
{
	register unsigned int CtrlReg;

	/* enable caches only if they are disabled */
#ifdef __GNUC__
	CtrlReg = mfcp(XREG_CP15_SYS_CONTROL);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_SYS_CONTROL);
	  CtrlReg = Reg; }
#endif

	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 *	Access flag not enabled.
	 */
    printf("SCTLR: 0x%08x\n\r", CtrlReg);


	/*
	The Large Physical Address Extension adds a new translation table format:
	the format used in an implementation that does not include the Large Physical Address Extension is now 
		called the Short-descriptor format, see Short-descriptor translation table format on page B3-1315
	the format added by the Large Physical Address Extension is the Long-descriptor format, see 
		Long-descriptor translation table format on page B3-1329.
	
	Access permission bits in a translation table descriptor control access to the corresponding memory region. 
	The Short-descriptor translation table format supports two options for defining the access permissions:
			 three bits, AP[2:0], define the access permissions
			 two bits, AP[2:1], define the access permissions, and AP[0] can be used as an Access flag.
		SCTLR.AFE selects the access permissions option. Setting this bit to 1, to enable the Access flag, also selects use 
		of AP[2:1] to define access permissions. 
	The Long-descriptor translation table format uses only AP[2:1] to control the access permissions, and provides an 
	AF bit for use as an Access flag. This means the VMSA behaves as if SCTLR.AFE is set to 1, regardless of the value 
	that software has written to this bit.

	*/
	if (CtrlReg & XREG_CP15_CONTROL_AFE_BIT)
	{
		g_u32AFEEnable=1;
	    printf("Access flag enabled. \n\r");
	    printf("The access permissions information in the translation table descriptors is limited to the AP[2:1] bits\n\r");
	}
	else
	{
		g_u32AFEEnable=0;
	    printf("Access flag not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_TRE_BIT)
	{
	    printf("TEX Remap enabled.\n\r");
		g_u32TEXRemapEnable =1;
	}
	else
	{
		g_u32TEXRemapEnable =0;
	    printf("TEX Remap not enabled.\n\r");
	    printf("TEX[2:1] are reassigned for use as flags managed by the operating system..\n\r");
	    printf("The TEX[0], C and B bits are used to describe the memory region attributes, with the MMU remap registers, the PRRR and NMRR.\n\r");
	}

}


