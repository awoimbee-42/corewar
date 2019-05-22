/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/22 16:58:00 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "vm.h"


// OCP:
// register: 0b01 > 1
// indirect: 0b10 > 2
// direct:   0b11 > 3

// ACB == OCP

/*
	-> 0b 68 01 ff b6 00 01 (sti) does not work
	   same with 0b 68 01 ff af 00 0f
	   sti just doesnt work
*/

void			op_live(t_vm *vm, int proc, int reg_num[3])
{
	int		i;
	int		player;
	int		fail;

	player = vm->procs.d[proc].reg[reg_num[0]];
	i = 0;
	fail = 0;
	while (i < vm->players.len && vm->players.d[i].id != player)
		if (++i == vm->players.len)
			fail = 1;
	if (!fail)
	{
		vm->players.d[i].period_lives += 1;
		vm->players.d[i].last_live = vm->cycle_curr;
		if (vm->verbosity >= VE_OPS)
			ft_printf("\n");
		if (vm->verbosity >= VE_LIVE)
			ft_printf("un processus dit que le joueur %d(%s) est en vie%s",
				vm->players.d[i].id, vm->players.d[i].head.prog_name,
				(vm->verbosity < VE_OPS) ? "\n" : "");
	}
	vm->procs.d[proc].live++;
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_ld(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_st(t_vm *vm, int proc, int reg_num[3])
{
	if (((vm->arena[circumem(vm->procs.d[proc].pc + 1)] >> 4) & 0b11) == IND_CODE)
		write32(vm, vm->procs.d[proc].pc, vm->procs.d[proc].pc + (load16(vm, vm->procs.d[proc].pc + 3) % IDX_MOD), vm->procs.d[proc].reg[reg_num[0]]);
	else
		vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_add(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_sub(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] - vm->procs.d[proc].reg[reg_num[1]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = vm->procs.d[proc].reg[reg_num[0]] - vm->procs.d[proc].reg[reg_num[1]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_and(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] & vm->procs.d[proc].reg[reg_num[1]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = vm->procs.d[proc].reg[reg_num[0]] & vm->procs.d[proc].reg[reg_num[1]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_or(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] | vm->procs.d[proc].reg[reg_num[1]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = vm->procs.d[proc].reg[reg_num[0]] | vm->procs.d[proc].reg[reg_num[1]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_xor(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ^ vm->procs.d[proc].reg[reg_num[1]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = vm->procs.d[proc].reg[reg_num[0]] ^ vm->procs.d[proc].reg[reg_num[1]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_zjmp(t_vm *vm, int proc, int reg_num[3])
{
	if (vm->procs.d[proc].carry)
	{
		vm->procs.d[proc].new_pc = circumem(vm->procs.d[proc].pc + (vm->procs.d[proc].reg[reg_num[0]] % IDX_MOD));
		if (vm->verbosity >= VE_OPS)
			ft_printf(" OK");
	}
	else if (vm->verbosity >= VE_OPS)
		ft_printf(" FAILED");
}

void			op_ldi(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].reg[reg_num[2]] = load32(vm, vm->procs.d[proc].pc + ((vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]]) % IDX_MOD));
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_sti(t_vm *vm, int proc, int reg_num[3])
{
	int addr;

	addr = vm->procs.d[proc].pc + (vm->procs.d[proc].reg[reg_num[1]] + vm->procs.d[proc].reg[reg_num[2]]) % IDX_MOD;
	// ft_fprintf(2, "addr: %d\nreg_num[1]: %d, reg_num[2]: %d\n", addr, vm->procs.d[proc].reg[reg_num[1]], vm->procs.d[proc].reg[reg_num[2]]); //PUUUTE
	write32(vm, vm->procs.d[proc].pc, addr, vm->procs.d[proc].reg[reg_num[0]]);
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_fork(t_vm *vm, int proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &vm->procs, vm->procs.d[proc]);
	vm->procs.d[vm->procs.len - 1].pc = circumem(vm->procs.d[proc].pc + (vm->procs.d[proc].reg[reg_num[0]] % IDX_MOD));
	vm->procs.d[vm->procs.len - 1].new_pc = 1;
	if (vm->verbosity >= VE_OPS)
		ft_printf(" (%d)",vm->procs.d[vm->procs.len - 1].pc);
		//ft_printf(" (%d) new_pc: %d",vm->procs.d[vm->procs.len - 1].pc, vm->procs.d[proc].new_pc);
	read_instruction(vm, vm->procs.len - 1);
}

void			op_lld(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_lldi(t_vm *vm, int proc, int reg_num[3])
{
	int		res;

	res = load32(vm, vm->procs.d[proc].pc + (vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]]));
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = load32(vm, vm->procs.d[proc].pc + (vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]]));
	(void)vm;

	(void)proc;
	(void)reg_num;
}

void			op_lfork(t_vm *vm, int proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &vm->procs, vm->procs.d[proc]);
	vm->procs.d[vm->procs.len - 1].pc = circumem(vm->procs.d[proc].pc + vm->procs.d[proc].reg[reg_num[0]]);
	vm->procs.d[vm->procs.len - 1].new_pc = 1;
	read_instruction(vm, vm->procs.len - 1);
	if (vm->verbosity >= VE_OPS)
		ft_printf(" (%d) new_pc: %d",vm->procs.d[vm->procs.len - 1].pc, vm->procs.d[proc].new_pc);
}

void			op_aff(t_vm *vm, int proc, int reg_num[3])
{
	uint8_t	c;
	int		cursor;

	c = vm->procs.d[proc].reg[reg_num[0]] % 256;
	if (vm->verbosity >= VE_AFF)
		ft_printf("{grn}aff: %c{eoc}\n", c);
	else if (vm->verbosity == VE_VISU)
	{
		cursor = ft_strlen(vm->visu.aff);
		if (cursor == AFF_BUFF_LEN - 1)
		{
			ft_bzero(vm->visu.aff, AFF_BUFF_LEN);
			cursor = 0;
		}
		vm->visu.aff[cursor] = c;
		vm->visu.aff[cursor + 1] = ' ';
	}
}
