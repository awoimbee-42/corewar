/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/17 22:48:07 by awoimbee         ###   ########.fr       */
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
	// if (vm->verbosity > 3)
	// 	ft_printf("P #%-5d | live %d\n", play->id, player);
}

void			op_ld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] ? 0 : 1;
	proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
}

void			op_st(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	if (((vm->arena[circumem(proc->pc + 1)] >> 4) & 0b11) == IND_CODE)
		write32(vm, proc, proc->pc + (load16(vm, proc->pc + 3) % IDX_MOD), proc->reg[reg_num[0]]);
	else
		proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
}

void			op_add(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] + proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] + proc->reg[reg_num[1]];
}

void			op_sub(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] - proc->reg[reg_num[1]];
}

void			op_and(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] & proc->reg[reg_num[1]] ? 0 : 1;
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] & proc->reg[reg_num[1]];
}

void			op_or(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] | proc->reg[reg_num[1]];
}

void			op_xor(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] ^ proc->reg[reg_num[1]];
}

void			op_zjmp(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	if (proc->carry)
		proc->new_pc = circumem(proc->pc + (proc->reg[reg_num[0]] % IDX_MOD));
}

void			op_ldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = load32(vm, proc->pc + ((proc->reg[reg_num[0]] + proc->reg[reg_num[1]]) % IDX_MOD));
}

void			op_sti(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int addr;

	addr = proc->pc + (proc->reg[reg_num[1]] + proc->reg[reg_num[2]]) % IDX_MOD;
	// ft_fprintf(2, "addr: %d\nreg_num[1]: %d, reg_num[2]: %d\n", addr, proc->reg[reg_num[1]], proc->reg[reg_num[2]]); //PUUUTE
	write32(vm, proc, addr, proc->reg[reg_num[0]]);
}

void			op_fork(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &play->procs, *proc);
	play->procs.d[play->procs.len - 1].pc = circumem(proc->pc + (proc->reg[reg_num[0]] % IDX_MOD));
	play->procs.d[play->procs.len - 1].op_cycles = 0;
}

void			op_lld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->carry = proc->reg[reg_num[0]] ? 0 : 1;
	proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
}

void			op_lldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int		res;

	res = load32(vm, proc->pc + (proc->reg[reg_num[0]] + proc->reg[reg_num[1]]));
	proc->carry = res ? 0 : 1;
	proc->reg[reg_num[2]] = load32(vm, proc->pc + (proc->reg[reg_num[0]] + proc->reg[reg_num[1]]));
}

void			op_lfork(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &play->procs, *proc);
	play->procs.d[play->procs.len - 1].pc = circumem(proc->pc + proc->reg[reg_num[0]]);
	play->procs.d[play->procs.len - 1].op_cycles = 0;
}

void			op_aff(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	uint8_t	c;

	c = proc->reg[reg_num[0]] % 256;
	if (vm->verbosity >= VE_AFF)
		ft_printf("{grn}aff: %c{eoc}\n", c);
	else if (vm->verbosity == VE_VISU)
		; // /!\ /!\ /!\ /!\ DO SOMETHING  /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /!\ /
}

int				check_valid_return_size(unsigned char cb, t_arg_type types, t_bool dir2)
{
	if (cb == REG_CODE && T_REG & types)
		return (REG_SIZE);
	else if (cb == DIR_CODE && T_DIR & types)
		return (dir2 ? 2 : DIR_SIZE);
	else if (cb == IND_CODE && T_IND & types)
		return (IND_SIZE);
	return (-1);
}

int				read_one_arg(t_vm *vm, t_proc *proc, uint8_t cb, int cur_arg)
{
	int		op_id;
	int		tmp;

	op_id = vm->arena[proc->pc] - 1;
	if (cb == REG_CODE)// && (T_REG & g_op[op_id].args_type[cur_arg]))
	{
		tmp = load8(vm, proc->new_pc);
		proc->new_pc = (proc->new_pc + 1) % MEM_SIZE;
		return (tmp <= REG_NUMBER && tmp >= 1 && (T_REG & g_op[op_id].args_type[cur_arg]) ? tmp : -1);
	}
	else if (cb == DIR_CODE)// && T_DIR & g_op[op_id].args_type[cur_arg])
	{
		proc->reg[REG_NUMBER + cur_arg] = g_op[op_id].dir2 ? load16(vm, proc->new_pc) : load32(vm, proc->new_pc);
		proc->new_pc = g_op[op_id].dir2 ? (proc->new_pc + 2) % MEM_SIZE : (proc->new_pc + 4) % MEM_SIZE;
		if (!(T_DIR & g_op[op_id].args_type[cur_arg]))
			return (-1);
	}
	else if (cb == IND_CODE)// && T_IND & g_op[op_id].args_type[cur_arg])
	{
		if (g_op[op_id].ldx_rel)
			proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + (load16(vm, proc->new_pc) % IDX_MOD));
		else
			proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + load16(vm, proc->new_pc));
		proc->new_pc = (proc->new_pc + 2) % MEM_SIZE;
		if (!(T_IND & g_op[op_id].args_type[cur_arg]))
			return (-1);
	}
	else
		return (-1);
	return (REG_NUMBER + cur_arg);
}



int			load_arg_into_regs(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int		op_id;
	uint8_t	cb;
	int		i;
	t_bool	fail;

	fail = FALSE;
	op_id = vm->arena[proc->pc] - 1;
	if (g_op[op_id].coding_byte == TRUE)
	{
		i = 0;
		cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
		proc->new_pc = (proc->pc + 2) % MEM_SIZE;
		while (i < g_op[op_id].nb_args)
		{
			reg_num[i] = read_one_arg(vm, proc, (cb >> (6 - i * 2)) & 0b11, i);
			if (reg_num[i] == -1)
				fail = TRUE;
			i++;
		}
	}
	else
	{
		proc->new_pc = (proc->pc + 1) % MEM_SIZE;
		reg_num[0] = read_one_arg(vm, proc, T_DIR, 0);
		if (reg_num[0] == -1)
			fail = TRUE;
	}
	return (!fail);
}

// void			ve_print_operation(t_vm *vm, t_play *play, t_proc *proc, int reg_num[MAX_ARGS_NUMBER])
// {
// 	char		*args;
// 	int			i;
// 	int			op_id;

// 	args = NULL;
// 	op_id = vm->arena[proc->pc] - 1; // not so great
// 	i = -1;
// 	while (++i < g_op[op_id].nb_args && reg_num[i] != -1)
// 	{
// 		ft_printf("Track the uninitialized: %d %d (%d) %d\n", op_id,  g_op[op_id].nb_args, proc->reg[reg_num[i]], reg_num[i]);
// 		ft_strcat_join(&args, ft_cprintf(" %d", proc->reg[reg_num[i]]));
// 	}
// 	if (i != g_op[op_id].nb_args)
// 		ft_strcat_join(&args, ft_cprintf(" {RED}_{eoc}"));
// 	ft_printf("P   %d | %s %s\n", play->id, g_op[op_id].name, args);
// }

void			launch_instruction(t_vm *vm, t_play *play, t_proc *proc)
{
	int		op_id;
	int		reg_num[MAX_ARGS_NUMBER];

	op_id = vm->arena[proc->pc] - 1;
	if (!load_arg_into_regs(vm, play, proc, reg_num))
	{
		ft_printf("fail ta %d\n", proc->new_pc);
	}
	else
		g_op[op_id].fun(vm, play, proc, reg_num);
	proc->pc = proc->new_pc % MEM_SIZE;
	// if (vm->verbosity >= VE_OPS)
	// 	ve_print_operation(vm, play, proc, reg_num);
}
