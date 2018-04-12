extern unsigned char _bss_begin;
extern unsigned char _bss_end;
extern unsigned char _rom_data_begin;
extern unsigned char _data_begin;
extern unsigned char _data_end;
extern unsigned char _ctors_begin;
extern unsigned char _ctors_end;

typedef void(*function_type)();

void start(void)
{
	unsigned char *to_pt;
	unsigned char *from_pt;
	unsigned char value;
	unsigned int ctor_pt;

	// Inicializa seccion bss
	to_pt = &_bss_begin;
	while (to_pt != &_bss_end)
		*to_pt++ = 0;


	// Inicializa seccion data
	from_pt = &_rom_data_begin;
	to_pt = &_data_begin;
	while (to_pt != &_data_end) {
		asm volatile("lpm %0, Z" : "=r" (value) : "z" (from_pt++) :);
		*to_pt++ = value;
	}

	from_pt = &_ctors_begin;
	while (from_pt != &_ctors_end) {
		asm volatile("lpm %0, Z" : "=r" (value) : "z" (from_pt++) :);
		ctor_pt = value;
		asm volatile("lpm %0, Z" : "=r" (value) : "z" (from_pt++) :);
		ctor_pt = (ctor_pt & 0x00FF) | (value << 8);
		((function_type)ctor_pt)();
	}

	asm volatile("rjmp main");
}
