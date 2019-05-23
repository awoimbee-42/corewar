/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 13:03:25 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 01:28:24 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

//	si le cycle_to_die tombe en meme temps qu'une instruction live on fait quoi ?
//	quand die_cycle_checks == MAX_CHECKS on décrémente de 1 ou de CYCLE_DELTA ?

static void		launch_instruction(t_vm *vm, int proc)
{
	int		reg_num[MAX_ARGS_NUMBER];
	int		i;

	if (load_arg_into_regs(vm, &vm->procs.d[proc], reg_num))
	{
		g_op[vm->procs.d[proc].op_id].fun(vm, proc, reg_num);
		if (vm->verbosity >= VE_OPS)
			ft_printf("\n");
	}
	if (vm->verbosity >= VE_PC_MOVE && vm->procs.d[proc].op_id != 8)
	{
		ft_printf("ADV %i (%#06x -> %#06x) ", vm->procs.d[proc].new_pc
				- vm->procs.d[proc].pc, vm->procs.d[proc].pc, vm->procs.d[proc].new_pc);
		i = -1;
		while (++i + vm->procs.d[proc].pc < vm->procs.d[proc].new_pc)
			ft_printf("%02x ", vm->arena[(vm->procs.d[proc].pc + i) % MEM_SIZE]);
		ft_printf("\n");
	}
	vm->procs.d[proc].pc = vm->procs.d[proc].new_pc % MEM_SIZE;
	vm->procs.d[proc].new_pc = 0;
}

void			read_instruction(t_vm *vm, int proc)
{
	int			op_id;

	op_id = vm->arena[vm->procs.d[proc].pc] - 1;
	if (0 <= op_id && op_id <= 15)
	{
		vm->procs.d[proc].op_id = op_id;
		vm->procs.d[proc].op_cycles = g_op[op_id].cycles;
	}
	else
	{
		if (vm->procs.d[proc].new_pc == 0)
			vm->procs.d[proc].pc = (vm->procs.d[proc].pc + 1) % MEM_SIZE;
		vm->procs.d[proc].new_pc = 0;
		vm->procs.d[proc].op_cycles = -1;
	}
}

void			check_live(t_vm *vm)
{
	int			nbr_live;
	int			j;

	if (vm->cycle_curr - vm->cycle_last_check < vm->cycle_die)
		return ;
	vm->cycle_last_check = vm->cycle_curr;
	vm->die_cycle_checks++;
	nbr_live = 0;
	j = vm->procs.len;
	while (j-- != 0)
	{
		nbr_live += vm->procs.d[j].live;
		if (vm->procs.d[j].live == 0)
		{
			if (vm->verbosity >= VE_PROCDEATH)
				ft_printf("\tProcess %d of player %d died\n",
					j, vm->procs.d[j].play->id);
			vm->procs.d[j].pid = 0;
		}
		vm->procs.d[j].live = 0;
	}
	vecproc_del_dead(&vm->procs);
	j = vm->players.len;
	while (j-- != 0)
		vm->players.d[j].period_lives = 0;
	if (nbr_live >= NBR_LIVE || vm->die_cycle_checks == MAX_CHECKS)
	{
		vm->cycle_die -= CYCLE_DELTA;
		vm->die_cycle_checks = 0;
		if (vm->verbosity >= VE_CYCLE)
			ft_printf("Cycle to die is now %i\n", vm->cycle_die);
	}
}

int				run_vm_cycle(t_vm *vm)
{
	int			i;

	++vm->cycle_curr;
	if (vm->verbosity >= VE_CYCLE)
		ft_printf("It is now cycle %lu\n", vm->cycle_curr);
	i = vm->procs.len;
	while (i != 0)
	{
		--i;
		if (vm->verbosity >= VE_REGISTER)
			print_register(vm, &vm->procs.d[i]);
		if (vm->procs.d[i].op_cycles <= 0)
			read_instruction(vm, i);
		if (vm->procs.d[i].op_cycles == 1)
			launch_instruction(vm, i);
		--vm->procs.d[i].op_cycles;
		vm->procs.d[i].new_pc = 0;
	}
	check_live(vm);
	if (vm->procs.len == 0)
		return (0);
	return (1);
}
