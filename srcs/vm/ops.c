/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/09 22:28:17 by awoimbee         ###   ########.fr       */
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

int		op_live(t_vm *vm, t_play *p, t_proc *proc)
{
	int i;
	int player;

	player = *(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	i = 0;
	while (i < vm->players.len && vm->players.d[i].id != player)
		if (++i == vm->players.len)
			return (1);
	proc->live++;
	proc->pc = (proc->pc + 5) % MEM_SIZE;
	ft_printf("un processus dit que le joueur x(nom_champion) est en vie");
	return (player);
}

int		op_ld(t_vm *vm, t_play *p, t_proc *proc)
{
	char	acb;
	int		tmp;
	int		rel;

	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	if ((acb & 0b10010000) == 0b10010000) // indirect
	{
		tmp = *(int*)&vm->arena[
			*(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE] % IDX_MOD];
		rel = 3;
	}
	else if ((acb & 0b11010000) == 0b11010000) // direct
	{
		tmp = *(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
		rel = 5;
	}
	else
		return (1);
	proc->reg[vm->arena[(proc->pc + rel) % MEM_SIZE] % REG_NUMBER] = tmp;
	proc->pc = (proc->pc + rel + 1) % MEM_SIZE;
	return (0);
}
/*
int		op_st(t_vm *vm, t_play *p, t_proc *proc)
{
	char acb;
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
*/
int		op_add(t_vm *vm, t_play *p, t_proc *proc)
{
	char	*reg_ids;

	reg_ids = (char*)proc->pc + 2;
	if (vm->arena[proc->pc + 1] != 0b01010100
		|| reg_ids[0] >= REG_NUMBER || reg_ids[0] < 1
		|| reg_ids[1] >= REG_NUMBER || reg_ids[1] < 1
		|| reg_ids[2] >= REG_NUMBER || reg_ids[2] < 1)
	{
		proc->carry = 0;
		return (1); 		//fail
	}
	proc->reg[reg_ids[2]] = proc->reg[reg_ids[0]] + proc->reg[reg_ids[1]];
	proc->carry = 1;
	proc->pc = (proc->pc + 13) % MEM_SIZE;
	return (0);
}

int		op_sub(t_vm *vm, t_play *p, t_proc *proc)
{
	char	*reg_ids;

	reg_ids = (char*)proc->pc + 2;
	if (vm->arena[proc->pc + 1] != 0b01010100
		|| reg_ids[0] >= REG_NUMBER || reg_ids[0] < 1
		|| reg_ids[1] >= REG_NUMBER || reg_ids[1] < 1
		|| reg_ids[2] >= REG_NUMBER || reg_ids[2] < 1)
	{
		proc->carry = 0;
		return (1); 		//fail
	}
	proc->reg[reg_ids[2]] = proc->reg[reg_ids[0]] - proc->reg[reg_ids[1]];
	proc->carry = 1;
	proc->pc = (proc->pc + 13) % MEM_SIZE;
	return (0);
}

int		op_zjmp(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = 3;
	if (proc->carry)
		rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + rel) % MEM_SIZE; // % IDX_MOD missing?        ??????????????????????????????
	return (0);
}

int		op_fork(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + (rel % IDX_MOD)) % MEM_SIZE;
	return (0);
}

int		op_lfork(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + rel) % MEM_SIZE;
	return (0);
}

int		op_aff(t_vm *vm, t_play *p, t_proc *proc)
{
	uint	reg_id;
	char	c;

	reg_id = (uint)vm->arena[(proc->pc + 2) % MEM_SIZE];
	if (reg_id > REG_NUMBER
		|| vm->arena[(proc->pc + 1) % MEM_SIZE] != 0b01000000)
		return (0);
	c = proc->reg[reg_id];
	write(1, &c, 1);
	proc->pc = (proc->pc + 5) % MEM_SIZE;
	return (0);
}
