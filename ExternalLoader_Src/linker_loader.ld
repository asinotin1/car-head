MEMORY
{
  RAM (xrw)      : ORIGIN = 0x24000000, LENGTH = 512K
}

ENTRY(Init)

SECTIONS
{
  .text :
  {
    KEEP(*(.text.Init))
    KEEP(*(.text.Write))
    KEEP(*(.text.SectorErase))
    KEEP(*(.text.MassErase))
    KEEP(*(.text.Read))
    
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
  } >RAM

  .data :
  {
    *(.data)
    *(.data*)
  } >RAM

  .bss :
  {
    *(.bss)
    *(.bss*)
    *(COMMON)
  } >RAM

  /* Loader Info is required by ST-Link Utility */
  .Loader_Info :
  {
    KEEP(*(.Loader_Info))
  } >RAM
}
