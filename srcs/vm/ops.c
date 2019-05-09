/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/09 17:11:44 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "vm.h"


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
	i = -1;
	while (++i < vm->players.len && vm->players.d[i].id != player)
		;
	if (i == vm->players.len)
		return (1); //fail
	proc->live++;
	proc->pc = (proc->pc + 5) % MEM_SIZE;
	ft_printf("un processus dit que le joueur x(nom_champion) est en vie");
	return (player);
}

int		op_ld(t_vm *vm, t_play *p, t_proc *proc)
{
	char acb;
	int num;
	int reg;
	int rel;

	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	if (acb & 192 == 192)
	{
		rel = 3;
		num = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
		num = *(int*)&vm->arena[num % MEM_SIZE]
	}
	else if (acb & 128 == 128)
	{
		rel = 5;
		num = *(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	}
	reg = vm->arena[(proc->pc + rel) % MEM_SIZE] % REG_NUMBER;
	proc->reg[reg] = num;
	proc->pc = (proc->pc + rel + 1) % MEM_SIZE;
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
	int a;
	int b;
	int c;

	a = vm->arena[(proc->pc + 2) % MEM_SIZE];
	b = vm->arena[(proc->pc + 3) % MEM_SIZE];
	c = vm->arena[(proc->pc + 4) % MEM_SIZE];
	if (a >= REG_NUMBER || b >= REG_NUMBER || c >= REG_NUMBER)
		proc->carry = 0;
	else
	{
		reg[c] = reg[a] + reg[b];
		proc->carry = 1;
	}
	proc->pc = (proc->pc + 5) % MEM_SIZE;
}

int		op_sub(t_vm *vm, t_play *p, t_proc *proc)
{
	int a;
	int b;
	int c;

	a = vm->arena[(proc->pc + 2) % MEM_SIZE];
	b = vm->arena[(proc->pc + 3) % MEM_SIZE];
	c = vm->arena[(proc->pc + 4) % MEM_SIZE];
	if (a >= REG_NUMBER || b >= REG_NUMBER || c >= REG_NUMBER)
		proc->carry = 0;
	else
	{
		reg[c] = reg[a] - reg[b];
		proc->carry = 1;
	}
	proc->pc = (proc->pc + 5) % MEM_SIZE;
}

int		op_zjmp(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = 3;
	if (proc->carry)
		rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + rel) % MEM_SIZE; // % IDX_MOD missing?        ??????????????????????????????
}

int		op_fork(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + (rel % IDX_MOD)) % MEM_SIZE;
}

int		op_lfork(t_vm *vm, t_play *p, t_proc *proc)
{
	int	rel;

	rel = *(short*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->pc = (proc->pc + rel) % MEM_SIZE;
}


// ??????????? What to print excatcly>>>>
int		op_aff(t_vm *vm, t_play *p, t_proc *proc)
{
	char	rel;

	rel = proc->reg[vm->arena[(proc->pc + 2) % MEM_SIZE]];
	write(1, &rel, 1);
	proc->pc = (proc->pc + 3) % MEM_SIZE;
}