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
* @file mmu_example.c
*
* This file contains a design example using the Cortex A9 MMU to protect memory.
*
* @note		None.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver       Who       Date               Changes
* -----   ----     --------    ---------------------------------------------
* 1.00a    Hank     03/10/10       First release
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

#include "sys_reg_report.h"
#include "xil_cache.h"
#include "mmu_table.h"
#include "reg_report.h"

#include "dbg_print.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are only defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#ifndef TESTAPP_GEN
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#endif


/**************************** Type Definitions *******************************/



/***************** Macros (Inline Functions) Definitions *********************/




/************************** Variable Definitions *****************************/

#ifndef TESTAPP_GEN
XScuGic IntcInstance;		/* Interrupt Controller Instance */
#endif

/*
 * The following variables are shared between non-interrupt processing and
 * interrupt processing such that they must be global.
 */
volatile int TimerExpired;


static u32 u32_prefetch_abort_num=0;
static u32 u32_data_abort_num=0;
static u32 u32_swi_num=0;


extern int __text_start[];
extern int __text_end[];


extern int __init_start[];
extern int __init_end[];

extern int __fini_start[];
extern int __fini_end[];

extern int __rodata_start[];
extern int __rodata_end[];

extern int __rodata1_start[];
extern int __rodata1_end[];

extern int __sdata2_start[];
extern int __sdata2_end[];

extern int __sbss2_start[];
extern int __sbss2_end[];


extern int __data_start[];
extern int __data_end[];
extern int __bss_start[];
extern int __bss_end[];

extern int __mmu_tbl_protect_hole_begin[];
extern int __mmu_tbl_start[];
extern int __mmu_tbl_end[];

extern int __heap_protect_hole_begin[];
extern int _heap_start[];
extern int _heap_end[];

extern int __stack_protect_hole_begin[];
extern int _stack_end[];
extern int __stack[];
extern int _irq_stack_end[];
extern int __irq_stack[];
extern int _supervisor_stack_end[];
extern int __supervisor_stack[];
extern int _abort_stack_end[];
extern int __abort_stack[];
extern int _fiq_stack_end[];
extern int __fiq_stack[];
extern int _undef_stack_end[];
extern int __undef_stack[];



/************************** Function Prototypes ******************************/
static int SetupIntrSystem(XScuGic *IntcInstancePtr );

void ExceptionMemAccess( void );
void ExceptionMemWrite( unsigned int *pu32_mem_region );

void ExceptionSwiHandler(void *Data);
void ExceptionDataAbortHandler(void *Data);
void ExceptionPrefetchAbortHandler(void *Data);
void ExceptionMemAccess( void );
void ExceptionInfoReport( unsigned int u32_exception_num );
void RebootTimeReport( void );


void ExceptionMemTestCall1( unsigned int *pu32_mem_region );
void ExceptionMemTestCall2( unsigned int *pu32_mem_region );
void ExceptionMemTestCall3( unsigned int *pu32_mem_region );
void ExceptionMemTestCall4( unsigned int *pu32_mem_region );
void ExceptionMemTestCall5( unsigned int *pu32_mem_region );
void ExceptionMemTestCall6( unsigned int *pu32_mem_region );
void ExceptionMemTestCall7( unsigned int *pu32_mem_region );
void ExceptionMemTestCall8( unsigned int *pu32_mem_region );
void StackAddressReport( void );


/************************** Implementation *****************************/


/*****************************************************************************/
/**
* Main function.
*
* @param	None.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#ifndef TESTAPP_GEN
int main(void)
{
	u32 Reg;

	xil_printf("MMU example begin.\r\n");

	StackAddressReport( );


	/*  Set domain control register in order to make MMU table valid */
	Reg =0x55555555;
	mtcp(XREG_CP15_DOMAIN_ACCESS_CTRL, Reg);

	
	//system_control_register_report( );
	//auxiliary_control_register_report( );
	//program_status_register_report( );
	//secure_config_register_report( );
		

	/* Standalone program '.text' section protection */	
    printf("\n\rSet \'text\' section: %p as read-only.\n\r", __text_start);
    SetMmuTblCacheAndAccessAttributes( (unsigned int)__text_start, 
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE,
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE, 1, 0, 0,
    		ARM_MMU_TBL_PRVLG_READ_ONLY_USER_READ_ONLY );
    //ReportMmuTblAttributes( (unsigned int)__text_start );
	
	/* Standalone program 'init',  'fini', '.rodata' section protection */	
    printf("\n\rSet \'init\' section: %p as read-only.\n\r", __init_start);
    SetMmuTblCacheAndAccessAttributes( (unsigned int)__init_start, 
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE,
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE, 1, 0, 0,
    		ARM_MMU_TBL_PRVLG_READ_ONLY_USER_READ_ONLY );
    //ReportMmuTblAttributes( (unsigned int)__init_start );

	/*	 
	.mmu_tbl (ALIGN(16384)) : {
	   . = ALIGN(0x100000);
	   __mmu_tbl_protect_hole_begin = .;
	   . += 0x100000;
	   __mmu_tbl_start = .;
	   *(.mmu_tbl)
	   . = ALIGN(0x100000);
	   __mmu_tbl_end = .;
	} > ps7_ddr_0_S_AXI_BASEADDR
       */
	printf("\n\rSet \'MMU table\' section: %p as read-only.\n\r", __mmu_tbl_protect_hole_begin);
    SetMmuTblCacheAndAccessAttributes( (unsigned int)__mmu_tbl_protect_hole_begin,
			ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE,
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE, 1, 0, 0,
    		ARM_MMU_TBL_PRVLG_READ_ONLY_USER_READ_ONLY );
    //ReportMmuTblAttributes( __mmu_tbl_protect_hole_begin );
	
	
	/* Standalone program '.heap' section protection */	
	/*	 
	.heap (NOLOAD) : {
	   . = ALIGN(0x100000);
	   __heap_protect_hole_begin = .;
	   . += 0x100000;
	   _heap = .;
	   HeapBase = .;
	   _heap_start = .;
	   . += _HEAP_SIZE;
	   _heap_end = .;
	   HeapLimit = .;
	   . = ALIGN(0x100000);
	} > ps7_ddr_0_S_AXI_BASEADDR
       */
	printf("\n\rSet the MMU section below \'heap\' section: %p as read-only.\n\r", __heap_protect_hole_begin);
    SetMmuTblCacheAndAccessAttributes( (unsigned int)__heap_protect_hole_begin,
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE,
    		ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE, 1, 0, 0,
    		ARM_MMU_TBL_PRVLG_READ_ONLY_USER_READ_ONLY );
    //ReportMmuTblAttributes( __heap_protect_hole_begin );


	/* Standalone program '.stack' section protection */	
	/*	 
	. = ALIGN(0x100000);
	__stack_protect_hole_begin = .;
	. += 0x100000;
	_stack_end = .;
	. += _STACK_SIZE;
	__stack = .;
	__stack = __stack;
	   */
	printf("\n\rSet the MMU section below \'stack\' section: %p as read-only.\n\r", __stack_protect_hole_begin);
	SetMmuTblCacheAndAccessAttributes( (unsigned int)__stack_protect_hole_begin,
			ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE,
			ARM_MMU_TBL_WRITE_BACK_WRITE_ALLOCATE, 1, 0, 0,
			ARM_MMU_TBL_PRVLG_READ_ONLY_USER_READ_ONLY );
	//ReportMmuTblAttributes( __stack_protect_hole_begin );


	/* Setup exception and interrupt handling function and enable them */
	SetupIntrSystem( &IntcInstance );
	//Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_SWI_INT, ExceptionSwiHandler, NULL);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_DATA_ABORT_INT, ExceptionDataAbortHandler, NULL);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_PREFETCH_ABORT_INT, ExceptionPrefetchAbortHandler, NULL);

	ExceptionMemTestCall4( (unsigned int *)__text_start );
	ExceptionMemTestCall5( (unsigned int *)__init_start );
	ExceptionMemTestCall6(  (unsigned int *)__mmu_tbl_protect_hole_begin );
	ExceptionMemTestCall7( (unsigned int *)__heap_protect_hole_begin );
	ExceptionMemTestCall8( (unsigned int *)__stack_protect_hole_begin );
	
	{
		int i;
		
		for( i=0; ; i++ )
		{
			xil_printf("MMU example end\r\n");
			sleep(5);
		}
	}

	return XST_SUCCESS;
}
#endif


void ExceptionSwiHandler(void *Data)
{
	u32_swi_num++;
	dbg_print("\n\r\n\r");
	dbg_print_var(u32_swi_num);

	ExceptionInfoReport( u32_swi_num );
}



void ExceptionDataAbortHandler(void *Data)
{

#if   0
	register unsigned int u32_link_reg;
	register unsigned int u32_pc_reg;
	register unsigned int u32_stack_pointer_reg;

	u32_link_reg = mflr( );
	u32_pc_reg = mfpc( );
	u32_stack_pointer_reg = mfsp( );
#endif	/*  */

	u32_data_abort_num++;
	dbg_print("\n\r\n\r");
	dbg_print_var(u32_data_abort_num);

	ExceptionInfoReport( u32_data_abort_num );
}



void ExceptionPrefetchAbortHandler(void *Data)
{

	u32_prefetch_abort_num++;
	dbg_print("\n\r\n\r");
	dbg_print_var(u32_prefetch_abort_num);
	
	ExceptionInfoReport( u32_prefetch_abort_num );
}



/*****************************************************************************/
/**
*
* This function sets up the interrupt system such that interrupts can occur
* for the device.
*
* @param	IntcInstancePtr is a pointer to the instance of XScuGic driver.
* @param	TimerInstancePtr is a pointer to the instance of XScuTimer
*		driver.
* @param	TimerIntrId is the Interrupt Id of the XScuTimer device.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
static int SetupIntrSystem(XScuGic *IntcInstancePtr )
{
	int Status;
	XScuGic_Config *IntcConfig;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	Xil_ExceptionInit();

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				IntcInstancePtr);

	/*
	 * Enable interrupts in the Processor.
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}







/****************************************************************************
*
* Read and report  c5, Data Fault Status Register (DFSR)
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void data_fault_status_register_report( void )
{
	register unsigned int data_fault_status;

	struct reg_defintion2   data_fault_status_reg_definition2[] =
	{
        /*    Type                                 a_shift_num        a_mask      b_shift_num        b_mask              value      name                               info          */
        {  REG_BITS_TYPE_CHECK,         0x1,         0x0,          12,         0x1,         0x1, "External abort type", " external aborts " },
        {  REG_BITS_TYPE_CHECK,         0x1,         0x0,          11,         0x1,         0x1, "Write not Read bit", "Abort caused by a write access." },
        {  REG_BITS_TYPE_CHECK,         0x1,         0x0,          11,         0x1,         0x0, "Write not Read bit", "Abort caused by a read access." },
        {  REG_BITS_TYPE_SHOW_DEC,      0x1,         0x0,           4,         0xf,         0x0, "MMU Domain", "The MMU domain of the fault address." },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x1, "Fault status bits.", "Alignment fault" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x4, "Fault status bits.", "cache maintenance fault" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0xc, "Fault status bits.", "Translation table walk synchronous external abort 1st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0xe, "Fault status bits.", "Translation table walk synchronous external abort 2st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x1c, "Fault status bits.", "Translation table walk synchronous parity error 1st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x1e, "Fault status bits.", "Translation table walk synchronous parity error 2st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x5, "Fault status bits.", "Translation fault section" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x7, "Fault status bits.", "Translation fault page" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x3, "Fault status bits.", "Access Flag fault section" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x6, "Fault status bits.", "Access Flag fault page" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0x9, "Fault status bits.", "Domain fault section" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0xb, "Fault status bits.", "Domain fault page" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0xd, "Fault status bits.", "Permission fault section" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0xf, "Fault status bits.", "Permission fault page" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0x2, "Fault status bits.", "Debug event" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x8, "Fault status bits.", "Synchronous external abort" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x14, "Fault status bits.", "Lockdown" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x1a, "Fault status bits.", "Coprocessor abort" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x19, "Fault status bits.", "Memory access synchronous parity error" },
		{  REG_BITS_TYPE_CHECK, 		0x1,		 0x0,		   11,		   0x0, 	   0x16, "Fault status bits.", "Asynchronous external abort" },
		{  REG_BITS_TYPE_CHECK, 		0x1,		 0x0,		   11,		   0x0, 	   0x18, "Fault status bits.", "Memory access asynchronous parity error" },

        {  0xffffffff,    0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read  Data Fault Status Register (DFSR) */
	data_fault_status = mfcp(XREG_CP15_DATA_FAULT_STATUS);
	print_var_hex(data_fault_status);


	/*
	 * Data Fault Status Register (DFSR)
	 */
	register2_report( data_fault_status, "Data Fault Status Register (DFSR)", data_fault_status_reg_definition2  );


}








/****************************************************************************
*
* Read and report  c5, Instruction Fault Status Register (IFSR)
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void instr_fault_status_register_report( void )
{
	register unsigned int instr_fault_status;

	struct reg_defintion2   instr_fault_status_reg_definition2[] =
	{
        /*    Type                                 a_shift_num        a_mask      b_shift_num        b_mask              value      name                               info          */
        {  REG_BITS_TYPE_CHECK,         0x1,         0x0,          12,         0x1,         0x1, "External abort type", " external aborts " },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0xc, "Fault status bits.", "Translation table walk synchronous external abort 1st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0xe, "Fault status bits.", "Translation table walk synchronous external abort 2st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x1c, "Fault status bits.", "Translation table walk synchronous parity error 1st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x1e, "Fault status bits.", "Translation table walk synchronous parity error 2st level" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x5, "Fault status bits.", "Translation fault section" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x7, "Fault status bits.", "Translation fault page" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x3, "Fault status bits.", "Access Flag fault section" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x6, "Fault status bits.", "Access Flag fault page" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0x9, "Fault status bits.", "Domain fault section" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0xb, "Fault status bits.", "Domain fault page" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0xd, "Fault status bits.", "Permission fault section" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0xf, "Fault status bits.", "Permission fault page" },
		{  REG_BITS_TYPE_CHECK, 		  6,		0x10,			0,		   0xf, 		0x2, "Fault status bits.", "Debug event" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,         0x8, "Fault status bits.", "Synchronous external abort" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x14, "Fault status bits.", "Lockdown" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x1a, "Fault status bits.", "Coprocessor abort" },
        {  REG_BITS_TYPE_CHECK,           6,        0x10,           0,         0xf,        0x19, "Fault status bits.", "Memory access synchronous parity error" },
        {  0xffffffff,    0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff,  0xffffffff, "", "" }
	};

	/* read  Instruction Fault Status Register (IFSR) */
	instr_fault_status = mfcp(XREG_CP15_DATA_FAULT_STATUS);
	print_var_hex(instr_fault_status);


	/*
	 * Instruction Fault Status Register (IFSR)
	 */
	register2_report( instr_fault_status, "Instruction Fault Status Register (IFSR)", instr_fault_status_reg_definition2  );


}

int	SysModeStackCheck( unsigned int u32_stack_pointer )
{
	int i;
	int i_frame_valid_flag=0;
	unsigned int *u32_frame_pointer_try;
	unsigned int *u32_frame_pointer_try2;
	unsigned int *u32_frame_pointer_try3;
	unsigned int *u32_frame_pointer_try4;
	unsigned int *u32_frame_pointer_try5;
	unsigned int *u32_frame_pointer_try6;
	
	if( (u32_stack_pointer < (unsigned int)_stack_end )
		|| (u32_stack_pointer > (unsigned int)__stack ) 
		|| ( 0 != ((unsigned int)u32_stack_pointer&0x3) ) )
	{
		print_var_hex( _stack_end );
		print_var_hex( __stack );
		xil_printf("Invalid stack pointer: 0x%08x\n\r", u32_stack_pointer );
		return -1;
	}
	xil_printf("\n\rAnalyze stack pointer: 0x%08x\n\r", u32_stack_pointer );

	/* FP always pointers to the LR register value */
	/* FP always is below the LR register value */
	/* Try to find frame pointer. it is always pointed by the SP */
	i_frame_valid_flag=0;
	for( i=0; i<200; i++ )
	{
		dbg_print("No.%d try for stack analysis.\n\r", i );
		dbg_print_var_hex( _stack_end );
		dbg_print_var_hex( __stack );

		/* Frame pointer points to LR. */
		/* Frame pointer is stored below LR. */
		u32_frame_pointer_try = (unsigned int *)(u32_stack_pointer-0x20+i*4);
		if(  (unsigned int)u32_frame_pointer_try < (unsigned int)_stack_end )
		{
			/* Out of stack space, reset it to the lower end of stack */
			u32_frame_pointer_try = (unsigned int *)(_stack_end);
		}		
		u32_frame_pointer_try2 = (unsigned int *)((*u32_frame_pointer_try)-4);
		dbg_print_var_hex( u32_frame_pointer_try );
		dbg_print_var_hex( u32_frame_pointer_try2 );
		if( ((unsigned int)u32_frame_pointer_try2 < (unsigned int)_stack_end )
			|| ((unsigned int)u32_frame_pointer_try2 > (unsigned int)__stack )
			|| ( 0 != ((unsigned int)u32_frame_pointer_try2&0x3) ) )
		{
			/* The location it points to is out of stack space. 
			  * Invalid stack frame.
			 */
			continue;
		}		
		//if( ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try2 )<0x10 )
		if( 8 == ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try2 ) )
		{
			/* Close to stack top, only 1 pointer is OK. ? */
			i_frame_valid_flag=1;
			break;
		}
		
		u32_frame_pointer_try3 = (unsigned int *)((*u32_frame_pointer_try2)-4);
		dbg_print_var_hex( u32_frame_pointer_try3 );
		if( ( (unsigned int)u32_frame_pointer_try3 < (unsigned int)_stack_end )
			|| ((unsigned int)u32_frame_pointer_try3 > (unsigned int)__stack )
			|| ( 0 != ((unsigned int)u32_frame_pointer_try3&0x3) ) )
		{
			continue;
		}
		//if( ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try3 )<0x10 )
		if( 8 == ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try3 ) )
		{
			/* Very close to stack top, only 2 pointer is OK. ? */
			i_frame_valid_flag=2;
			break;
		}
		
		u32_frame_pointer_try4 = (unsigned int *)((*u32_frame_pointer_try3)-4);
		dbg_print_var_hex( u32_frame_pointer_try4 );
		if( ( (unsigned int)u32_frame_pointer_try4 < (unsigned int)_stack_end )
			|| ((unsigned int)u32_frame_pointer_try4 > (unsigned int)__stack )
			|| ( 0 != ((unsigned int)u32_frame_pointer_try4&0x3) ) )
		{
			continue;
		}
		//if( ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try4 )<0x10 )
		if( 8 == ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try4 ) )
		{
			/* Very close to stack top, only 3 pointer is OK. ? */
			i_frame_valid_flag=3;
			break;
		}

		u32_frame_pointer_try5 = (unsigned int *)((*u32_frame_pointer_try4)-4);
		dbg_print_var_hex( u32_frame_pointer_try5 );
		if( ( (unsigned int)u32_frame_pointer_try5 < (unsigned int)_stack_end )
			|| ((unsigned int)u32_frame_pointer_try5 > (unsigned int)__stack )
			|| ( 0 != ((unsigned int)u32_frame_pointer_try5&0x3) ) )
		{
			continue;
		}
		//if( ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try5 )<0x10 )
		if( 8 == ( (unsigned int)__stack-(unsigned int)u32_frame_pointer_try5 ) )
		{
			/* Very close to stack top, only 4 pointer is OK. ? */
			i_frame_valid_flag=4;
			break;
		}

		u32_frame_pointer_try6 = (unsigned int *)((*u32_frame_pointer_try5)-4);
		dbg_print_var_hex( u32_frame_pointer_try6 );
		if( ( (unsigned int)u32_frame_pointer_try6 < (unsigned int)_stack_end )
			|| ((unsigned int)u32_frame_pointer_try6 > (unsigned int)__stack )
			|| ( 0 != ((unsigned int)u32_frame_pointer_try6&0x3) ) )
		{
			continue;
		}

		/* 5 pointer is used to find the stack. ? */
		/* Find 5 valid frame pointer */
		i_frame_valid_flag=5;
		break;
	}

	if( 0==i_frame_valid_flag )
	{
		xil_printf("Invalid stack: 0x%08x after %d try.\n\r", u32_stack_pointer, i );
		return -1;
	}

	/* u32_frame_pointer_try has valid stack frame pointer */
	dbg_print_var_hex( i_frame_valid_flag );
	dbg_print_var_hex( u32_frame_pointer_try );
	i_frame_valid_flag=0;
	//u32_frame_pointer_try--;  /* Modify pointer to point to frame pointer. */
	for( i=1; i<200; i++ )
	{
		xil_printf("\n\rNo. %d Frame at 0x%08x\n\r", i, u32_frame_pointer_try );
		xil_printf("FP: 0x%08x, LR: 0x%08x\n\r", u32_frame_pointer_try[0], u32_frame_pointer_try[1] );
		if(  (unsigned int)u32_frame_pointer_try == ((unsigned int)__stack -8 ) )
		{
			xil_printf("Last frame is found.\n\r" );
			break;
		}

		u32_frame_pointer_try = (unsigned int *)((*u32_frame_pointer_try)-4);
		if( (  (unsigned int)u32_frame_pointer_try < (unsigned int)_stack_end )
			|| (  (unsigned int)u32_frame_pointer_try > (unsigned int)__stack )
			|| ( 0 != ((unsigned int)u32_frame_pointer_try&0x3) ) )
		{
			xil_printf("No more Frame.\n\r\n\r" );
			break;
		}
	}
	dbg_print("\n\r" );

	return 0;
}

void ExceptionInfoReport( 	unsigned int u32_exception_num )
{
	unsigned int *pu32_stack_pointer;

	unsigned int data_fault_address;
	unsigned int aux_data_fault_status;
	
	unsigned int inst_fault_status;
	unsigned int inst_fault_address;
	unsigned int aux_inst_fault_status;

	unsigned int u32_exception_mode_cpsr;
	unsigned int u32_system_mode_cpsr;
	unsigned int u32_system_mode_r13_sp;
	unsigned int u32_system_mode_r14_lr;
	
	/* Get information about exception */
	data_fault_status_register_report( );
	data_fault_address = mfcp(XREG_CP15_DATA_FAULT_ADDRESS);
	print_var_hex(data_fault_address);

	aux_data_fault_status = mfcp(XREG_CP15_AUX_DATA_FAULT_STATUS);
	print_var_hex(aux_data_fault_status);

	instr_fault_status_register_report( );
	inst_fault_status = mfcp(XREG_CP15_INST_FAULT_STATUS);
	print_var_hex(inst_fault_status);

	inst_fault_address = mfcp(XREG_CP15_INST_FAULT_ADDRESS);
	print_var_hex(inst_fault_address);

	aux_inst_fault_status = mfcp(XREG_CP15_AUX_INST_FAULT_STATUS);
	print_var_hex(aux_inst_fault_status);

	/*
		MVN (Move Not) performs a bitwise NOT operation on the operand2 value, and places the result 
		into Rd.
		mrs r0, cpsr			* get the current PSR *
		mvn r1, #0x1f			* set up the system stack pointer * 
		and r2, r1, r0
		orr r2, r2, #0x1F			* SYS mode *
		msr cpsr, r2
		ldr r13,=SYS_stack			* SYS stack pointer *
	*/
	u32_exception_mode_cpsr = mfcpsr();
	u32_system_mode_cpsr = u32_exception_mode_cpsr&(~0x1f);
	u32_system_mode_cpsr |= 0x1f;
	mtcpsr( u32_system_mode_cpsr );

	/* B1.6.15 Data Abort exception  
	   new_lr_value = if CPSR.T == 1 then PC+4 else PC;
			T, bit [5]  Thumb execution state bit. This bit and the J execution state bit, bit [24], determine the 
			instruction set state of the processor, ARM, Thumb, Jazelle, or ThumbEE. ISETSTATE on 
			page A2-15 describes the encoding of these bits. CPSR.J and CPSR.T are the same bits as 
			ISETSTATE.J and ISETSTATE.T respectively. For more information, see Instruction set 
			states on page B1-23.
			For details of how these bits can be accessed see Accessing the execution state bits on 
			page B1-18.
	  So for Xilinx Standalone application,
	   new_lr_value = PC;
	   */
	u32_system_mode_r13_sp = mfgpr(13);
	u32_system_mode_r14_lr = mfgpr(14);
	mtcpsr( u32_exception_mode_cpsr );  // restore to the exception mode
	print_var_hex(u32_system_mode_r13_sp);
	print_var_hex(u32_system_mode_r14_lr);

	xil_printf("\n\r\n\r");
	xil_printf("Dump stack of system mode that causes this exception::\n\r");

	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0)-0x20 );
	print_pointer_hex_8(pu32_stack_pointer);
	
	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0) );
	print_pointer_hex_16(pu32_stack_pointer);

	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0)+0x40 );
	print_pointer_hex_16(pu32_stack_pointer);
	
#if  1
	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0)+0x80 );
	if( (unsigned int)pu32_stack_pointer < (unsigned int)__stack )
	{
		print_pointer_hex_16(pu32_stack_pointer);
	}
	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0)+0xc0 );
	if( (unsigned int)pu32_stack_pointer < (unsigned int)__stack )
	{
		print_pointer_hex_16(pu32_stack_pointer);
	}
	
	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0)+0x100 );
	if( (unsigned int)pu32_stack_pointer < (unsigned int)__stack )
	{
		print_pointer_hex_16(pu32_stack_pointer);
	}
	
	pu32_stack_pointer = (unsigned int *)( (u32_system_mode_r13_sp&0xffffffe0)+0x140 );
	if( (unsigned int)pu32_stack_pointer < (unsigned int)__stack )
	{
		print_pointer_hex_16(pu32_stack_pointer);
	}
#endif /*  */	

	SysModeStackCheck( u32_system_mode_r13_sp );


#if 0
	if( 1 == (u32_exception_num%1000) )
	{
		system_control_register_report( );
		auxiliary_control_register_report( );
		
		program_status_register_report( );
		secure_config_register_report( );
	}
#endif

	/* 
	 *  Just for demo
	  * Skip the instruction caused the exception 
	  *
	  */
	u32_system_mode_r14_lr = u32_system_mode_r14_lr+4;
	mtgpr(14, u32_system_mode_r14_lr);
	dbg_print("Set LR as 0x%08x\n\r", u32_system_mode_r14_lr);

	dbg_print("\n\r\n\r\n\r");
	sleep(1);
	
}


void ExceptionMemWrite( unsigned int *pu32_mem_region )
{
	static unsigned int uiExecNum=0;

	uiExecNum++;
	xil_printf("\n\r\n\r\n\rWrite memory address: 0x%08x\n\r", pu32_mem_region);
	*pu32_mem_region = uiExecNum;
	
	//print_pointer_hex_4(pu32_mem_region);
	dbg_print_var(u32_prefetch_abort_num);
	dbg_print_var(u32_data_abort_num);
	dbg_print_var(u32_swi_num);
}



void ExceptionMemTestCall1( unsigned int *pu32_mem_region )
{
	ExceptionMemWrite( pu32_mem_region );
}


void ExceptionMemTestCall2( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall1( pu32_mem_region );
}

void ExceptionMemTestCall3( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall2( pu32_mem_region );
}

void ExceptionMemTestCall4( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall3( pu32_mem_region );
}

void ExceptionMemTestCall5( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall4( pu32_mem_region );
}

void ExceptionMemTestCall6( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall5( pu32_mem_region );
}

void ExceptionMemTestCall7( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall6( pu32_mem_region );
}

void ExceptionMemTestCall8( unsigned int *pu32_mem_region )
{
	ExceptionMemTestCall7( pu32_mem_region );
}

void StackAddressReport( void )
{

	print_var_hex( __text_start );
	print_var_hex( __text_end );

	print_var_hex( __init_start );
	print_var_hex( __init_end );

	print_var_hex( __fini_start );
	print_var_hex( __fini_end );

	print_var_hex( __rodata_start );
	print_var_hex( __rodata_end );

	print_var_hex( __rodata1_start );
	print_var_hex( __rodata1_end );

	print_var_hex( __sdata2_start );
	print_var_hex( __sdata2_end );

	print_var_hex( __sbss2_start );
	print_var_hex( __sbss2_end );

	print_var_hex( __data_start );
	print_var_hex( __data_end );
	print_var_hex( __bss_start );
	print_var_hex( __bss_end );

	print_var_hex( __mmu_tbl_protect_hole_begin );
	print_var_hex( __mmu_tbl_start );
	print_var_hex( __mmu_tbl_end );


	print_var_hex( __heap_protect_hole_begin );
	print_var_hex( _heap_start );
	print_var_hex( _heap_end );
	print_var_hex( __stack_protect_hole_begin );
	print_var_hex( _stack_end );
	print_var_hex( __stack );
	print_var_hex( _irq_stack_end );
	print_var_hex( __irq_stack );
	print_var_hex( _supervisor_stack_end );
	print_var_hex( __supervisor_stack );
	print_var_hex( _abort_stack_end );
	print_var_hex( __abort_stack );
	print_var_hex( _fiq_stack_end );
	print_var_hex( __fiq_stack );
	print_var_hex( _undef_stack_end );
	print_var_hex( __undef_stack );
	printf("\n\r\n\r");

}
