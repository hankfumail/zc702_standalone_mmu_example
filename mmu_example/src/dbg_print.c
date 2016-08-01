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
* @file dbg_print.c
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




#include "dbg_print.h"






/************************** Constant Definitions *****************************/



/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/




/************************** Variable Definitions *****************************/


/*   */
u8        gu8_dbg_flag=0;
u8        gu8_mod_dbg_flag[DBG_MOD_NUM]={};


/************************** Function Prototypes ******************************/




/************************** Function Implementation ******************************/



void dbg_flag_set( u8 u8_dbg_flag )
{

    gu8_dbg_flag = u8_dbg_flag;

}

u8 dbg_flag_get( void )
{

    return gu8_dbg_flag;

}


void dbg_mod_flag_set( u8 u8_dbg_mod, u8 u8_mod_dbg_flag )
{
    if( u8_dbg_mod >= DBG_MOD_NUM )
    {
        return;
    }

    gu8_mod_dbg_flag[u8_dbg_mod] = u8_mod_dbg_flag;

}

u8 dbg_mod_flag_get( u8 u8_dbg_mod )
{
    if( u8_dbg_mod >= DBG_MOD_NUM )
    {
        return 0;
    }

    return gu8_mod_dbg_flag[u8_dbg_mod];

}

#ifdef DBG_PRINT_TEST

void dbg_print_test( void )
{
    int  i_check = 1000;
    static int  i_check_static = 1000;
    int  *pi_null = NULL;
    int  *pi_check = &i_check ;
    int  *pi_check_static = &i_check_static ;

    dbg_print("dbg_print_test begin.")

    dbg_print_param_min_check(i_check, 100, RTN_VOID);
    dbg_print_param_min_check(i_check, 10000, RTN_VOID);
    dbg_print_param_max_check(i_check, 100, RTN_VOID);
    dbg_print_param_max_check(i_check, 10000, RTN_VOID);
    dbg_print_param_limit_check(i_check, 100, 200-1, RTN_VOID);
    dbg_print_param_limit_check(i_check, 100, 10000-1, RTN_VOID);
    dbg_print_param_limit_check(i_check, 10000, 20000-1, RTN_VOID);
    
    dbg_print_null_pointer_check(pi_null, RTN_VOID);
    dbg_print_null_pointer_check(i_check, RTN_VOID);
    
    dbg_ptr_limit_check(i_check, RTN_VOID);
    ptr_limit_check(i_check, RTN_VOID);
    dbg_ptr_limit_check(pi_check_static, RTN_VOID);
    ptr_limit_check(pi_check_static, RTN_VOID);

    
#ifdef DBG_RUNTIME_PRINT_ENABLE
    dbg_flag_set( 5 );
    dbg_rt_print(4, "dbg_rt_print at level 4, should not be printed out.")
    dbg_rt_print(5, "dbg_rt_print at level 5, should not be printed out.")
    dbg_rt_print(6, "dbg_rt_print at level 6, should be printed out.")
#endif  /* DBG_RUNTIME_PRINT_ENABLE  */

    
#ifdef DBG_RUNTIME_PRINT_ENABLE
    dbg_mod_flag_set( 0, 5 );
    dbg_rt_mod_print(0, 4, "dbg_rt_print at level 4, should not be printed out.")
    dbg_rt_mod_print(0, 5, "dbg_rt_print at level 5, should not be printed out.")
    dbg_rt_mod_print(0, 6, "dbg_rt_print at level 6, should be printed out.")
#endif  /* DBG_RUNTIME_MOD_PRINT_ENABLE  */

}

#endif  /* DBG_PRINT_TEST  */

