/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_sidepview.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 18:17:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 15:12:04 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void	print_cycledie_inf(t_vm *vm)
{
	wprintw(vm->visu.sidep.statusw,
				"\n\nCYCLE_TO_DIE: %-10d"
				"\nCycle checks: %-10d",
				vm->cycle_die,
				vm->die_cycle_checks);
}

static void	print_ops(t_vm *vm) //for debug more than anything else
{
	int		i;
	int		j;
	int		pc;

	i = -1;
	while (++i < vm->players.len)
	{
		wattron(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + i));
		j = -1;
		while (++j < vm->players.d[i].procs.len)
		{
			pc = vm->players.d[i].procs.d[j].pc;
			wprintw(vm->visu.sidep.statusw,
				"\n\nOperation: %-10s"
				"\n\tCycles remaining: %-10d",
				g_op[vm->arena[vm->players.d[i].procs.d[j].pc] - 1].name,
				vm->players.d[i].procs.d[j].op_cycles);
		}
		wattroff(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + i));
	}
}

static void	print_players(t_vm *vm)
{
	int		i;
	int		p_nb;

	p_nb = 0;
	i = -1;
	while (++i < vm->players.len)
	{
		wprintw(vm->visu.sidep.statusw, "\n\nPlayer %d: ", vm->players.d[i].id);
		wattron(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + i));
		wprintw(vm->visu.sidep.statusw, "%s", vm->players.d[i].head.prog_name);
		wattroff(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + i));
		wprintw(vm->visu.sidep.statusw,
			"\n\tLast live: N/A"
			"\n\tLives in current period: N/A: N/A");
	}
}

static void	print_proc_nb(t_vm *vm)
{
	int		i;
	int		p_nb;

	p_nb = 0;
	i = -1;
	while (++i < vm->players.len)
	{
		p_nb += vm->players.d[i].procs.len;
	}
	wprintw(vm->visu.sidep.statusw,
		"\nProcesses: %d",
		p_nb);
}


void		visu_sidepview(t_vm *vm)
{
	char	*s;

	wattron(vm->visu.sidep.rootw, COLOR_PAIR(CONTOUR_COLOR));
	box(vm->visu.sidep.rootw, '*', '*');
	wattroff(vm->visu.sidep.rootw, COLOR_PAIR(CONTOUR_COLOR));

	wmove(vm->visu.sidep.statusw, 0, 0);
	s = vm->visu.paused ? "PAUSED" : "RUNNING";
	wprintw(vm->visu.sidep.statusw,
		"** %s **"
		"\n\nMax cycles/second: %.0f"
		"\n\nCycle: %d",
		s, vm->visu.op_per_sec, vm->cycle_curr);
	print_proc_nb(vm);
	print_players(vm);
	print_ops(vm);
	print_cycledie_inf(vm);
}
