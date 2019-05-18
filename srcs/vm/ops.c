/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/18 14:27:03 by awoimbee         ###   ########.fr       */
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

void			op_live(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int		i;
	int		player;
	int		fail;

	player = proc->reg[reg_num[0]];
	i = 0;
	fail = 0;
	while (i < vm->players.len && vm->players.d[i].id != player)
		if (++i == vm->players.len)
			fail = 1;
	if (!fail)
	{
		vm->players.d[i].period_lives += 1;
		if (vm->verbosity >= VE_LIVE)
			ft_printf("un processus dit que le joueur %d(%s) est en vie\n",
				vm->players.d[i].id, vm->players.d[i].head.prog_name);
	}
	proc->live++;
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_ld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] ? 0 : 1;
	proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_st(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	if (((vm->arena[circumem(proc->pc + 1)] >> 4) & 0b11) == IND_CODE)
		write32(vm, proc, proc->pc + (load16(vm, proc->pc + 3) % IDX_MOD), proc->reg[reg_num[0]]);
	else
		proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_add(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] + proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] + proc->reg[reg_num[1]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_sub(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] - proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] - proc->reg[reg_num[1]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_and(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] & proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] & proc->reg[reg_num[1]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_or(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] | proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] | proc->reg[reg_num[1]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_xor(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] ^ proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] ^ proc->reg[reg_num[1]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_zjmp(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	if (proc->carry)
		proc->new_pc = circumem(proc->pc + (proc->reg[reg_num[0]] % IDX_MOD));
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_ldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = load32(vm, proc->pc + ((proc->reg[reg_num[0]] + proc->reg[reg_num[1]]) % IDX_MOD));
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_sti(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int addr;

	addr = proc->pc + (proc->reg[reg_num[1]] + proc->reg[reg_num[2]]) % IDX_MOD;
	// ft_fprintf(2, "addr: %d\nreg_num[1]: %d, reg_num[2]: %d\n", addr, proc->reg[reg_num[1]], proc->reg[reg_num[2]]); //PUUUTE
	write32(vm, proc, addr, proc->reg[reg_num[0]]);
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_fork(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &play->procs, *proc);
	play->procs.d[play->procs.len - 1].pc = circumem(proc->pc + (proc->reg[reg_num[0]] % IDX_MOD));
	play->procs.d[play->procs.len - 1].op_cycles = 0;
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_lld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] ? 0 : 1;
	proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_lldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int		res;

	res = load32(vm, proc->pc + (proc->reg[reg_num[0]] + proc->reg[reg_num[1]]));
	proc->carry = res ? 0 : 1;
	proc->reg[reg_num[2]] = load32(vm, proc->pc + (proc->reg[reg_num[0]] + proc->reg[reg_num[1]]));
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_lfork(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &play->procs, *proc);
	play->procs.d[play->procs.len - 1].pc = circumem(proc->pc + proc->reg[reg_num[0]]);
	play->procs.d[play->procs.len - 1].op_cycles = 0;
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}

void			op_aff(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	uint8_t	c;

	c = proc->reg[reg_num[0]] % 256;
	if (vm->verbosity >= VE_AFF)
		ft_printf("{grn}aff: %c{eoc}\n", c);
	else if (vm->verbosity == VE_VISU)
		; // /!\ /!\ /!\ /!\ DO SOMETHING  /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /
	(void)vm;
	(void)play;
	(void)proc;
	(void)reg_num;
}
