/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_memview.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:57:21 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 14:39:51 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		visu_init_memview(t_vm *vm)
{
	int				i;
	int				mem_pc;
	uint			mem;

	i = -1;
	while (++i < vm->players.len)
	{
		wmove(vm->visu.arenaw, mem_pc / 64, mem_pc % 64);
		mem = 0;
		mem_pc = vm->players.d[i].procs.d->pc;
		wattron(vm->visu.arenaw, COLOR_PAIR(PLAY0_COLOR + i));
		while (mem < vm->players.d[i].head.prog_size)
		{
			if (mem_pc % 64 == 0)
				wprintw(vm->visu.arenaw, "\n");
			wprintw(vm->visu.arenaw, " %02hhx", vm->arena[mem_pc]);
			vm->mem_owner[mem_pc] = i + PLAY0_COLOR;
			mem_pc++;
			mem++;
		}
		wattroff(vm->visu.arenaw, COLOR_PAIR(i + PLAY0_COLOR));
	}
}

static void	memview_cursors(t_vm *vm)
{
	int				pl;
	int				pr;
	int				pc;

	pl = -1;
	while (++pl < vm->players.len)
	{
		pr = -1;
		while (++pr < vm->players.d[pl].procs.len)
		{
			pc = vm->players.d[pl].procs.d[pr].pc;
			wmove(vm->visu.arenaw, pc / 64, (pc % 64) * 3 + 1);
			wattron(vm->visu.arenaw, COLOR_PAIR(vm->mem_owner[pc] + DELT_CURS_COLOR));
			wprintw(vm->visu.arenaw, "%02hhx", vm->arena[pc]);
			wattroff(vm->visu.arenaw, COLOR_PAIR(vm->mem_owner[pc] + DELT_CURS_COLOR));
		}
	}
}

void		visu_memview(t_vm *vm)
{
	int				i;
	int				mem;

	wmove(vm->visu.arenaw, 0, 0);
	mem = 0;
	while (mem != 4096)
	{
		i = -1;
		while (++i < 64)
		{
			wattron(vm->visu.arenaw, COLOR_PAIR(vm->mem_owner[mem + i]));
			wprintw(vm->visu.arenaw, " %02hhx", vm->arena[mem + i]);
			wattroff(vm->visu.arenaw, COLOR_PAIR(vm->mem_owner[mem + i]));
			if (vm->mem_owner[mem + i] >= FRESH0_COLOR)
				vm->mem_owner[mem + i] -= DELT_FRESH_COLOR;
		}
		mem += 64;
		wprintw(vm->visu.arenaw, "\n", vm->arena[mem]);
	}
	memview_cursors(vm);
}
