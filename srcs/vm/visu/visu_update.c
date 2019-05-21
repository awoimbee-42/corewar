/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 22:29:22 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/21 22:26:36 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include <ncurses.h>
#include <time.h>

void		visu_khandler(t_vm *vm)
{
	int		c;

	while ((c = getch()) != ERR)
	{
		if (c == NC_Q)
			vm->visu.op_per_sec -= 10;
		else if (c == NC_W)
			vm->visu.op_per_sec -= 1;
		else if (c == NC_E)
			vm->visu.op_per_sec += 1;
		else if (c == NC_R)
			vm->visu.op_per_sec += 10;
		else if (c == NC_SPC)
			vm->visu.paused = vm->visu.paused ? FALSE : TRUE;
		else if (c == NC_1)
			vm->visu.op_per_sec = 50;
		else if (c == NC_2)
			vm->visu.op_per_sec = 9999;
		else if (c == NC_ESC)
			exit_vm(vm, "exit success"); // <- We still print 'Error', which sucks
		// ft_strcpy(vm->visu.aff, gb_add(&vm->gb, ft_cprintf("Key pressed: %d", c)));
	}
	if (vm->visu.op_per_sec < 1)
		vm->visu.op_per_sec = 1;
}

/*
**	getch() f***s everything up so I have to redraw EVERYTHING, EVERYTIME
**	This piece of SH*T IS SO F***ING INEFFICIENT LIKE WTF MAN
**	WHY.
*/

void		visu_update(t_vm *vm)
{
	visu_khandler(vm); // BIG PIECE OF HOT POO HERE

	wattron(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
	box(vm->visu.rootw, '*', '*');
	wattroff(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
	// wbkgd(vm->visu.arenaw, COLOR_PAIR(0));
	visu_sidepview(vm);
	visu_memview(vm);
	wrefresh(vm->visu.rootw);
	wrefresh(vm->visu.arenaw);
	wrefresh(vm->visu.sidep.rootw);
	wrefresh(vm->visu.sidep.statusw);
}

void		visu_endloop(t_vm *vm, int winner)
{
	 ft_strcpy(vm->visu.aff, gb_add(&vm->gb, ft_cprintf("Contestant %i, \"%s\", has won !\nPress ESC to quit.", vm->players.d[winner].id, vm->players.d[winner].head.prog_name)));
	while (1)
	{
		visu_khandler(vm); // BIG PIECE OF HOT POO HERE
		wattron(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
		box(vm->visu.rootw, '*', '*');
		wattroff(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
		visu_sidepview(vm);
		visu_memview(vm);
		wrefresh(vm->visu.rootw);
		wrefresh(vm->visu.arenaw);
		wrefresh(vm->visu.sidep.rootw);
		wrefresh(vm->visu.sidep.statusw);
	}
}
