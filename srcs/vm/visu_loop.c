/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 22:29:22 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/13 00:06:26 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

void		nc_dump_memory(const uint8_t *addr, WINDOW *w)
{
	int				i;
	int				size;
	char			*base;

	base = "0123456789abcdef";
	size = 0;
	while (size != 4096)
	{
		i = -1;
		while (++i < 64)
			wprintw(w, " %02hhx", addr[size + i]);
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
		nc_dump_memory(vm->arena, vm->visu.arenaw);

	}

}
