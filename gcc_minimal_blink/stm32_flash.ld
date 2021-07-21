/*
*****************************************************************************

*****************************************************************************
*/




/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
  MEMORY_B1 (rx)  : ORIGIN = 0x60000000, LENGTH = 0K
}

/* Entry Point */
ENTRY(main)

/* Define output sections */
SECTIONS
{

  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH
  
  /* program code into FLASH */
  .text : ALIGN(4)
  {
    *(.vector_table) /* Vector table */
    *(.text) /* Program code */
    KEEP (*(.vector_table))
  } >FLASH
}
