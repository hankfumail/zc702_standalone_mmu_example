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

#include "sleep.h"


#include "xil_types.h"
#include "xil_io.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "xil_exception.h"

#include "Xpseudo_asm.h"

#include "reg_report.h"
#include "Sys_reg_report.h"

/***************** Macros (Inline Functions) Definitions *********************/


/**************************** Type Definitions *******************************/

/************************** Constant Definitions *****************************/

/************************** Variable Definitions *****************************/




/************************** Function Prototypes ******************************/


/************************** Function Implementation ******************************/




/****************************************************************************
*
* Read and report ARM system level register.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void system_control_register_report( void )
{
	int i;
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
	AFE, bit[29]  Access flag enable. The possible values of this bit are:
	0  In the translation table descriptors, AP[0] is an access permissions bit. The full range of
	access permissions is supported. No Access flag is implemented.
	1  In the translation table descriptors, AP[0] is the Access flag. Only the simplified model
	for access permissions is supported.
	Setting this bit to 1 enables use of the AP[0] bit in the translation table descriptors as the Access flag.
	It also restricts access permissions in the translation table descriptors to the simplified model
	described in AP[2:1] access permissions model on page B3-1348.
	In an implementation that includes the Security Extensions, this bit is Banked between the Secure
	and Non-secure copies of the register.
	In an implementation that includes the Virtualization Extensions, when TTBCR.EAE is set to 1, to
	enable use of the Long-descriptor translation table format, this bit is UNK/SBOP.
	 */
	if (CtrlReg & XREG_CP15_CONTROL_TE_BIT)
	{
	    printf("Thumb Exception enabled.\n\r");
	}
	else
	{
	    printf("Thumb Exception not enabled.\n\r");
	}

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
	    printf("Access flag enabled.\n\r");
	}
	else
	{
	    printf("Access flag not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_TRE_BIT)
	{
	    printf("TEX Remap enabled.\n\r");
	}
	else
	{
	    printf("TEX Remap not enabled.\n\r");
	    printf("TEX[2:1] are reassigned for use as flags managed by the operating system..\n\r");
	    printf("The TEX[0], C and B bits are used to describe the memory region attributes, with the MMU remap registers, the PRRR and NMRR.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_NMFI_BIT)
	{
	    printf("Software cannot mask FIQs.\n\r");
	}
	else
	{
	    printf("Software can mask FIQs.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_V_BIT)
	{
	    printf("High exception vectors enabled.\n\r");
	}
	else
	{
	    printf("High exception vectors not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_Z_BIT)
	{
	    printf("Program flow prediction enabled.\n\r");
	}
	else
	{
	    printf("Program flow prediction not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_I_BIT)
	{
	    printf("L1 Instruction Cache enabled.\n\r");
	}
	else
	{
	    printf("L1 Instruction Cache not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_C_BIT)
	{
	    printf("L1 Data Cache enabled.\n\r");
	}
	else
	{
	    printf("L1 Data Cache not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_A_BIT)
	{
	    printf("Alignment fault checking enabled.\n\r");
	}
	else
	{
	    printf("Alignment fault checking not enabled.\n\r");
	}

	if (CtrlReg & XREG_CP15_CONTROL_M_BIT)
	{
	    printf("MMU enabled.\n\r");
	}
	else
	{
	    printf("MMU not enabled.\n\r");
	}


	/* B3.7 Memory access control  */
	/* AP[2:0] access permissions control, Short-descriptor format only
	   This access permissions model applies when using the Short-descriptor translation tables format, and the
	   SCTLR.AFE bit is set to 0. Table B3-8 shows this access permissions model.
	 */
	//mtcp(XREG_CP15_DOMAIN_ACCESS_CTRL, 0x55555555);


	/* Read domain access control register */
#ifdef __GNUC__
	CtrlReg = mfcp(XREG_CP15_DOMAIN_ACCESS_CTRL);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_DOMAIN_ACCESS_CTRL);
	  CtrlReg = Reg; }
#endif

	printf("Domain access control register: 0x%08x.\n\r", CtrlReg);
	for( i=0; i<16; i++ )
	{
		if( 0x00 == ( (CtrlReg>>(i*2))&0x3) )
		{
		    printf("No access to domain %d.\n\r", i);
		}
		else if( 0x01 == ( (CtrlReg>>(i*2))&0x3) )
		{
		    printf("Client: access to domain %d is dependant on TLB entry.\n\r", i);
		}
		else if( 0x00 == ( (CtrlReg>>(i*2))&0x3) )
		{
		    printf("Reserved for domain %d.\n\r", i);
		}
		else
		{
		    printf("Manager: full access to domain %d.\n\r", i);
		}
	}



}




/****************************************************************************
*
* Read and report Multiprocessor Affinity Register.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void Xil_Multiprocessor_Affinity_read( void )
{
	register unsigned int CtrlReg;

	/* enable caches only if they are disabled */
#ifdef __GNUC__
	CtrlReg = mfcp(XREG_CP15_MULTI_PROC_AFFINITY);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_MULTI_PROC_AFFINITY);
	  CtrlReg = Reg; }
#endif

	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 *	Access flag not enabled.
	 */
    printf("Multiprocessor Affinity Register: 0x%08x\n\r", CtrlReg);

	/*
	U bit Multiprocessing Extensions:
	 */
	if ((CtrlReg>>30) & 1 )
	{
	    printf("processor is a uniprocessor.\n\r");
	}
	else
	{
	    printf(" processor is part of an MPCore cluster.\n\r");
	}


	/* Cluster ID Value read in CLUSTERID configuration pins
	 */
	//if ((CtrlReg>>8) & (0xf) )
	{
		printf("Cluster ID:%d.\n\r", ((CtrlReg>>8) & (0xf) ));
	}

 
	/*  CPU ID Indicates the CPU number in the Cortex-A9 MPCore configuration:
	 */
	//if ( (CtrlReg>>8) & (0xf) )
	{
		printf("CPU ID:%d.\n\r", ((CtrlReg>>0) & (0x3) ));
	}


}






/****************************************************************************
*
* Read and report Auxiliary Control Register.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void Xil_Auxiliary_Control_Register_read( void )
{
	register unsigned int CtrlReg;

	/* enable caches only if they are disabled */
#ifdef __GNUC__
	CtrlReg = mfcp(XREG_CP15_AUX_CONTROL);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_AUX_CONTROL);
	  CtrlReg = Reg; }
#endif

	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 *	Access flag not enabled.
	 */
    printf("Auxiliary Control Register: 0x%08x\n\r", CtrlReg);

	/*
	Support for parity checking
	 */
	if ((CtrlReg>>9) & 1 )
	{
	    printf("parity checking Enabled.\n\r");
	}
	else
	{
	    printf("parity checking Disabled.\n\r");
	}


	/* Enable allocation in one cache way only
	 */
	if ((CtrlReg>>8) & (1) )
	{
		printf("Enable allocation in one cache way only Enabled.\n\r");
	}
	else
	{
		printf("Enable allocation in one cache way only Disabled.\n\r");
	}

 
	/*  The exclusive cache configuration does not permit data to reside in L1 and L2 at the same time.
	 */
	if ( (CtrlReg>>7) & (1) )
	{
		printf("The exclusive cache configuration Enabled.\n\r");
	}
	else
	{
		printf("The exclusive cache configuration Disabled.\n\r");
	}
 
	/*  SMP: Signals if the Cortex-A9 processor is taking part in coherency or not.
	 */
	if ( (CtrlReg>>6) & (1) )
	{
		printf("SMP Enabled.\n\r");
	}
	else
	{
		printf("SMP Disabled.\n\r");
	}
 
	/*  Enable write full line of zeros mode.
	 */
	if ( (CtrlReg>>3) & (1) )
	{
		printf("Write full line of zeros mode Enabled.\n\r");
	}
	else
	{
		printf("Write full line of zeros mode Disabled.\n\r");
	}
 
	/*  L1 prefetch enable
	 */
	if ( (CtrlReg>>2) & (1) )
	{
		printf("L1 prefetch Enabled.\n\r");
	}
	else
	{
		printf("L1 prefetch Disabled.\n\r");
	}
 
	/*  L2 prefetch enable
	 */
	if ( (CtrlReg>>1) & (1) )
	{
		printf("L2 prefetch Enabled.\n\r");
	}
	else
	{
		printf("L2 prefetch Disabled.\n\r");
	}
 
	/*  Cache and TLB maintenance broadcast
	 */
	if ( (CtrlReg>>0) & (1) )
	{
		printf("Cache and TLB maintenance broadcast Enabled.\n\r");
	}
	else
	{
		printf("Cache and TLB maintenance broadcast Disabled.\n\r");
	}
 
	/*  
	if ( (CtrlReg>>) & (1) )
	{
		printf(" Enabled.\n\r");
	}
	else
	{
		printf(" Disabled.\n\r");
	}
	*/

}



/****************************************************************************
*
* Read and report  register.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void Xil_cpsr_read( void )
{
	register unsigned int mpsr;

	mpsr = mfcpsr();


	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 *	Access flag not enabled.
	 */
    printf("mpsr: 0x%08x\n\r", mpsr );

	/*
	if ( (CtrlReg>>) & (1) )
	{
		printf(" Enabled.\n\r");
	}
	else
	{
		printf(" Disabled.\n\r");
	}
	*/

}



