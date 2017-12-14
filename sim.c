#include <stdio.h>
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
    read();
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
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
    	if (op>3)//é I-type
    	{
    		uint32_t imu = pc & 0xffff;
    		int im = pc & 0xffff;

    		if (op == 9)//ADDIU
    		{
    			val_rtu = val_rsu + imu;
    			NEXT_STATE.REGS[rt] = val_rtu;
    		}
    		if (op == 8)//ADDI
    		{
    			val_rt = val_rs + im;
    			NEXT_STATE.REGS[rt] = val_rt;
    		}

    		if (op == 0xa)//SLTI
			{
				NEXT_STATE.REGS[rt] = val_rs < im;
			}

			if (op == 0xb)//SLTIU
			{
				NEXT_STATE.REGS[rt] = val_rs < imu;
			}

			if (op == 0xd)//ORI
			{
				NEXT_STATE.REGS[rt] = val_rs | im;
			}

			if (op == 0xe)//XORI
			{
				NEXT_STATE.REGS[rt] = val_rs ^ im;
			}

			if (op == 0xc)//ANDI
			{
				NEXT_STATE.REGS[rt] = val_rs & im;
			}

			if (op == 0xf)//LUI
			{
				NEXT_STATE.REGS[rt] = im << 16;
			}
    	}
    	else{//é J-type

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
		}
		if (func == 0x24)//AND
		{
			NEXT_STATE.REGS[rd] = val_rs & val_rt;
		}

		if (func == 0x25)//OR
		{
			NEXT_STATE.REGS[rd] = val_rs | val_rt;
		}

		if (func == 0x26)//XOR
		{
			NEXT_STATE.REGS[rd] = val_rs ^ val_rt;
		}

		if (func == 0x27)//NOR
		{
			NEXT_STATE.REGS[rd] = ~(val_rs | val_rt);
		}

		if (func == 0x2a)//SLT
		{
			NEXT_STATE.REGS[rd] = val_rs < val_rt;
		}

		if (func == 0x2b)//SLTU
		{
			NEXT_STATE.REGS[rd] = val_rsu < val_rtu;
		}



		if (func == 0x18)//MULT
		{
			long result = val_rs * val_rt;
			NEXT_STATE.HI = result >> 32;
			NEXT_STATE.LO = result & 0xffffffff;
		}

		if (func == 0x19)//MULTU
		{
			uint64_t result = val_rsu * val_rtu;
			NEXT_STATE.HI = result >> 32;
			NEXT_STATE.LO = result & 0xffffffff;
		}

		if (func == 0x10)//MFHI
		{
			NEXT_STATE.REGS[rd] = hi;
		}

		if (func == 0x12)//MFLO
		{
			NEXT_STATE.REGS[rd] = lo;
		}

		if (func == 0x11)//MTHI
		{
			NEXT_STATE.HI = val_rs;
		}

		if (func == 0x13)//MTLO
		{
			NEXT_STATE.LO = val_rs;
		}


		if (func == 0x1a)//DIV
		{
			NEXT_STATE.LO = val_rs / val_rt;
			NEXT_STATE.HI = val_rs % val_rt;
		}

		if (func == 0x1b)//DIVU
		{
			NEXT_STATE.LO = val_rsu / val_rtu;
			NEXT_STATE.HI = val_rsu % val_rtu;
		}

		if (func == 0x22)//SUB
		{
			NEXT_STATE.REGS[rd] = val_rs - val_rt;
		}

		if (func == 0x23)//SUBU
		{
			NEXT_STATE.REGS[rd] = val_rsu - val_rtu;
		}

		if (func == 0x20)//ADD
		{
			NEXT_STATE.REGS[rd] = val_rs + val_rt;
		}
		if (func == 0x21)//ADDU
		{
			NEXT_STATE.REGS[rd] = val_rsu + val_rtu;
			printf("%x %x\n",val_rsu, val_rtu );
		}

		if (func == 0x00)//SLL
		{
			NEXT_STATE.REGS[rd] = val_rt << shamt;
		}

		if (func == 0x02)//SRL
		{
			NEXT_STATE.REGS[rd] = val_rtu >> shamt;
		}

		if (func == 0x03)//SRA
		{
			NEXT_STATE.REGS[rd] = val_rt >> shamt;
		}

    }
}
