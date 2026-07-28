MEMORY
{
  RAM (xrw) : ORIGIN = 0x24000000, LENGTH = 512K /* AXI SRAM */
}

SECTIONS
{
  .text :
  {
    KEEP(*(.Dev_info))
    KEEP(*(.text.Init))
    KEEP(*(.text.Write))
    KEEP(*(.text.SectorErase))
    KEEP(*(.text.MassErase))
    *(.text*)
    *(.rodata*)
  } > RAM
  
  .data : 
  {
    *(.data*)
  } > RAM
  
  .bss :
  {
    *(.bss*)
    *(COMMON)
  } > RAM
}
