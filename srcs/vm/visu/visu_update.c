/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 22:29:22 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 14:38:11 by awoimbee         ###   ########.fr       */
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
		if (c == 113)
			vm->visu.op_per_sec -= 10;
		else if (c == 119)
			vm->visu.op_per_sec -= 1;
		else if (c == 101)
			vm->visu.op_per_sec += 1;
		else if (c == 114)
			vm->visu.op_per_sec += 10;
		else if (c == 32)
			vm->visu.paused = vm->visu.paused ? FALSE : TRUE;
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
	// refresh(); // GARBAGE


}
