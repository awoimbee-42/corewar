/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skiessli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/08 14:56:04 by skiessli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "vm.h"


/*
**	#################
**	  Helper function
**	#################
*/

int		get_first_arg(t_vm *vm, t_play *p, t_proc *proc)
{
	char acb;
	int num;
	int reg;
	int rel;

	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	if (acb & 192 == 192)
	{
		rel = 3;
		num = *(short*)&vm->arena[(proc->pc + 2) % MEM_SIZE];
		num = *(int*)&vm->arena[(proc->pc + (num + IDX_MOD)) % MEM_SIZE]
	}
	else if (acb & 128 == 128)
	{
		rel = 5;
		num = *(int*)&vm->arena[(proc->pc + 2) % MEM_SIZE];
	}
}

int		get_second_arg(t_vm *vm, t_play *p, t_proc *proc)
{

}

int		get_thrid_arg(t_vm *vm, t_play *p, t_proc *proc)
{

}

int		op_live(t_vm *vm, t_play *p, t_proc *proc)
{
	int player;

	player = *(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE];
	p->alive_flag = TRUE;
	proc->pc = (proc->pc + 5) % MEM_SIZE;
	return (player);
}

void	op_ld(t_vm *vm, t_play *p, t_proc *proc)
{
	char acb;
	int num;
	int reg;
	int rel;

	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	if (acb & 192 == 192)
	{
		rel = 3;
		num = *(short*)&vm->arena[(proc->pc + 2) % MEM_SIZE];
		num = *(int*)&vm->arena[(proc->pc + (num + IDX_MOD)) % MEM_SIZE]
	}
	else if (acb & 128 == 128)
	{
		rel = 5;
		num = *(int*)&vm->arena[(proc->pc + 2) % MEM_SIZE];
	}
	else
	{
		proc->carry = 0;
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		return ;
	}
	reg = vm->arena[(proc->pc + rel + 1) % MEM_SIZE];
	if (reg >= REG_SIZE)
	{
		proc->carry = 0;
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		return ;
	}
	proc->reg[reg] = num;
	proc->carry = 1;
	proc->pc = (proc->pc + rel + 1) % MEM_SIZE;
}

void	op_st(t_vm *vm, t_play *p, t_proc *proc)
{
	char acb;
	int num;
	int reg;
	int rel;

	acb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	reg = vm->arena[(proc->pc + 2) % MEM_SIZE];
	if (reg >= REG_SIZE)
	{
		proc->carry = 0;
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		return ;
	}
	if (acb & 48 == 48)
	{
		rel = 3;
		num = *(short*)&vm->arena[(proc->pc + 3) % MEM_SIZE];
		*(int*)&vm->arena[(proc->pc + (num % IDX_MOD)) % MEM_SIZE] = proc->reg[reg];
	}
	else if (acb & 32 == 32)
	{
		rel = 5;
		num = *(int*)&vm->arena[(proc->pc + 3) % MEM_SIZE];
	}
	else
	{
		proc->carry = 0;
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		return ;
	}
	proc->carry = 1;
	proc->pc = (proc->pc + rel + 1) % MEM_SIZE;
}

void	op_add(t_vm *vm, t_play *p, t_proc *proc)
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

void	op_sub(t_vm *vm, t_play *p, t_proc *proc)
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

void	op_and(t_vm *vm, t_play *p, t_proc *proc)
{

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


// ??????????? What to print excatcly>>>>
void	op_aff(t_vm *vm, t_play *p, t_proc *proc)
{
	char			rel;
	unsigned int	reg;

	reg = *(unsigned int*)&vm->arena[(proc->pc + 2) % MEM_SIZE];
	if (reg >= REG_SIZE)	
	{
		proc->pc = (proc->pc + 1) % MEM_SIZE;
		return ;	
	}
	rel = proc->reg[reg];
	write(1, &rel, 4);
	proc->pc = (proc->pc + 3) % MEM_SIZE;
}