// si5351 Bibliothek
#include "si5351.h"
#include "hardware/i2c.h"
#include "pico/double.h"
#include <stdio.h>

uint8_t Bus;
DivPara MS0, MS1, MS2, PLLA, PLLB;
bool Ckl0_set = false, Ckl2_set = false;

// i2c-Funktionen
void i2cInit(uint8_t bus, uint8_t sda, uint8_t scl){
	Bus = bus;
	i2c_init(I2C_INSTANCE(bus), 400*1000);
	gpio_set_function(sda, GPIO_FUNC_I2C);
	gpio_set_function(scl, GPIO_FUNC_I2C);
	gpio_pull_up(sda);
	gpio_pull_up(scl);
}
void setReg(uint8_t reg, uint8_t data){
	uint8_t m_reg[] = {reg, data};
	i2c_write_blocking(I2C_INSTANCE(Bus), ADDR, m_reg, 2, false);
}
void setRegs(uint8_t reg, uint8_t len, uint8_t *data){
	uint8_t my_reg[len + 1];
	my_reg[0] = reg;
	for(int i=1; i<len + 1; i++) my_reg[i] = data[i - 1];
	i2c_write_blocking(I2C_INSTANCE(Bus), ADDR, my_reg, len + 1, false);
}
uint8_t getReg(uint8_t reg){
	uint8_t my_reg[] = {reg};
	i2c_write_blocking(I2C_INSTANCE(Bus), ADDR, my_reg, 1, false);
	uint8_t ret[1];
	i2c_read_blocking(I2C_INSTANCE(Bus), ADDR, ret, 1, false);
	return ret[0];
}

// Helferfunktionen
void setPLL(DivPara *para, uint pllb){
	// Calc Parameters
	uint32_t pll_p1 = 128 * para->a + ((128 * para->b) / para->c) - 512;
	uint32_t pll_p2 = 128 * para->b - para->c * ((128 * para->b) / para->c);
	pll_p2 = pll_p2 >> 12;
	uint32_t pll_p3 = para->c;
	pll_p3 = pll_p3 >> 12;
	// Write Parameters to register
	uint8_t reg[8];
	reg[0] = (((pll_p3 >> 8) & 0xFF));
	reg[1] = ((pll_p3 & 0xFF));
	reg[2] = (((pll_p1 >> 16) & 0x03));
	reg[3] = (((pll_p1 >> 8) & 0xFF));
	reg[4] = ((pll_p1 & 0xFF));
	reg[5] = (((pll_p3 >> 12) & 0xF0) | ((pll_p2 >> 16) & 0x0F));
	reg[6] = (((pll_p2 >> 8) & 0xFF));
	reg[7] = ((pll_p2 & 0xFF));
	setRegs(26 + pllb * 8, 8, reg);
	uint8_t pll_even = 0x00;
	if(para->even) pll_even = 0x40;
	setReg(22 + pllb, pll_even);
}
void setMS(DivPara *para, uint nr){
	// Calc Parameters
	uint32_t ms_p1 = 128 * para->a + ((128 * para->b) / para->c) - 512;
	uint32_t ms_p2 = 128 * para->b - para->c * ((128 * para->b) / para->c);
	ms_p2 = ms_p2 >> 12;
	uint32_t ms_p3 = para->c;
	ms_p3 = ms_p3 >> 12;
	// Write Parameters to register
	uint8_t my_reg[8];
	my_reg[0] = (((ms_p3 >> 8) & 0xFF));
	my_reg[1] = ((ms_p3 & 0xFF));
	my_reg[2] = (((ms_p1 >> 16) & 0x03));
	my_reg[3] = (((ms_p1 >> 8) & 0xFF));
	my_reg[4] = ((ms_p1 & 0xFF));
	my_reg[5] = (((ms_p3 >> 12) & 0xF0) | ((ms_p2 >> 16) & 0x0F));
	my_reg[6] = (((ms_p2 >> 8) & 0xFF));
	my_reg[7] = ((ms_p2 & 0xFF));
	setRegs(42 + nr * 8, 8, my_reg);
	uint8_t ms_even = 0x0C;
	if(para->even) ms_even = 0x4C;
	setReg(16 + nr, ms_even);// enable output
}
bool writeDivsor(uint8_t ckl, uint8_t pll_a, uint32_t pll_b, uint32_t pll_c, bool pll_int, bool pll_frac, \
				uint16_t ms_a, uint32_t ms_b, uint32_t ms_c, bool ms_int, bool ms_frac, uint8_t r_div){
	if(ckl < 2){
		PLLA.a = pll_a;
		PLLA.b = pll_b;
		PLLA.c = pll_c;
		PLLA.even = pll_int;
		PLLA.frac = pll_frac;
		setPLL(&PLLA, 0);
		MS0.a = ms_a;
		MS0.b = ms_b;
		MS0.c = ms_c;
		MS0.even = ms_int;
		MS0.frac = false;
		setMS(&MS0, 0);
		MS1.a = ms_a;
		MS1.b = ms_b;
		MS1.c = ms_c;
		MS1.even = ms_int;
		MS1.frac = ms_frac;
		setMS(&MS1, 1);
		uint8_t stat = getReg(17);// Set ckl1 phase invert
		stat |= 0x10;
		setReg(17, stat);
		if(r_div > 1){
			stat = getReg(44);
			stat |= 0x70;
			setReg(44, stat);
			stat = getReg(52);
			stat |= 0x70;
			setReg(52, stat);
		}
		return true;
	}else{
		PLLB.a = pll_a;
		PLLB.b = pll_b;
		PLLB.c = pll_c;
		PLLB.even = pll_int;
		PLLB.frac = pll_frac;
		setPLL(&PLLB, 1);
		MS2.a = ms_a;
		MS2.b = ms_b;
		MS2.c = ms_c;
		MS2.even = ms_int;
		MS2.frac = ms_frac;
		setMS(&MS2, 2);
		uint8_t stat = getReg(18);// Set ckl2 PLLB
		stat |= 0x20;
		setReg(18, stat);
		if(r_div > 1){
			stat = getReg(60);
			stat |= 0x70;
			setReg(60, stat);
		}
		return true;
	}
}

// Ausfuehrung
bool setParas(uint8_t ckl, uint32_t f_out){
	bool pll_int = false, ms_int = false;
	uint8_t pll_a = 1;
	uint16_t ms_a = 1;
	uint32_t pll_b = 0, pll_c = 1, ms_b = 0, ms_c = 1;
	uint32_t f_vco, f_ms;
	if( (f_out > (150 * 1000 * 1000)) ){
		ms_a = 4;
		ms_int = true;
		f_vco = 4 * f_out;
		pll_a = f_vco / F_XTAL;
		bool frac = true;
		if(f_vco % F_XTAL == 0){ frac=false; if(pll_a % 2 == 0) pll_int = true; }
		else{ pll_b = (f_vco % F_XTAL); pll_c = F_XTAL; }
		writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, frac, ms_a, ms_b, ms_c, ms_int, false, 1);
		if(ckl < 2){
			uint8_t stat = getReg(44);
			stat |= 0x0C;
			setReg(44, stat);
			stat = getReg(44 + 8);
			stat |= 0x0C;
			setReg(44 + 8, stat);
			return true;
		}
		else{
			uint8_t stat = getReg(44 + 16);
			stat |= 0x0C;
			setReg(44+ 16, stat);
			return true;
		}
	}
	else if( (f_out > (112500 * 1000)) && (f_out <= (150 * 1000 * 1000)) ){
		ms_a = 6;
		ms_int = true;
		f_vco = f_out * ms_a;
		pll_a = f_vco / F_XTAL;
		bool frac = true;
		if(f_vco % F_XTAL == 0){ frac=false; if(pll_a % 2 == 0) pll_int = true; }
		else{ pll_b = (f_vco % F_XTAL); pll_c = F_XTAL; }
		return writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, frac, ms_a, ms_b, ms_c, ms_int, true, 1);
	}
	else{
		uint8_t r_div = 1;
		if(f_out < 500 * 1000) r_div = 128;
		for(int i=DIV_PLL_MAX; i>=DIV_PLL_MIN; i--){// pll & ms = EvInt
			if( ((i * F_XTAL) % (f_out * r_div) == 0) \
			  &&(((i * F_XTAL) / (f_out * r_div)) % 2 == 0)
			  &&((i * F_XTAL) % 2 == 0) ){
				  pll_a = i;
				  ms_a = (i * F_XTAL) / (f_out * r_div);
				  pll_int = true;
				  ms_int = true;
				  return writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, false, ms_a, ms_b, ms_c, ms_int, false, r_div);
			}
		}
		for(int i=DIV_PLL_MAX; i>=DIV_PLL_MIN; i--){// pll=int, ms=EvInt
			if( ((i * F_XTAL) % (f_out * r_div) == 0) \
			  &&(((i * F_XTAL) / (f_out * r_div)) % 2 == 0) ){
				  pll_a = i;
				  ms_a = (i * F_XTAL) / (f_out * r_div);
				  ms_int = true;
				  return writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, false, ms_a, ms_b, ms_c, ms_int,false, r_div);
			}
		}
		for(int i=DIV_PLL_MAX; i>=DIV_PLL_MIN; i--){// pll=EvInt, ms=int
			if( ((i * F_XTAL) % (f_out * r_div) == 0) \
			  &&((i * F_XTAL) % 2 == 0) ){
				  pll_a = i;
				  ms_a = (i * F_XTAL) / (f_out * r_div);
				  pll_int = true;
				  return writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, false, ms_a, ms_b, ms_c, ms_int, false, r_div);
			}
		}
		for(int i=DIV_PLL_MAX; i>=DIV_PLL_MIN; i--){// pll & ms = integer
			if( ((i * F_XTAL) % (f_out * r_div) == 0) ) {
				  pll_a = i;
				  ms_a = (i * F_XTAL) / (f_out * r_div);
				  return writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, false, ms_a, ms_b, ms_c, ms_int, false, r_div);
			}
		}
		for(int i=DIV_PLL_MAX; i>=DIV_PLL_MIN; i -= 2){// pll=EvInt, ms=frac
			ms_a = (i * F_XTAL) / (f_out * r_div);
			if( (ms_a >= 8) && (ms_a <= 2048) ){
				ms_b = (i * F_XTAL) % (f_out * r_div);
				ms_c = f_out;
				pll_int = true;
				pll_a = i;
				return writeDivsor(ckl, pll_a, pll_b, pll_c, pll_int, false, ms_a, ms_b, ms_c, ms_int, true, r_div);
			}
		}
	}
	return false;
}

// General-Funktionen
void disable_out(){
	setReg(3, 0xFF);
	for(int i=16; i<18; i++) setReg(i, 0x80);// CKL disble
}
void enable_out(){
	setReg(3, 0xF8);// enable ckl0 - ckl3
	uint8_t stat;// Register 3 enable
	if(Ckl0_set){
		stat = getReg(16);
		stat &= 0x7F;
		setReg(16, stat);
		stat = getReg(17);
		stat &= 0x7F;
		setReg(17, stat);
	}
	if(Ckl2_set){
		stat = getReg(18);
		stat &= 0x7F;
		setReg(18, stat);
	}
	setReg(177, 0xAC);// reset PLL_A PLL_B
	setReg(1, 0x00);// reset Stikybit
}

void si5351_setLoad(uint8_t load){
	uint8_t reg = 0x22 | (load << 6);
	disable_out();
	setReg(183, reg);
	enable_out();
}
void si5351_setFreq(uint32_t f_out0, uint32_t f_out2){
	disable_out();
	if(f_out0 >= 8000 && f_out0 <= 150 * 1000 * 1000){ Ckl0_set = true; setParas(0, f_out0); }
	else Ckl0_set = false;
	if(f_out2 >= 8000 && f_out2 <= 150 * 1000 * 1000){ Ckl2_set = true; setParas(2, f_out2); }
	else Ckl2_set = false;
	if(Ckl0_set || Ckl2_set) enable_out();
}
void si5351_init(uint8_t bus, uint8_t sda, uint8_t scl, uint32_t freq0, uint32_t freq2){
	i2cInit(bus, sda, scl);
	disable_out();
	setReg(9, 0xFF);// disable PIN_ENABLE
	setReg(15, 0x00);// PLL_source=XTAL, CKL_DIV=1
	setReg(24, 0x2A);// Disable-State = Highimpedance
	setReg(183, 0xD2);// XTAL_CL = 10pF
	busy_wait_ms(100);
	si5351_setFreq(freq0, freq2);
}
