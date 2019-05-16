/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_sidepview.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 18:17:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 13:57:18 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

	wattron(vm->visu.sidep.rootw, COLOR_PAIR(32));
	box(vm->visu.sidep.rootw, '*', '*');
	wattroff(vm->visu.sidep.rootw, COLOR_PAIR(32));

	wmove(vm->visu.sidep.statusw, 0, 0);
	s = vm->visu.paused ? "PAUSED" : "RUNNING";
	wprintw(vm->visu.sidep.statusw,
		"** %s **"
		"\n\nMax cycles/second: %.0f"
		"\n\nCycle: %d",
		s, vm->visu.op_per_sec, vm->cycle_curr);
	print_proc_nb(vm);


}
