/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 14:56:02 by skiessli          #+#    #+#             */
/*   Updated: 2019/05/11 16:59:38 by awoimbee         ###   ########.fr       */
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

void	op_live(t_vm *vm, t_play *p, t_proc *proc)
{
	int		i;
	int		player;
	int		fail;

	player = swap32_endian(*(int*)&vm->arena[(proc->pc + 1) % MEM_SIZE]);
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
/*
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
*/
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
