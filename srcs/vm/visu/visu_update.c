/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 22:29:22 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/15 18:42:19 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include <ncurses.h>

void		*visu_khandler(void *vm)
{
	int		c;

	c = getch();
	ft_printf("fdp t'as appuye sur %d\n", c);
	return (NULL);
}

void		visu_update(t_vm *vm)
{
	visu_sidepview(vm);
	visu_memview(vm);
}
