/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:59:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/08 18:43:12 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

void			loop_player(t_vm *env, t_play *p)
{
	size_t	i;

	i = -1;
	while (++i < p->procs.len)
	{
		if (!(env->cycle_curr % env->cycle_die) && !p->procs.d[i].live)
		{
			ft_printf("Process %lu of player %d died because he thought that live was not an important instruction\n", i, p->id);
			vecproc_del_at(&p->procs, i);
		}


	}
}

void			loop(t_vm *env)
{
	size_t	i;

	while (env->players.len != 0)
	{
		if (!(env->cycle_curr % env->cycle_die))
			env->die_cycle_checks++;
		if (env->die_cycle_checks == MAX_CHECKS)
		{
			env->die_cycle_checks = 0;
			env->cycle_die -= 1; // 1 ou CYCLE_DELTA ?
		}

		i = -1;
		while (++i < env->players.len)
			loop_player(env, &env->players.d[i]);
		++env->cycle_curr;
	}

}