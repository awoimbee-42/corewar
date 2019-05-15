/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/15 17:52:24 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "vm.h"


// OCP:
// register: 0b01 > 1
// indirect: 0b10 > 2
// direct:   0b11 > 3

// ACB == OCP


/*
**	#################
**	  Helper function
**	#################
*/
/*
void	op_live(t_vm *vm, t_play *p, t_proc *proc)
{
	int		i;
	int		player;
	int		fail;

	player = load32(vm, proc->pc + 1);//swap32_endian(*(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE]);
	i = 0;
	fail = 0;
	while (i < vm->players.len && vm->players.d[i].id != player)
		if (++i == vm->players.len)
			fail = 1;
	if (!fail)
	{
		// vm->players.d[i].live(..)
		ft_printf("un processus dit que le joueur x(nom_champion) est en vie\n");
	}
	proc->live++;
	proc->pc = (proc->pc + 5) % MEM_SIZE;
	if (vm->verbosity == 4)
		ft_printf("P #%-5d | live %d\n", p->id, player);
}

void	op_ld(t_vm *vm, t_play *p, t_proc *proc)
{
	uint8_t	acb;
	int		tmp;
	int		rel;
	int		fail;

	fail = 0;
	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	if (acb == 0b11010000) // indirect
	{
		tmp = swap16_endian(*(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE]);
		tmp = *(int*)&vm->arena[tmp % IDX_MOD];
		rel = 3;
	}
	else if (acb == 0b10010000) // direct
	{
		tmp = *(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
		rel = 5;
	}
	else
	{
		ft_printf("P #%5d | ld FAIL\n", p->id);
		proc->pc += 1;
		return ;
	}
	proc->reg[vm->arena[(proc->pc + rel) % MEM_SIZE] % REG_NUMBER] = tmp;
	proc->pc = (proc->pc + rel + REG_SIZE + 1) % MEM_SIZE;
	ft_printf("P #%5d | ld SUCCESS\n", p->id);
}

int		op_st(t_vm *vm, t_play *p, t_proc *proc)
{
	uint8_t acb;
	int num;
	int reg;
	int rel;

	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	if (acb & 48 == 48)
	{
		rel = 3;
		num = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
		num = *(int*)&vm->arena[num % MEM_SIZE]
	}
	else if (acb & 32 == 32)
	{
		rel = 5;
		num = *(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	}
	vm->arena[num % MEM_SIZE] = ;
	proc->reg[reg] = num;
	proc->pc = (proc->pc + rel + 1) % MEM_SIZE;
}

void	op_add(t_vm *vm, t_play *p, t_proc *proc)
{
	uint8_t	*reg_ids;

	reg_ids = &vm->arena[proc->pc + 2];
	if (vm->arena[proc->pc + 1] != 0b01010100
		|| reg_ids[0] >= REG_NUMBER || reg_ids[0] < 1
		|| reg_ids[1] >= REG_NUMBER || reg_ids[1] < 1
		|| reg_ids[2] >= REG_NUMBER || reg_ids[2] < 1)
	{
		proc->pc += 1;
		proc->carry = 0;
		if (vm->verbosity == 4)
		ft_printf("P #%5d | add FAIL\n", p->id);
		return ;
	}
	proc->reg[reg_ids[2]] = proc->reg[reg_ids[0]] + proc->reg[reg_ids[1]];
	proc->carry = 1;
	proc->pc = (proc->pc + 13) % MEM_SIZE;
}

void	op_sub(t_vm *vm, t_play *p, t_proc *proc)
{
	uint8_t	*reg_ids;

	reg_ids = &vm->arena[proc->pc + 2];
	if (vm->arena[proc->pc + 1] != 0b01010100
		|| reg_ids[0] >= REG_NUMBER || reg_ids[0] < 1
		|| reg_ids[1] >= REG_NUMBER || reg_ids[1] < 1
		|| reg_ids[2] >= REG_NUMBER || reg_ids[2] < 1)
	{
		proc->carry = 0;
		proc->pc += 1; //
	}
	proc->reg[reg_ids[2]] = proc->reg[reg_ids[0]] - proc->reg[reg_ids[1]];
	proc->carry = 1;
	proc->pc = (proc->pc + 13) % MEM_SIZE;
}

void	op_zjmp(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = 3;
	if (proc->carry)
		rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + rel) % MEM_SIZE; // % IDX_MOD missing?        ??????????????????????????????
}

void	op_fork(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + (rel % IDX_MOD)) % MEM_SIZE;
}

void	op_lfork(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + rel) % MEM_SIZE;
}

void	op_aff(t_vm *vm, t_play *p, t_proc *proc)
{
	uint	reg_id;
	uint8_t	c;

	reg_id = (uint)vm->arena[(proc->pc + 2) % MEM_SIZE];
	if (reg_id > REG_NUMBER
		|| vm->arena[(proc->pc + 1) % MEM_SIZE] != 0b01000000)
		return ;
	c = proc->reg[reg_id];
	write(1, &c, 1);
	proc->pc = (proc->pc + 5) % MEM_SIZE;
}
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
		// vm->players.d[i].live(..)
		ft_printf("un processus dit que le joueur x(nom_champion) est en vie\n");
	}
	proc->live++;
	// proc->pc = (proc->pc + 5) % MEM_SIZE;
	if (vm->verbosity > 3)
		ft_printf("P #%-5d | live %d\n", play->id, player);
}

void			op_ld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
}

void			op_st(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	if (((vm->arena[(proc->pc + 1) % MEM_SIZE] >> 4) & 0b11) == IND_CODE)
		write32(vm, proc, proc->pc + (load16(vm, proc->pc + 3) % IDX_MOD), proc->reg[reg_num[0]]);
	else
		write32(vm, proc, (proc->pc + (proc->reg[reg_num[1]] % IDX_MOD)), proc->reg[reg_num[0]]);
}

void			op_add(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] + proc->reg[reg_num[1]];
}

void			op_sub(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = proc->reg[reg_num[0]] - proc->reg[reg_num[1]];
}

void			op_and(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
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
		proc->new_pc = proc->reg[reg_num[0] % IDX_MOD] % MEM_SIZE;
}

void			op_ldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = load32(vm, proc->pc + ((proc->reg[reg_num[0]] + proc->reg[reg_num[1]]) % IDX_MOD));
}

void			op_sti(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	int addr;

	addr = (proc->reg[reg_num[1]] + proc->reg[reg_num[2]]) % IDX_MOD;
	write32(vm, proc, proc->pc + addr, proc->reg[reg_num[0]]);
}

void			op_fork(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &play->procs, *proc);
	play->procs.d[play->procs.len - 1].pc = (proc->pc + (proc->reg[reg_num[0]] % IDX_MOD)) % MEM_SIZE;
	play->procs.d[play->procs.len - 1].op_cycles = 0;
}

void			op_lld(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[1]] = proc->reg[reg_num[0]];
}

void			op_lldi(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	proc->reg[reg_num[2]] = load32(vm, proc->pc + (proc->reg[reg_num[0]] + proc->reg[reg_num[1]]));
}

void			op_lfork(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	vecproc_push(&vm->gb, &play->procs, *proc);
	play->procs.d[play->procs.len - 1].pc = (proc->pc + proc->reg[reg_num[0]]) % MEM_SIZE;
	play->procs.d[play->procs.len - 1].op_cycles = 0;
}

void			op_aff(t_vm *vm, t_play *play, t_proc *proc, int reg_num[3])
{
	uint8_t	c;

	c = proc->reg[reg_num[0]] % 256;
	if (vm->verbosity > 0)
		ft_printf("{grn}aff: %c{eoc}\n", c);
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
	// int		size;
	int		tmp;

	op_id = vm->arena[proc->pc] - 1;
	if (cb == REG_CODE && (T_REG & g_op[op_id].args_type[cur_arg]))
	{
		tmp = load8(vm, proc->new_pc);
		proc->new_pc = (proc->new_pc + 1) % MEM_SIZE;
		return (tmp <= REG_NUMBER && tmp >= 1 ? tmp : -1);
	}
	else if (cb == DIR_CODE && T_DIR & g_op[op_id].args_type[cur_arg])
	{
		proc->reg[REG_NUMBER + cur_arg] = g_op[op_id].dir2 ? load16(vm, proc->new_pc) : load32(vm, proc->new_pc);
		proc->new_pc = g_op[op_id].dir2 ? (proc->new_pc + 2) % MEM_SIZE : (proc->new_pc + 4) % MEM_SIZE;
	}
	else if (cb == IND_CODE && T_IND & g_op[op_id].args_type[cur_arg])
	{
		if (g_op[op_id].ldx_rel)
			proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + (load16(vm, proc->new_pc) % IDX_MOD));
		else
			proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + load16(vm, proc->new_pc));
		proc->new_pc = (proc->new_pc + 2) % MEM_SIZE;
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
				return (0);
			i++;
		}
	}
	else
	{
		proc->new_pc = (proc->pc + 1) % MEM_SIZE;
		reg_num[0] = read_one_arg(vm, proc, T_DIR, 0);
		if (reg_num[0] == -1)
			return (0);
		// proc->new_pc = (proc->new_pc + 2) % MEM_SIZE;
	}
	// g_op[op_id].fun(vm, play, proc, reg_num);
	return (1);
}




void			launch_instruction(t_vm *vm, t_play *play, t_proc *proc)
{
	int		op_id;
	int		reg_num[3];

	op_id = vm->arena[proc->pc] - 1;
	if (!load_arg_into_regs(vm, play, proc, reg_num))
	{
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		 // I dunno what is needed here
		 if (g_op[op_id].modcarry)
		 	proc->carry = 0;
		return ;
	}
	g_op[op_id].fun(vm, play, proc, reg_num);
	if (g_op[op_id].modcarry)
		proc->carry = 1;
	proc->pc = proc->new_pc % MEM_SIZE;
}