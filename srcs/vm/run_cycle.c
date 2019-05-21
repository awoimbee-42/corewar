/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 13:03:25 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/21 00:21:25 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

//	si le cycle_to_die tombe en meme temps qu'une instruction live on fait quoi ?
//	quand die_cycle_checks == MAX_CHECKS on décrémente de 1 ou de CYCLE_DELTA ?

static void		launch_instruction(t_vm *vm, int proc)
{
	int		reg_num[MAX_ARGS_NUMBER];

	if (load_arg_into_regs(vm, &vm->procs.d[proc], reg_num))
		g_op[vm->procs.d[proc].op_id].fun(vm, proc, reg_num);
	
	if (vm->verbosity >= VE_PC_MOVE)
		ft_printf("\nADV %i (%#06x -> %#06x)", vm->procs.d[proc].new_pc
				- vm->procs.d[proc].pc, vm->procs.d[proc].pc, vm->procs.d[proc].new_pc);
	vm->procs.d[proc].pc = vm->procs.d[proc].new_pc % MEM_SIZE;
	vm->procs.d[proc].new_pc = 0;
	if (vm->verbosity >= VE_OPS)
		ft_printf("\n");
}

void			read_instruction(t_vm *vm, int proc)
{
	int			op_id;

	op_id = vm->arena[vm->procs.d[proc].pc] - 1;
	//ft_printf("read op_id: %i pc: %i new_pc: %i\n", op_id, vm->procs.d[proc].pc, vm->procs.d[proc].new_pc);
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
		vm->procs.d[proc].op_cycles = 0;
	}
}

// static int		loop_player(t_vm *env, t_play *p)
// {
// 	int			i;

// 	i = p->procs.len;
// 	while (i-- != 0)
// 	{
// 		if (env->verbosity >= VE_REGISTER)
// 			print_register(p, i);
// 		--p->procs.d[i].op_cycles;
// 		if (p->procs.d[i].op_cycles == 0 && --p->procs.d[i].op_cycles)
// 			launch_instruction(env, p, &p->procs.d[i]);
// 		if (p->procs.d[i].op_cycles == -1)
// 			read_instruction(&p->procs.d[i], env);
// 		p->procs.d[i].new_pc = 0;
// 	}
// 	if (p->procs.len == 0)
// 		return (0);
// 	return (1);
// }

void			check_live(t_vm *vm)
{
	int			nbr_live;
	int			j;

	if (vm->cycle_die && vm->cycle_curr % vm->cycle_die)
		return ;
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
				vecproc_del_at(&vm->procs, j);
		}
		vm->procs.d[j].live = 0;
	}
	j = vm->players.len;
	while (j-- != 0)
		vm->players.d[j].period_lives = 0;
	if (nbr_live >= NBR_LIVE || vm->die_cycle_checks == MAX_CHECKS)
	{
		vm->cycle_die -= CYCLE_DELTA;
		vm->die_cycle_checks = 0;
	}
}

int				run_vm_cycle(t_vm *vm)
{
	int			i;
	int			alive;

	++vm->cycle_curr;
	alive = FALSE;
	if (vm->verbosity >= VE_CYCLE)
		ft_printf("{PNK}cycle: %lu{eoc}\n", vm->cycle_curr);
	check_live(vm);
	i = vm->procs.len;
	while (i-- != 0)
	{
		if (vm->verbosity >= VE_REGISTER)
			print_register(vm, &vm->procs.d[i]);
		--vm->procs.d[i].op_cycles;
		if (vm->procs.d[i].op_cycles == 0 && --vm->procs.d[i].op_cycles)
			launch_instruction(vm, i);
		if (vm->procs.d[i].op_cycles == -1)
			read_instruction(vm, i);
		vm->procs.d[i].new_pc = 0;
	}
	if (vm->procs.len == 0)
		return (0);
	return (1);
}
