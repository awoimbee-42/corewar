/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:06:53 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/22 20:34:41 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "init.h"
#include <signal.h>
#include <locale.h>

static void	init_ncwindows(t_vm *vm)
{
	int		dim[2];

	getmaxyx(stdscr, dim[1], dim[0]);
	if (dim[0] < 250 || dim[1] < 68)
		exit_vm(vm, gb_add(&vm->gb,
				ft_cprintf("Windows too small (%dx%d), minimum is (250x68)",
					dim[0], dim[1])));
	if (!(vm->visu.rootw = newwin(68, 250, 0, 0))
		|| !(vm->visu.arenaw = subwin(vm->visu.rootw, 64, 193, 2, 2))
		|| !(vm->visu.sidep.rootw = subwin(vm->visu.rootw, 68, 54, 0, 196))
		|| !(vm->visu.sidep.statusw = subwin(vm->visu.sidep.rootw, 57, 48, 2, 199))
		|| !(vm->visu.sidep.printw = subwin(vm->visu.sidep.rootw, 6, 48, 60, 199)))
		exit_vm(vm, "Could not create ncurses windows for visualizer.");
}

static void	init_colors(t_vm *vm)
{
	ft_memset(vm->mem_owner, PLAY0_COLOR - 1, MEM_SIZE);
	/* setup memory color */
	init_pair(CONTOUR_COLOR, COLOR_BRIGHT_BLACK, COLOR_BRIGHT_BLACK);
	init_pair(PLAY0_COLOR - 1, COLOR_BRIGHT_BLACK, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 0, COLOR_GREEN, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 1, COLOR_BLUE, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 2, COLOR_RED, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 3, COLOR_YELLOW, COLOR_BLACK);
	/* setup cursors color */
	init_pair(CURS0_COLOR + -1, COLOR_BLACK, COLOR_WHITE);
	init_pair(CURS0_COLOR + 0, COLOR_BLACK, COLOR_GREEN);
	init_pair(CURS0_COLOR + 1, COLOR_BLACK, COLOR_BLUE);
	init_pair(CURS0_COLOR + 2, COLOR_BLACK, COLOR_RED);
	init_pair(CURS0_COLOR + 3, COLOR_BLACK, COLOR_YELLOW);
	/* setup freshly written memory color */
	init_pair(FRESH0_COLOR + 0, COLOR_BRIGHT_GREEN, COLOR_BLACK);
	init_pair(FRESH0_COLOR + 1, COLOR_BRIGHT_BLUE, COLOR_BLACK);
	init_pair(FRESH0_COLOR + 2, COLOR_BRIGHT_RED, COLOR_BLACK);
	init_pair(FRESH0_COLOR + 3, COLOR_BRIGHT_YELLOW, COLOR_BLACK);
}

void		init_ncurses(t_vm *vm)
{
	setlocale(LC_ALL,"");
	vm->verbosity = VE_VISU;
	vm->visu.op_per_sec = 10;
	vm->visu.paused = TRUE;
	initscr();
	noecho();
	curs_set(FALSE);
	start_color();
	nodelay(stdscr, TRUE);
	init_ncwindows(vm);
	init_colors(vm);
	visu_init_memview(vm);
}
