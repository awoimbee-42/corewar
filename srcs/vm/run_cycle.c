/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 13:03:25 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/17 22:41:16 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

//	si le cycle_to_die tombe en meme temps qu'une instruction live on fait quoi ?
//	quand die_cycle_checks == MAX_CHECKS on décrémente de 1 ou de CYCLE_DELTA ?

static void		read_instruction(t_proc *proc, t_play *play, t_vm *env)
{
	int			op_id;

	op_id = env->arena[proc->pc] - 1;
	if (op_id < 0 || 15 < op_id)
	{
		//proc->pc = (proc->pc + 1) % MEM_SIZE;
		proc->op_cycles = 0;
	}
	else
		proc->op_cycles = g_op[op_id].cycles;
}

static int		loop_player(t_vm *env, t_play *p)
{
	int			i;

	i = p->procs.len;
	while (i-- != 0)
	{
		--p->procs.d[i].op_cycles;
		if (p->procs.d[i].op_cycles == 0 && --p->procs.d[i].op_cycles)
			launch_instruction(env, p, &p->procs.d[i]);
		if (p->procs.d[i].op_cycles == -1)
			read_instruction(&p->procs.d[i], p, env);
	}
	if (p->procs.len == 0)
		return (0);
	return (1);
}

void			check_live(t_vm *vm)
{
	int			nbr_live;
	int			i[2];

	if (vm->cycle_curr % vm->cycle_die)
		return ;
	// ft_printf("check live: %d %% %d\n", vm->cycle_curr, vm->cycle_die);
	vm->die_cycle_checks++;
	nbr_live = 0;
	i[0] = vm->players.len;
	while (i[0]-- != 0)
	{
		i[1] = vm->players.d[i[0]].procs.len;
		while (i[1]-- != 0)
		{
			// ft_printf("\t\tProcess %lu of player %d\n", i[1], vm->players.d[i[0]].id);
			nbr_live += vm->players.d[i[0]].procs.d[i[1]].live;
			if (vm->players.d[i[0]].procs.d[i[1]].live == 0)
			{
				// ft_printf("{red}");
				if (vm->verbosity >= VE_PROCDEATH)
					ft_printf("\tProcess %lu of player %d died\n",
						i[1], vm->players.d[i[0]].id);
				vecproc_del_at(&vm->players.d[i[0]].procs, i[1]);
			}
			vm->players.d[i[0]].procs.d[i[1]].live = 0;
		}
	}
	if (nbr_live < NBR_LIVE || vm->die_cycle_checks == MAX_CHECKS)
	{
		if ((vm->cycle_die -= CYCLE_DELTA) <= 0)
			vm->cycle_die = 1;
		vm->die_cycle_checks = 0;
	}
}

int				run_vm_cycle(t_vm *vm)
{
	int			i;
	int			alive;

	++vm->cycle_curr;
	if (vm->verbosity >= VE_ALL)
		ft_printf("{PNK}cycle: %lu{eoc}\n", vm->cycle_curr);
	check_live(vm);
	i = vm->players.len;
	while (i-- != 0)
	{
		// alive = 0;
		alive = loop_player(vm, &vm->players.d[i]);
		// if (loop_player(vm, &vm->players.d[i]) == 1)
		// 	alive = 1;
	}
	return (alive);
}
