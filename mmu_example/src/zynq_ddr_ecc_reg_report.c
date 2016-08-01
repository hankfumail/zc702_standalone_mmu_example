/******************************************************************************
*
* (c) Copyright 2013 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file Mb_stack_unwind.c
*
* This file contains a design example using the MicroBlaze exception features.
*
* @note		None.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver       Who       Date               Changes
* -----   ----     --------    ---------------------------------------------
* 1.00a    Hank     03/10/14       First release
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xil_cache.h"
#include "xil_io.h"
#include "xil_types.h"

#include "dbg_print.h"

#include "zynq_ddr_ecc_reg_report.h"



/************************** Constant Definitions *****************************/



/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/


/************************** Variable Definitions *****************************/





/************************** Function Prototypes ******************************/




/************************** Function Implementation ******************************/





void ecc_reg_read_print( void )
{

    unsigned int ui_reg;

    ui_reg = Xil_In32( 0xF80060C4 );
	xil_printf("0xF80060C4(ECC error clear):\t%08x\n\r", ui_reg);


    ecc_corr_reg_read_print( );
    ecc_uncorr_reg_read_print( );

    ui_reg = Xil_In32( 0xF80060F0 );
	xil_printf("0xF80060F0(ECC error count):\t%08x\n\r", ui_reg);
	xil_printf("0xF80060F0(correctable ECC error count,):\t%08x\n\r", (ui_reg>>8)&0xff );
	xil_printf("0xF80060F0(uncorrectable ECC error count):\t%08x\n\r", ui_reg&0xff);

    ui_reg = Xil_In32( 0xF80060F4 );
	xil_printf("0xF80060F4(ECC mode/scrub,shoud be 0x4 if ECC is enabled.):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060F8 );
	xil_printf("0xF80060F8(ECC data mask low, 8-bits ):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060FC );
	xil_printf("0xF80060FC(ECC data mask high, always be 0):\t%08x\n\r", ui_reg);

}


void ecc_corr_reg_read_print( void )
{

    unsigned int ui_reg;

    ui_reg = Xil_In32( 0xF80060C8 );
	xil_printf("0xF80060C8(ECC error correction number):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060CC );
	xil_printf("0xF80060CC(ECC error correction address log):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060D0 );
	xil_printf("0xF80060D0(ECC error correction data log low 8-bits):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060D4 );
	xil_printf("0xF80060D4(ECC error correction data log mid always be 0):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060D8 );
	xil_printf("0xF80060D8(ECC error correction data log high  5-bits):\t%08x\n\r", ui_reg);


}

void ecc_uncorr_reg_read_print( void )
{

    unsigned int ui_reg;


    ui_reg = Xil_In32( 0xF80060DC );
	xil_printf("0xF80060DC(ECC unrecoverable error status, bit 0):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060E0 );
	xil_printf("0xF80060E0(ECC unrecoverable error address):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060E4 );
	xil_printf("0xF80060E4(ECC unrecoverable error data low lower 8-bits ):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060E8 );
	xil_printf("0xF80060E8(ECC unrecoverable error data middle, always be 0.):\t%08x\n\r", ui_reg);

    ui_reg = Xil_In32( 0xF80060EC );
	xil_printf("0xF80060EC(ECC unrecoverable error data high 5-bits ):\t%08x\n\r", ui_reg);


}
void ecc_reg_counter_print( void )
{
    unsigned int ui_reg;

    ui_reg = Xil_In32( 0xF80060F0 );
	xil_printf("0xF80060F0(ECC error count):\t%08x\n\r", ui_reg);
	xil_printf("0xF80060F0(correctable ECC error count,):\t%08x\n\r", (ui_reg>>8)&0xff );
	xil_printf("0xF80060F0(uncorrectable ECC error count):\t%08x\n\r", ui_reg&0xff);

}

void l2c_reg_print( void )
{
    unsigned int ui_reg;

    ui_reg = Xil_In32( 0xF8F0221C );
	xil_printf("0xF8F0221C(L2 cache Raw Interrupt Status Register):\t%08x\n\r", ui_reg);

    if( 0!=( ui_reg&0x180) )
    {
        xil_printf("Cacheable memory has error data.\n\r" );
    }
}





