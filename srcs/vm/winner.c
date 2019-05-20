/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 16:33:52 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/20 17:59:26 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

void		print_winner(t_vm *vm)
{
	int		i;
	int		win_i;

	if (vm->cycle_curr == vm->cycle_dump)
		return ;
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
	ft_printf("Contestant %i \"%s\", has won !\n", win_i + 1, vm->players.d[win_i].head.prog_name);
}

void		print_register(t_play *p, int id)
{
	int		i;
	char	*out;

	out = NULL;
	out = ft_cprintf("Proc: %i.%i  |", p->id, id);
	i = -1;
	while (++i < REG_NUMBER + 1 + MAX_ARGS_NUMBER)
	{
		ft_strcat_join(&out, ft_cprintf(" r%-2i: %-10i", i, p->procs.d[id].reg[i]));
		if (i == REG_NUMBER)
			ft_strcat_join(&out, " |");
	}
	i = ft_strlen(out);
	out[i] = '\n';
	write(STDOUT_FILENO, out, i + 1);
	free(out);
}