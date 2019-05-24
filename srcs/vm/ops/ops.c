/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/24 15:16:50 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void			op_live(t_vm *vm, int proc, int reg_num[3])
{
	int			i;
	int			player;
	int			fail;

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
			ft_printf("A process shows that player %d (%s) is alive%s",
				vm->players.d[i].id, vm->players.d[i].head.prog_name,
				(vm->verbosity < VE_OPS) ? "\n" : "");
	}
	vm->procs.d[proc].live++;
}

void			op_ld(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
}

void			op_st(t_vm *vm, int proc, int reg_num[3])
{
	int			addr;
	int			pc;

	pc = vm->procs.d[proc].pc;
	if (((vm->arena[circumem(pc + 1)] >> 4) & 0b11) == IND_CODE)
	{
		addr = pc + (load16(vm, pc + 3) % IDX_MOD);
		write32(vm, pc, addr, vm->procs.d[proc].reg[reg_num[0]]);
	}
	else
		vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
}

void			op_add(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_sub(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] - vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_and(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] & vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_or(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] | vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_xor(t_vm *vm, int proc, int reg_num[3])
{
	t_register	res;

	res = vm->procs.d[proc].reg[reg_num[0]] ^ vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_zjmp(t_vm *vm, int proc, int reg_num[3])
{
	if (vm->procs.d[proc].carry)
	{
		vm->procs.d[proc].new_pc = circumem(
				vm->procs.d[proc].pc
				+ (vm->procs.d[proc].reg[reg_num[0]] % IDX_MOD));
		if (vm->verbosity >= VE_OPS)
			ft_printf(" OK");
	}
	else if (vm->verbosity >= VE_OPS)
		ft_printf(" FAILED");
}

void			op_ldi(t_vm *vm, int proc, int reg_num[3])
{
	t_register	addr_rel;
	t_register	data;

	addr_rel = (vm->procs.d[proc].reg[reg_num[0]]
			+ vm->procs.d[proc].reg[reg_num[1]]) % IDX_MOD;
	data = load32(vm, vm->procs.d[proc].pc + addr_rel);
	vm->procs.d[proc].reg[reg_num[2]] = data;
}

void			op_sti(t_vm *vm, int proc, int reg_num[3])
{
	int			addr;
	t_proc		*p;

	p = &vm->procs.d[proc];
	addr = p->pc + (p->reg[reg_num[1]] + p->reg[reg_num[2]]) % IDX_MOD;
	write32(vm, p->pc, addr, p->reg[reg_num[0]]);
}

void			op_fork(t_vm *vm, int proc, int reg_num[3])
{
	t_proc		*parent;
	t_proc		*child;

	vecproc_push(&vm->gb, &vm->procs, vm->procs.d[proc]);
	parent = &vm->procs.d[proc];
	child = &vm->procs.d[vm->procs.len - 1];
	child->pc = circumem(parent->pc + (parent->reg[reg_num[0]] % IDX_MOD));
	child->new_pc = 1;
	if (vm->verbosity >= VE_OPS)
		ft_printf(" (%d)", child->pc);
	read_instruction(vm, vm->procs.len - 1);
}

void			op_lld(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
}

void			op_lldi(t_vm *vm, int proc, int reg_num[3])
{
	int32_t		res;
	t_proc		*p;

	p = &vm->procs.d[proc];
	res = load32(vm, p->pc + (p->reg[reg_num[0]] + p->reg[reg_num[1]]));
	p->carry = res ? 0 : 1;
	p->reg[reg_num[2]] = res;
}

void			op_lfork(t_vm *vm, int proc, int reg_num[3])
{
	t_proc		*parent;
	t_proc		*child;

	vecproc_push(&vm->gb, &vm->procs, vm->procs.d[proc]);
	parent = &vm->procs.d[proc];
	child = &vm->procs.d[vm->procs.len - 1];
	child->pc = circumem(parent->pc + parent->reg[reg_num[0]]);
	child->new_pc = 1;
	if (vm->verbosity >= VE_OPS)
		ft_printf(" (%d)", child->pc);
	read_instruction(vm, vm->procs.len - 1);
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
		if (cursor >= AFF_BUFF_LEN - 1)
		{
			ft_bzero(vm->visu.aff, AFF_BUFF_LEN);
			cursor = 0;
		}
		vm->visu.aff[cursor] = c;
		vm->visu.aff[cursor + 1] = ' ';
	}
}
