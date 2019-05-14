/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 22:29:22 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/13 04:11:56 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

const char	*g_pcolors[] = {"\x1b[31m", "\e[32m", "\e[33m", "\e[34m"};

void		visu_memview(t_vm *vm)
{
	int				i;
	int				mem;
	int				mem_pc;

	wmove(vm->visu.arenaw, 0, 0);
	mem = 0;
	while (mem != 4096)
	{
		i = -1;
		while (++i < 64)
		{
			wattron(vm->visu.arenaw, COLOR_PAIR(1));
			wprintw(vm->visu.arenaw, " %02hhx", vm->arena[mem + i]);  // add color escape code for each player;
			wattroff(vm->visu.arenaw, COLOR_PAIR(1));
		}
		mem += 64;
		wprintw(vm->visu.arenaw, "\n", vm->arena[mem]);
	}
	/*
	i = -1;
	while (++i < vm->players.len)
	{
		wmove(vm->visu.arenaw, mem_pc / 64, mem_pc % 64);
		mem = 0;
		mem_pc = vm->players.d[i].procs.d->pc;
		while (mem < vm->players.d[i].head.prog_size)
		{
			wrefresh(vm->visu.arenaw);
			sleep(1);
			if (mem_pc % 64 == 0)
				wprintw(vm->visu.arenaw, "\n");
			wattron(vm->visu.arenaw, COLOR_PAIR(2));
			wprintw(vm->visu.arenaw, " %02hhx", vm->arena[mem_pc]);  // add color escape code for each player
			wattroff(vm->visu.arenaw, COLOR_PAIR(2));
			mem_pc++;
			mem++;
		}
	}*/
	wrefresh(vm->visu.arenaw);
}

void		nc_dump_memory(const uint8_t *addr, WINDOW *w)
{
	int				i;
	int				size;

	size = 0;
	while (size != 4096)
	{
		i = -1;
		while (++i < 64)
			wprintw(w, "   ");
		wprintw(w, "\n", addr[size + i]);
		size += 64;
	}


	wmove(w, 0, 0);
	wrefresh(w);
}

// dump 64 * 64 bytes


void		visu_loop(t_vm *vm)
{
	while (1)
	{
		// ft_printf("{RED}PUTE{eoc}\n");
		visu_memview(vm);
		//nc_dump_memory(vm->arena, vm->visu.arenaw);

	}

}
