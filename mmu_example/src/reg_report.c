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

#include "reg_report.h"
#include "dbg_print.h"

/***************** Macros (Inline Functions) Definitions *********************/

/*
 * SLCR registers
 */
#define SLCR_LOCK	0xF8000004 /**< SLCR Write Protection Lock */
#define SLCR_UNLOCK	0xF8000008 /**< SLCR Write Protection Unlock */
#define FPGA_RST_CTRL	0xF8000240 /**< FPGA software reset control */

#define SLCR_LOCK_VAL	0x767B
#define SLCR_UNLOCK_VAL	0xDF0D


/**************************** Type Definitions *******************************/



/************************** Constant Definitions *****************************/



/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/



/************************** Function Implementation ******************************/










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
void register_report( u32 u32_reg_value, char *ps_reg_name, struct reg_defintion *p_reg_info  )
{
	int		i;
	u32 	u32_temp;

	for( i=0; i<1000; i++)
	{
		if( ( 0xffffffff == p_reg_info[i].type ) 
			&&( 0xffffffff == p_reg_info[i].mask ) 
			&& ( 0xffffffff == p_reg_info[i].value ) )
		{
			break;
		}

		//u32_temp = (u32_reg_value>>p_reg_info[i].shift_right_num)&u32_reg_value&p_reg_info[i].mask;
		u32_temp = (u32_reg_value>>p_reg_info[i].shift_right_num)&p_reg_info[i].mask;
		if( REG_BITS_TYPE_SHOW_DEC == p_reg_info[i].type )
		{
			printf("%s register %s: %ld.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_02HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%02lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_04HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%04lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_08HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%08lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_CHECK == p_reg_info[i].type )
		{
			if( u32_temp == p_reg_info[i].value )
			{
				printf("%s register %s: %s.\n\r", ps_reg_name, p_reg_info[i].name, p_reg_info[i].info );
			}
		}
	
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
void register2_report( u32 u32_reg_value, char *ps_reg_name, struct reg_defintion2 *p_reg_info  )
{
	int		i;
	u32 	u32_temp_b;
	u32 	u32_temp;

	for( i=0; i<1000; i++)
	{
		if( ( 0xffffffff == p_reg_info[i].type ) 
			&&( 0xffffffff == p_reg_info[i].a_mask ) 
			&& ( 0xffffffff == p_reg_info[i].value ) )
		{
			break;
		}

		u32_temp = (u32_reg_value>>p_reg_info[i].a_shift_right_num)&p_reg_info[i].a_mask;
		u32_temp_b = (u32_reg_value>>p_reg_info[i].b_shift_right_num)&p_reg_info[i].b_mask;
		u32_temp |=u32_temp_b;
		if( REG_BITS_TYPE_SHOW_DEC == p_reg_info[i].type )
		{
			printf("%s register %s: %ld.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_02HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%02lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_04HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%04lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_SHOW_08HEX == p_reg_info[i].type )
		{
			printf("%s register %s: 0x%08lx.\n\r", ps_reg_name, p_reg_info[i].name, u32_temp);
		}
		else if( REG_BITS_TYPE_CHECK == p_reg_info[i].type )
		{
			if( u32_temp == p_reg_info[i].value )
			{
				printf("%s register %s: %s.\n\r", ps_reg_name, p_reg_info[i].name, p_reg_info[i].info );
			}
		}
	
	}


}


void register_read_report( u32 u32_reg_addr, char *ps_reg_name, struct reg_defintion *p_reg_info  )
{
	u32 u32_reg_value;

	u32_reg_value = Xil_In32( u32_reg_addr );
	register_report( u32_reg_value, ps_reg_name, p_reg_info  );
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
void auxiliary_control_register_report( void )
{
	register unsigned int u32_aux_ctrl_reg;


	struct reg_defintion   aux_ctrl_reg_definition[] =
	{
		{  REG_BITS_TYPE_CHECK,  9,  1,  1, "Parity on", "Parity checking enabled" },
		{  REG_BITS_TYPE_CHECK,  8,  1,  1, "Alloc in one way", "Enable allocation in one cache way only." },
		{  REG_BITS_TYPE_CHECK,  7,  1,  1, "Exclusive cache bit.", "does not permit data to reside in L1 and L2 at the same time" },
		{  REG_BITS_TYPE_CHECK,  6,  1,  1, "SMP", "Cortex-A9 processor is taking part in coherency" },
		{  REG_BITS_TYPE_CHECK,  3,  1,  1, "Write full line of zeros mode", "Enabled" },
		{  REG_BITS_TYPE_CHECK,  2,  1,  1, "L1 prefetch ", "enabled" },
		{  REG_BITS_TYPE_CHECK,  1,  1,  1, "L2 prefetch ", "enabled" },
		{  REG_BITS_TYPE_CHECK,  0,  1,  1, "Cache and TLB maintenance broadcast", "enabled" },
		{  0xffffffff,	0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read Auxiliary Control Register */
#ifdef __GNUC__
	u32_aux_ctrl_reg = mfcp(XREG_CP15_AUX_CONTROL);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_AUX_CONTROL);
	  u32_aux_ctrl_reg = Reg; }
#endif
	print_var_hex(u32_aux_ctrl_reg);

	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 */
    printf("Auxiliary Control Register: 0x%08x\n\r", u32_aux_ctrl_reg);

	register_report( u32_aux_ctrl_reg, "Auxiliary Control Register", aux_ctrl_reg_definition  );

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
void auxiliary_control_register_parity_report( void )
{
	register unsigned int u32_aux_ctrl_reg;


	struct reg_defintion   aux_ctrl_reg_definition[] =
	{
		{  REG_BITS_TYPE_CHECK,  9,  1,  1, "Parity on", "Parity checking enabled" },
		{  0xffffffff,	0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read Auxiliary Control Register */
#ifdef __GNUC__
	u32_aux_ctrl_reg = mfcp(XREG_CP15_AUX_CONTROL);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_AUX_CONTROL);
	  u32_aux_ctrl_reg = Reg; }
#endif
	print_var_hex(u32_aux_ctrl_reg);

	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 */
	register_report( u32_aux_ctrl_reg, "Auxiliary Control Register", aux_ctrl_reg_definition  );

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
void reboot_status_register_report( void )
{
	register unsigned int u32_reboot_status;

	struct reg_defintion   reboot_status_reg_definition[] =
	{
		{  REG_BITS_TYPE_CHECK,  22,  1,  1, "POR", "Last reset was due to POR (power on reset)" },
		{  REG_BITS_TYPE_CHECK,  21,  1,  1, "SRST_B", "Last reset was due to SRST_B(soft reset)" },
		{  REG_BITS_TYPE_CHECK,  20,  1,  1, "DBG_RST", "Last reset was due to DBG_RST(debug system reset)" },
		{  REG_BITS_TYPE_CHECK,  19,  1,  1, "SLC_RST", "Last reset was due to SLC_RST(SLC soft reset)" },
		{  REG_BITS_TYPE_CHECK,  18,  1,  1, "AWDT1_RST", "Last reset was due to APU watchdog timer 1" },
		{  REG_BITS_TYPE_CHECK,  17,  1,  1, "AWDT0_RST", "Last reset was due to APU watchdog timer 0" },
		{  REG_BITS_TYPE_CHECK,  16,  1,  1, "SWDT_RST", "Last reset was due to system watchdog timeout" },
		{  0xffffffff,	0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read reboot_status Register */

	Xil_Out32(SLCR_UNLOCK, SLCR_UNLOCK_VAL);

	u32_reboot_status = Xil_In32(0xF8000258);
	print_var_hex(u32_reboot_status);
	print_var_hex( (u32_reboot_status>>16)&0xff );
	Xil_Out32(0xF8000258, 0);

	Xil_Out32(SLCR_LOCK, SLCR_LOCK_VAL);

	/*
	 * Xilinx default setting:	SCTLR: 0x08c5187d
	 */
	register_report( u32_reboot_status, "Reboot Status", reboot_status_reg_definition  );

}






/****************************************************************************
*
* Read and report  Current Program Status Register (CPSR)
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void program_status_register_report( void )
{
	register unsigned int u32_current_program_status_reg;
	register unsigned int u32_saved_program_status_reg;


/*
	The Current Program Status Register (CPSR)
	The Saved Program Status Registers (SPSRs)
	Format of the CPSR and SPSRs
	B1.3.1 ARM processor modes : Table B1-1 ARM processor modes
*/
	struct reg_defintion   program_status_reg_definition[] =
	{
		{  REG_BITS_TYPE_SHOW_DEC,   31,  1,  1, "N, bit [31]", "Negative condition code flag" },
		{  REG_BITS_TYPE_SHOW_DEC,	 30,	1,	1, "Z, bit [30]", "Zero condition code flag" },
		{  REG_BITS_TYPE_SHOW_DEC,    29,  1,  1, "C, bit [29]", "Carry condition code flag" },
		{  REG_BITS_TYPE_SHOW_DEC,    28,  1,  1, "N, bit [31]", "Overflow condition code flag." },
		{  REG_BITS_TYPE_SHOW_DEC,    27,  1,  1, "Q, bit [27]", "Cumulative saturation flag." },
		{  REG_BITS_TYPE_SHOW_DEC,    25,  0x3,  1, "IT[1:0], bits [26:25]", "If-Then execution state bits for the Thumb IT (If-Then) instruction. " },
		{  REG_BITS_TYPE_SHOW_DEC,    24,  1,  1, "J, bit [24]", "Jazelle bit" },
		{  REG_BITS_TYPE_SHOW_DEC,    16,  0xf,  1, "GE[3:0], bits [19:16]", "Greater than or Equal flags, for SIMD instructions." },
		{  REG_BITS_TYPE_SHOW_DEC,    10,  0x3f,  1, "IT[7:2], bits [15:10]", "If-Then execution state bits for the Thumb IT (If-Then) instruction. " },
		{  REG_BITS_TYPE_CHECK,    9,  1,  1, "E, bit [9]", "Endianness execution state bit, 1  Big endian operation." },
		{  REG_BITS_TYPE_CHECK,    9,  1,  0, "E, bit [9]", "Endianness execution state bit, 0  Little endian operation." },
		{  REG_BITS_TYPE_SHOW_DEC,    8,  1,  1, "A, bit [8] Asynchronous abort disable bit.", " " },
		{  REG_BITS_TYPE_SHOW_DEC,    7,  1,  1, "I, bit [7]  Interrupt disable bit.", "" },
		{  REG_BITS_TYPE_CHECK,    6,  1,  0, "F, bit [6]  Fast interrupt disable bit.", "0	Exception enabled" },
		{  REG_BITS_TYPE_CHECK,    6,  1,  1, "F, bit [6]  Fast interrupt disable bit.", "1  Exception disabled." },
		{  REG_BITS_TYPE_SHOW_DEC,    5,  1,  1, "T, bit [5]", "Thumb execution state bit." },
		{  REG_BITS_TYPE_SHOW_HEX,    0,  0x1f,  0, "M[4:0], bits [4:0] Mode field.", "Value" },
		
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b10000, "M[4:0], bits [4:0] Mode field.", "User mode, Unprivileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b10001, "M[4:0], bits [4:0] Mode field.", "FIQ mode, Privileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b10010, "M[4:0], bits [4:0] Mode field.", "IRQ mode, Privileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b10011, "M[4:0], bits [4:0] Mode field.", "Supervisor mode, Privileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b10110, "M[4:0], bits [4:0] Mode field.", "Monitor mode, Privileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b10111, "M[4:0], bits [4:0] Mode field.", "Abort mode, Privileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b11011, "M[4:0], bits [4:0] Mode field.", "Undefined mode, Privileged" },
		{  REG_BITS_TYPE_CHECK,    0,  0x1f,  0b11111, "M[4:0], bits [4:0] Mode field.", "System mode, Privileged" },
		{  0xffffffff,	0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read Current Program Status Register (CPSR) */
	u32_current_program_status_reg = mfcpsr();
	print_var_hex(u32_current_program_status_reg);
	
	u32_saved_program_status_reg = mfspsr();
	print_var_hex(u32_saved_program_status_reg);

	register_report( u32_current_program_status_reg, "Current Program Status Register (CPSR)", program_status_reg_definition  );
    printf("\n\r");
	register_report( u32_saved_program_status_reg, "Saved Program Status Register (CPSR)", program_status_reg_definition  );
    printf("\n\r");
	

}






/****************************************************************************
*
* Read and report  Current Program Status Register (CPSR)
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void secure_config_register_report( void )
{
	register unsigned int u32_current_program_status_reg;
	register unsigned int u32_secure_config_reg;

	struct reg_defintion   secure_config_reg_definition[] =
	{
		{  REG_BITS_TYPE_CHECK,    6,  1,  0, "nET, bit [6]  Not Early Termination.", "0  Early termination permitted." },
		{  REG_BITS_TYPE_CHECK,    6,  1,  1, "nET, bit [6]  Not Early Termination.", "1  Disable early termination. " },
		{  REG_BITS_TYPE_CHECK,    5,  1,  0, "AW, bit [5]  A bit writable. ", "0  the CPSR.A bit can be modified only in Secure state." },
		{  REG_BITS_TYPE_CHECK,    5,  1,  1, "AW, bit [5]  A bit writable. ", "1  the CPSR.A bit can be modified in any security state." },
		{  REG_BITS_TYPE_CHECK,    4,  1,  0, "FW, bit [4]  F bit writable. ", "0  the CPSR.F bit can be modified only in Secure state" },
		{  REG_BITS_TYPE_CHECK,    4,  1,  1, "FW, bit [4]  F bit writable. ", "1  the CPSR.F bit can be modified in any security state." },
		{  REG_BITS_TYPE_CHECK,    3,  1,  0, "EA, bit [3]  External Abort handler.", "0  Abort mode handles external aborts" },
		{  REG_BITS_TYPE_CHECK,    3,  1,  1, "EA, bit [3]  External Abort handler.", "1  Monitor mode handles external aborts." },
		{  REG_BITS_TYPE_CHECK,    2,  1,  0, "FIQ, bit [2]  FIQ handler.", "0  FIQ mode entered when FIQ is taken" },
		{  REG_BITS_TYPE_CHECK,    2,  1,  1, "FIQ, bit [2]  FIQ handler.", "1  Monitor mode entered when FIQ is taken." },
		{  REG_BITS_TYPE_CHECK,    1,  1,  0, "IRQ, bit [1]  IRQ handler.", "0  IRQ mode entered when IRQ is taken" },
		{  REG_BITS_TYPE_CHECK,    1,  1,  1, "IRQ, bit [1]  IRQ handler.", "1  Monitor mode entered when IRQ is taken." },
		{  REG_BITS_TYPE_CHECK,    0,  1,  0, "NS, bit [0]  Non Secure bit.", "Secure state" },
		{  REG_BITS_TYPE_CHECK,    0,  1,  1, "NS, bit [0]  Non Secure bit.", "Non-secure state if Processor mode is not in monitor mode" },
		{  0xffffffff,	0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read  Secure Configuration Register (SCR) */
#ifdef __GNUC__
	u32_secure_config_reg = mfcp(XREG_CP15_SECURE_CONFIG);
#else
	{ volatile register unsigned int Reg __asm(XREG_CP15_SECURE_CONFIG);
	  u32_secure_config_reg = Reg; }
#endif
	print_var_hex(u32_secure_config_reg);


	/* read Current Program Status Register (CPSR) */
	u32_current_program_status_reg = mfcpsr();
	print_var_hex(u32_current_program_status_reg);


	/*
	 * Secure Configuration Register (SCR)
	 */
	register_report( u32_secure_config_reg, "Secure Configuration Register (SCR)", secure_config_reg_definition  );


	if( 0b10110 == (u32_current_program_status_reg&0x1f) )
	{
		printf("Processor mode is in monitor mode\n");
	}
	else
	{
		if( 0 == (u32_secure_config_reg&0x1) )
		{
			printf("Processor mode is in secure mode\n");
		}
		else
		{
			printf("Processor mode is in non-secure mode\n");
		}
	}
}

