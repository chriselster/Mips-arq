#include <stdio.h>
#include "shell.h"

uint32_t pc;
uint32_t hi; 
uint32_t lo;

void read(){
	pc = mem_read_32(CURRENT_STATE.PC);
    hi = CURRENT_STATE.HI;
    lo = CURRENT_STATE.LO;
}



void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
    NEXT_STATE.PC =  CURRENT_STATE.PC + 4;
    read();
    uint32_t op = pc/0x4000000;
    printf("%x\n",pc);

    uint32_t rs = pc & 0x3e00000;
 	rs = rs >> 21;
 	uint32_t rt = pc & 0x1f0000;
 	rt = rt >> 16;

 	uint32_t val_rtu = CURRENT_STATE.REGS[rt];
    uint32_t val_rsu = CURRENT_STATE.REGS[rs];
    int val_rt = CURRENT_STATE.REGS[rt];
    int val_rs = CURRENT_STATE.REGS[rs];
    if (op)
    {
    	if (op>3 || op == 1)//é I-type
    	{
    		uint16_t imu = pc & 0xffff;
    		short im = pc & 0xffff;

    		if (op == 9)//ADDIU
    		{
    			val_rt = val_rs + im;
    			NEXT_STATE.REGS[rt] = val_rt;
    			return;
    		}
    		if (op == 8)//ADDI
    		{
    			val_rt = val_rs + im;
    			NEXT_STATE.REGS[rt] = val_rt;
    			return;
    		}

    		if (op == 0xa)//SLTI
			{
				NEXT_STATE.REGS[rt] = val_rs < im;
				return;
			}

			if (op == 0xb)//SLTIU
			{
				NEXT_STATE.REGS[rt] = val_rs < imu;
				return;
			}

			if (op == 0xd)//ORI
			{
				NEXT_STATE.REGS[rt] = val_rs | imu;
				return;
			}

			if (op == 0xe)//XORI
			{
				NEXT_STATE.REGS[rt] = val_rs ^ im;
				return;
			}

			if (op == 0xc)//ANDI
			{
				NEXT_STATE.REGS[rt] = val_rs & im;
				return;
			}

			if (op == 0xf)//LUI
			{
				NEXT_STATE.REGS[rt] = imu << 16;
				return;
			}

			if (op == 0x20)//LB
			{
				val_rs += im;
				char val8_rt = mem_read_32(val_rs);
				NEXT_STATE.REGS[rt] = val8_rt;
				return;
			}

			if (op == 0x21)//LH
			{
				val_rs += im;
				short val16_rt = mem_read_32(val_rs);
				NEXT_STATE.REGS[rt] = val16_rt;
				return;
			}

			if (op == 0x24)//LBU
			{
				val_rs += im;
				val_rtu = mem_read_32(val_rs);
				val_rtu = val_rtu & 0xff;
				NEXT_STATE.REGS[rt] = val_rtu;
				return;
			}

			if (op == 0x25)//LHU
			{
				val_rs += im;
				val_rtu = mem_read_32(val_rs);
				val_rtu = val_rtu & 0xffff;
				NEXT_STATE.REGS[rt] = val_rtu;
				return;
			}

			if (op == 0x23)//LW
			{
				val_rs += im;
				NEXT_STATE.REGS[rt] = mem_read_32(val_rs);
				return;
			}

			if (op == 0x28)//SB
			{
				val_rs += im;
				val_rt = val_rt & 0xff;
				mem_write_32(val_rs,val_rt);
				return;
			}

			if (op == 0x29)//SH
			{
				val_rs += im;
				val_rt = val_rt & 0xffff;
				mem_write_32(val_rs,val_rt);
				return;
			}

			if (op == 0x2b)//SW
			{
				val_rs += im;
				mem_write_32(val_rs,val_rt);
				return;
			}

			if (op == 0x4)//BEQ
			{
				if (val_rs == val_rt){
					printf("%x\n",im );
					im = im << 2;
					printf("%x\n",im );
					NEXT_STATE.PC = (CURRENT_STATE.PC + im + 4);
				}
				return;
			}

			if (op == 0x5)//BNEQ
			{
				if (val_rs != val_rt){
					printf("%x\n",im );
					im = im << 2;
					printf("%x\n",im );
					NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
				}
				return;
			}

			if (op == 0x6){//BLEZ
				if (val_rs <= 0){
					im = im << 2;
					NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
				}
				return;
			}

			if (op == 0x7){//BGTZ
				if (val_rs > 0){
					im = im << 2;
					NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
				}
				return;
			}

			if (op == 0x1)
			{
				switch (rt){
					case 0:
						if (val_rs < 0){//BLTZ
							im = im << 2;
							NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
						}
						break;

					case 1:
						if (val_rs >= 0){//BGEZ
							im = im << 2;
							NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
						}
						break;

					case 0x11:
						if (val_rs >= 0){//BGEZAL
							im = im << 2;
							NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
							NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
						}
						break;


					case 0x10:
						if (val_rs < 0){//BLTZAL
							im = im << 2;
							NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
							NEXT_STATE.PC = CURRENT_STATE.PC + im + 4;
						}
						break;
				}
				return;
			}

    	}
    	else{//é J-type

    		int tgt = pc & 0x3ffffff;
    		tgt = tgt << 2;
    		if (op == 0x2)//J
			{
				NEXT_STATE.PC = tgt;
				return;
			}

			if (op == 0x3)//JAL
			{
				NEXT_STATE.REGS[31] = CURRENT_STATE.PC+4;
				NEXT_STATE.PC = tgt;
				return;
			}

    	}
    }else{//é R-type

    	uint32_t rd = pc & 0xf800;
		rd = rd >> 11;
		uint32_t shamt = pc & 0x7c0;
		shamt = shamt >> 6;
		uint32_t func = pc & 0x3f;
		
		if (func == 0xc)//SYSCALL
		{
			if (CURRENT_STATE.REGS[2] == 10)
			{
				RUN_BIT = 0;
			}
			return;
		}
		if (func == 0x24)//AND
		{
			NEXT_STATE.REGS[rd] = val_rs & val_rt;
			return;
		}

		if (func == 0x25)//OR
		{
			NEXT_STATE.REGS[rd] = val_rs | val_rt;
			return;
		}

		if (func == 0x26)//XOR
		{
			NEXT_STATE.REGS[rd] = val_rs ^ val_rt;
			return;
		}

		if (func == 0x27)//NOR
		{
			NEXT_STATE.REGS[rd] = ~(val_rs | val_rt);
			return;
		}

		if (func == 0x2a)//SLT
		{
			NEXT_STATE.REGS[rd] = val_rs < val_rt;
			return;
		}

		if (func == 0x2b)//SLTU
		{
			NEXT_STATE.REGS[rd] = val_rsu < val_rtu;
			return;
		}



		if (func == 0x18)//MULT
		{
			long result = val_rs * val_rt;
			NEXT_STATE.HI = result >> 32;
			NEXT_STATE.LO = result & 0xffffffff;
			return;
		}

		if (func == 0x19)//MULTU
		{
			uint64_t result = val_rsu * val_rtu;
			NEXT_STATE.HI = result >> 32;
			NEXT_STATE.LO = result & 0xffffffff;
			return;
		}

		if (func == 0x10)//MFHI
		{
			NEXT_STATE.REGS[rd] = hi;
			return;
		}

		if (func == 0x12)//MFLO
		{
			NEXT_STATE.REGS[rd] = lo;
			return;
		}

		if (func == 0x11)//MTHI
		{
			NEXT_STATE.HI = val_rs;
			return;
		}

		if (func == 0x13)//MTLO
		{
			NEXT_STATE.LO = val_rs;
			return;
		}


		if (func == 0x1a)//DIV
		{
			NEXT_STATE.LO = val_rs / val_rt;
			NEXT_STATE.HI = val_rs % val_rt;
			return;
		}

		if (func == 0x1b)//DIVU
		{
			NEXT_STATE.LO = val_rsu / val_rtu;
			NEXT_STATE.HI = val_rsu % val_rtu;
			return;
		}

		if (func == 0x22)//SUB
		{
			NEXT_STATE.REGS[rd] = val_rs - val_rt;
			return;
		}

		if (func == 0x23)//SUBU
		{
			NEXT_STATE.REGS[rd] = val_rsu - val_rtu;
			return;
		}

		if (func == 0x20)//ADD
		{
			NEXT_STATE.REGS[rd] = val_rs + val_rt;
			return;
		}
		if (func == 0x21)//ADDU
		{
			NEXT_STATE.REGS[rd] = val_rsu + val_rtu;
			return;
		}

		if (func == 0x00)//SLL
		{
			NEXT_STATE.REGS[rd] = val_rt << shamt;
			return;
		}

		if (func == 0x02)//SRL
		{
			NEXT_STATE.REGS[rd] = val_rtu >> shamt;
			return;
		}

		if (func == 0x06)//SRLV
		{
			NEXT_STATE.REGS[rd] = val_rtu >> (val_rs & 0x1f);
			return;
		}

		if (func == 0x03)//SRA
		{
			NEXT_STATE.REGS[rd] = val_rt >> shamt;
			return;
		}

		if (func == 0x07)//SRAV
		{
			NEXT_STATE.REGS[rd] = val_rt >> (val_rs & 0x1f);
			return;
		}

		if (func == 0x04)//SLLV
		{
			NEXT_STATE.REGS[rd] = val_rt << (val_rs & 0x1f);
			return;
		}

		if (func == 0x8)//JR
		{
			NEXT_STATE.PC = val_rsu;
			return;
		}

		if (func == 0x9)//JARL
		{
			NEXT_STATE.PC = val_rsu + 4;
			if(rd){
				NEXT_STATE.REGS[rd] = CURRENT_STATE.PC+8;
			}
			else NEXT_STATE.REGS[31] = CURRENT_STATE.PC+8;
			return;
		}

    }
}
