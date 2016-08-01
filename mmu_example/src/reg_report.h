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

     
#ifndef __REG_REPORT_H_
#define __REG_REPORT_H_


#include <stdio.h>

#include "xil_types.h"




/***************** Macros (Inline Functions) Definitions *********************/

#define        REG_BITS_TYPE_SHOW_DEC        10        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_02HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_04HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_SHOW_08HEX        11        /* Print name and value */
#define        REG_BITS_TYPE_CHECK        20        /* Check value, print information if it equals */



/* pseudo assembler instructions */
#define mfspsr()    ({unsigned int rval; \
              __asm__ __volatile__(\
                "mrs    %0, spsr\n"\
                : "=r" (rval)\
              );\
              rval;\
             })

#define mtspsr(v)    __asm__ __volatile__(\
              "msr    spsr,%0\n"\
              : : "r" (v)\
            )
        
#define mflr( )    ({unsigned int rval; \
                      __asm__ __volatile__(\
                        "mov %0,lr\n"\
                        : "=r" (rval)\
                      );\
                      rval;\
                     })
        
#define mfpc( )    ({unsigned int rval; \
                      __asm__ __volatile__(\
                        "mov %0,pc\n"\
                        : "=r" (rval)\
                      );\
                      rval;\
                     })
    

#define mfsp( )    ({unsigned int rval; \
              __asm__ __volatile__(\
                "mov %0,sp\n"\
                : "=r" (rval)\
              );\
              rval;\
             })



/**************************** Type Definitions *******************************/

struct reg_defintion {
    u32          type;
    u32          shift_right_num;
    u32          mask;
    u32            value;
    char         name[64];
    char        info[256-64-4-4-4-4];
};

struct reg_defintion2 {
    u32          type;
    u32          a_shift_right_num;
    u32          a_mask;
    u32          b_shift_right_num;
    u32          b_mask;
    u32            value;
    char         name[64];
    char        info[256-64-4-4-4-4-4-4];
};

/*
struct reg_defintion2    data_fault_status_reg_definition[] =
{
    *    Type                               a_shift_num       a_mask    b_shift_num   b_mask         value   name       info          *
    {  REG_BITS_TYPE_CHECK,         0x1,         0x0,         0x6,         0x1,         0x0, "            ", "            " },
    {  REG_BITS_TYPE_CHECK,         0x1,         0x0,         0x6,         0x1,         0x0, "            ", "            " },
    {  0xffffffff,    0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff, "", "" }
};
*/

/************************** Constant Definitions *****************************/


/************************** Variable Definitions *****************************/



/************************** Function Prototypes ******************************/

void register_report( u32 u32_reg_value, char *ps_reg_name, struct reg_defintion *p_reg_info  );
void register2_report( u32 u32_reg_value, char *ps_reg_name, struct reg_defintion2 *p_reg_info  );
void register_read_report( u32 u32_reg_addr, char *ps_reg_name, struct reg_defintion *p_reg_info  );
void auxiliary_control_register_report( void );
void auxiliary_control_register_parity_report( void );
void reboot_status_register_report( void );
void program_status_register_report( void );
void secure_config_register_report( void );

#endif /* __REG_REPORT_H_  */

