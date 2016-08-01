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

	 
#ifndef __SYS_MMU_TABLE_H_
#define __SYS_MMU_TABLE_H_


#include <stdio.h>

#include "sleep.h"


#include "xil_types.h"
#include "xil_io.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "xil_exception.h"

#include "Xpseudo_asm.h"



/***************** Macros (Inline Functions) Definitions *********************/
/*
 B3.7.2 C, B, and TEX[2:0] encodings without TEX remap
Table B3-7 shows the C, B, and TEX[2:0] encodings when TEX remap is disabled (TRE == 0).
TEX[2:0] : 14:12 bits
C	:  3 bit
B	: 2 bit

 00 Non-cacheable
 01 Write-Back, Write-Allocate
 10 Write-Through, no Write-Allocate
 11 Write-Back, no Write-Allocate
 */
#define		ARM_MMU_TBL_NO_CACHE  							0
#define		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE  			1
#define		ARM_MMU_TBL_WRITE_THROUGH_NO_WRITE_ALLOCATE  	2
#define		ARM_MMU_TBL_WRITE_BACK_NO_WRITE_ALLOCATE  		3


/*  Table B3-4 VMSAv7 MMU access permissions
0 00	 Privileged: No access		  User: No access All accesses generate Permission faults
0 01	 Privileged: Read/write 	 User: No access Privileged access only
0 10	 Privileged: Read/write 	 User: Read-only Writes in User mode generate Permission faults
0 11	 Privileged: Read/write 	 User: Read/write Full access
1 00	 - - Reserved

1 01	 Privileged: Read-only		User: No access Privileged read-only
1 10	 Privileged: Read-only		User: Read-only Privileged and User read-only, deprecated in VMSAv7a
1 11	 Privileged: Read-only		User: Read-only Privileged and User read-onlyb
*/


#define		ARM_MMU_TBL_PRVLG_NO_ACCESS_USER_NO_ACCESS  			0
#define		ARM_MMU_TBL_PRVLG_RW_USER_NO_ACCESS  					1
#define		ARM_MMU_TBL_PRVLG_RW_USER_READ_ONLY  					2
#define		ARM_MMU_TBL_PRVLG_RW_USER_RW  							3
#define		ARM_MMU_TBL_PRVLG_READ_ONLY_USER_NO_ACCESS  			5
#define		ARM_MMU_TBL_PRVLG_READ_ONLY_USER_READ_ONLY  			7




/**************************** Type Definitions *******************************/

/************************** Constant Definitions *****************************/

/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/

void SetMmuTblCacheAttributes(u32 addr, u32 u32Inner, u32 u32Outer, u32 u32Share, u32 u32StronglyOrdered, u32 u32Device );
void SetMmuTblCacheAndAccessAttributes(u32 addr, u32 u32Inner, u32 u32Outer, u32 u32Share, u32 u32StronglyOrdered, u32 u32Device, u32 u32AccessFlag );
void ReportMmuTblAttributes( u32 addr );
void ReadSysControlMmuSetting( void );


#endif /* __SYS_MMU_TABLE_H_  */

