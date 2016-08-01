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

	 
#ifndef __DBG_PRINT_H_
#define __DBG_PRINT_H_


#include <stdio.h>
#include <stdlib.h>

#include "xil_types.h"



/**************************** Type Definitions *******************************/



/************************** Constant Definitions *****************************/

#define	DBG_MOD_NUM     16

#define	RTN_VOID


/***************** Macros (Inline Functions) Definitions *********************/

#define	DBG_CHECK


#define	DBG_PRINT_ENABLE
#define	DBG_RUNTIME_PRINT_ENABLE
#define	DBG_RUNTIME_MOD_PRINT_ENABLE
#define	DBG_PRINT_TEST

#ifndef STDOUT_BASEADDRESS
#undef DBG_PRINT_ENABLE
#undef DBG_RUNTIME_PRINT_ENABLE
#undef DBG_RUNTIME_MOD_PRINT_ENABLE
#undef DBG_PRINT_TEST
#endif


/* debug print function */
#define print_var(xxx) 						xil_printf( "%s = %d\r\n", #xxx, xxx )
#define print_var_non_zero(xxx) 			{	if (0 != (xxx) ) xil_printf( "%s = %d\r\n", #xxx, xxx ); }
#define print_info_var(info, xxx) 			xil_printf( info" %s = %d\r\n", #xxx, xxx )
#define print_info_var_non_zero(info, xxx) 	{	if (0 != (xxx) ) xil_printf( info" %s = %d\r\n", #xxx, xxx ); }
#define print_var_hex(xxx) 					xil_printf( "%s = 0x%08x\r\n", #xxx, (unsigned int)xxx )
#define print_var_hex_non_zero(xxx) 		{	if (0 != (xxx) ) xil_printf( "%s = 0x%08x\r\n", #xxx, (unsigned int)xxx ); }
#define print_info_var_hex(info, xxx) 		xil_printf( info" %s = 0x%08x\r\n", #xxx, (unsigned int)xxx )
#define print_info_var_hex_non_zero(info, xxx) 		{	if (0 != xxx ) xil_printf( info" %s = 0x%08x\r\n", #xxx, (unsigned int)xxx ); }

#define print_var_location(xxx) 			xil_printf( "Address of variable:%s is 0x%08x\r\n", #xxx, (unsigned int)&xxx )

#define print_pointer_hex_4(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3] ); }

#define print_pointer_hex_8(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3] ); \
												xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[4]), (pointer)[4], (pointer)[5], (pointer)[6], (pointer)[7] );}

#define print_pointer_hex_8a(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x  %08x  %08x  %08x  %08x\r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3], \
						(pointer)[4], (pointer)[5], (pointer)[6], (pointer)[7] ); }

#define print_pointer_hex_16(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3] ); \
												xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[4]), (pointer)[4], (pointer)[5], (pointer)[6], (pointer)[7] ); \
											xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[8]), (pointer)[8], (pointer)[9], (pointer)[10], (pointer)[11] ); \
												xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[12]), (pointer)[12], (pointer)[13], (pointer)[14], (pointer)[15] );}


#define print_var_old(xxx) 					xil_printf( "Old %s = %d\r\n", #xxx, (unsigned int)xxx )
#define print_var_old_hex(xxx) 				xil_printf( "Old %s = 0x%08x\r\n", #xxx, (unsigned int)xxx )
#define print_var_old_hex_non_zero(xxx) 	{	if (0 != xxx ) xil_printf( "Old %s = 0x%08x\r\n", #xxx, (unsigned int)xxx ); }
#define print_var_new(xxx) 					xil_printf( "New %s = %d\r\n", #xxx, (unsigned int)xxx )
#define print_var_new_hex(xxx) 				xil_printf( "New %s = 0x%08x\r\n", #xxx, (unsigned int)xxx )
#define print_var_new_hex_non_zero(xxx) 	{	if (0 != xxx ) xil_printf( "New %s = 0x%08x\r\n", #xxx, (unsigned int)xxx ); }


#ifdef	DBG_PRINT_ENABLE

#define dbg_print_func_begin 						xil_printf( "\r\n%s : begin.\r\n", __func__ )

#define dbg_print(format,args...) 				xil_printf(format, ##args)

#define dbg_print_var(xxx) 						xil_printf( "%s : %s = %d\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_var_non_zero(xxx) 			{	if (0 != (unsigned int)(xxx) ) xil_printf( "%s : %s = %d\r\n", __func__, #xxx, (unsigned int)xxx ); }
#define dbg_print_var_hex(xxx) 					xil_printf( "%s : %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_var_hex_non_zero(xxx) 		{	if (0 != (unsigned int)(xxx) ) xil_printf( "%s : %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx ); }

#define dbg_print_info_var(info, xxx) 			xil_printf( " %s :"info" : %s = %d\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_info_var_non_zero(info, xxx) 	{	if (0 != (unsigned int)(xxx) ) xil_printf( info" %s : %s = %d\r\n", __func__, #xxx, (unsigned int)xxx ); }
#define dbg_print_info_var_hex(info, xxx) 		xil_printf( " %s :"info"%s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_info_var_hex_non_zero(info, xxx) 		{	if (0 != (unsigned int)(xxx) ) xil_printf( " %s :"info" %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx ); }

#define dbg_print_null_pointer_check(xxx, rtn_sts) 		{	if (0 != (unsigned int)(xxx) ) { xil_printf( "Error: %s: %s pointer: %s is NULL.\r\n", __FILE__, __func__, #xxx );  return rtn_sts; } }

#define dbg_print_param_min_check(xxx, min, rtn_sts) 	{	if ( (unsigned int)(xxx)<(min) ) { xil_printf( "Error: %s: %s parameter: %s: %d is less than %d.\r\n", __FILE__, __func__, #xxx, xxx, min );  return rtn_sts; } }
#define dbg_print_param_max_check(xxx, max, rtn_sts) 	{	if ( (unsigned int)(xxx)>(max) ) { xil_printf( "Error: %s: %s parameter: %s: %d is great than %d.\r\n", __FILE__, __func__, #xxx, xxx, max );  return rtn_sts; } }
#define dbg_print_param_limit_check(xxx, min, max, rtn_sts) 	{	if ( ( (unsigned int)(xxx)<(min) ) || ( (unsigned int)(xxx)>(max) ) ) { xil_printf( "Error: %s: %s parameter: %s: %d is beyond %d-%d.\r\n", __FILE__, __func__, #xxx, xxx, min, max );  return rtn_sts; } }

#define dbg_print_pointer_hex_4(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3] ); }

#define dbg_print_pointer_hex_8(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3] ); \
												xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[4]), (pointer)[4], (pointer)[5], (pointer)[6], (pointer)[7] );}

#define dbg_print_pointer_hex_16(pointer) 		{	xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[0]), (pointer)[0], (pointer)[1], (pointer)[2], (pointer)[3] ); \
												xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[4]), (pointer)[4], (pointer)[5], (pointer)[6], (pointer)[7] ); \
											xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[8]), (pointer)[8], (pointer)[9], (pointer)[10], (pointer)[11] ); \
												xil_printf( "0x%08x:  %08x  %08x  %08x  %08x \r\n", \
						(unsigned int)&((pointer)[12]), (pointer)[12], (pointer)[13], (pointer)[14], (pointer)[15] );}


#define dbg_print_var_old(xxx) 					xil_printf( "%s : Old %s = %d\r\n", __func__, #xxx,(unsigned int) xxx )
#define dbg_print_var_old_hex(xxx) 				xil_printf( "%s : Old %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_var_old_hex_non_zero(xxx) 	{	if (0 != (unsigned int)xxx ) xil_printf( "%s : Old %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx ); }

#define dbg_print_var_new(xxx) 					xil_printf( "%s : New %s = %d\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_var_new_hex(xxx) 				xil_printf( "%s : New %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx )
#define dbg_print_var_new_hex_non_zero(xxx) 	{	if (0 != (unsigned int)xxx ) xil_printf( "%s : New %s = 0x%08x\r\n", __func__, #xxx, (unsigned int)xxx ); }

#else     	/* DBG_PRINT_ENABLE  */

#define dbg_print_func_begin 		

#define dbg_print(format,args...)

#define dbg_print_var(xxx)
#define dbg_print_var_non_zero(xxx)
#define dbg_print_var_hex(xxx)
#define dbg_print_var_hex_non_zero(xxx)

#define dbg_print_info_var(info, xxx)
#define dbg_print_info_var_non_zero(info, xxx)
#define dbg_print_info_var_hex(info, xxx)
#define dbg_print_info_var_hex_non_zero(info, xxx)

#define dbg_print_null_pointer_check(xxx, rtn_sts) 		

#define dbg_print_param_min_check(xxx, min, rtn_sts) 	
#define dbg_print_param_max_check(xxx, max, rtn_sts) 	

#define dbg_print_pointer_hex_4(pointer) 		
#define dbg_print_pointer_hex_8(pointer) 	
#define dbg_print_pointer_hex_16(pointer) 		


#define dbg_print_var_old(xxx)
#define dbg_print_var_old_hex(xxx)
#define dbg_print_var_old_hex_non_zero(xxx)

#define dbg_print_var_new(xxx)
#define dbg_print_var_new_hex(xxx)
#define dbg_print_var_new_hex_non_zero(xxx)

#endif		/* DBG_PRINT_ENABLE  */


#ifdef DBG_RUNTIME_PRINT_ENABLE

#define dbg_rt_print(type,...) \
		if (((type) > gu8_dbg_flag))  {xil_printf (__VA_ARGS__); }
        
#else		/* DBG_RUNTIME_PRINT_ENABLE  */

#define dbg_rt_print(type, ...)

#endif		/* DBG_RUNTIME_PRINT_ENABLE  */

#ifdef DBG_RUNTIME_MOD_PRINT_ENABLE

#define dbg_rt_mod_print(mod, type,...)                \
    if (((mod) < DBG_MOD_NUM ) )                        \
        {                                               \
            if (((type) > gu8_mod_dbg_flag[mod]))      \
            {                                           \
                xil_printf (__VA_ARGS__);               \
            }                                            \
        }
    
        
#else		/* DBG_RUNTIME_MOD_PRINT_ENABLE  */

#define dbg_rt_mod_print(type, ...)

#endif		/* DBG_RUNTIME_MOD_PRINT_ENABLE  */


#ifdef DBG_CHECK

#define dbg_ptr_limit_check( ptr, rtn_sts )                                      \
    if( ( (unsigned int) (ptr) <=    (unsigned int)__rodata_end )           \
         ||  ( (unsigned int) (ptr) >    (unsigned int)_heap_end ) )        \
        {                                               \
	        xil_printf( "Error: %s : %s : pointer %s is invalid: 0x%08x \r\n", __FILE__, __func__, #ptr, (unsigned int)ptr );  \
            return rtn_sts;                                          \
        }
    
        
#else		/* DBG_CHECK  */

#define dbg_ptr_limit_check( ptr, rtn_sts )

#endif		/* DBG_CHECK  */



#define ptr_limit_check( ptr, rtn_sts  )                                      \
    if( ( (unsigned int) (ptr) <=    (unsigned int)__rodata_end )           \
         ||  ( (unsigned int) (ptr) >    (unsigned int)_heap_end ) )        \
        {                                               \
	        xil_printf( "Error: %s : %s : pointer %s is invalid: 0x%08x \r\n", __FILE__, __func__, #ptr, (unsigned int)ptr );  \
            return rtn_sts;                                                 \
        }


/************************** Variable Definitions *****************************/

extern  u8        gu8_dbg_flag;
extern  u8        gu8_mod_dbg_flag[DBG_MOD_NUM];

extern int __rodata_start[];
extern int __rodata_end[];
extern int _heap_start[];
extern int _heap_end[];

/************************** Function Prototypes ******************************/


void dbg_flag_set( u8 u8_dbg_flag );
u8 dbg_flag_get( void );

void dbg_mod_flag_set( u8 u8_dbg_mod, u8 u8_mod_dbg_flag );
u8 dbg_mod_flag_get( u8 u8_dbg_mod );

#ifdef DBG_PRINT_TEST
void dbg_print_test( void );
#endif  /* DBG_PRINT_TEST  */

#endif /* __DBG_PRINT_H_  */

