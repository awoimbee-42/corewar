/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 16:33:52 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/18 16:43:42 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		print_winner(t_vm *vm)
{
	int		i;
	int		win_i;

	win_i = 0;
	i = 0;
	while (++i < vm->players.len)
	{
		if (vm->players.d[i].last_live > vm->players.d[win_i].last_live)
			win_i = i;
		else if (vm->players.d[i].last_live == vm->players.d[win_i].last_live)
			if (vm->players.d[i].period_lives > vm->players.d[win_i].period_lives)
				win_i = i;
	}
	ft_printf("le joueur %d(%s) a gagne", vm->players.d[win_i].id, vm->players.d[win_i].head.prog_name);
}