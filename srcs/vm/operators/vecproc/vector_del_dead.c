/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_del_dead.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 14:19:54 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 18:05:25 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include <x86intrin.h>

/*
**	Remove all the dead processes
**		A process is dead if it's PID is 0
*/

/*
**	proc_memcpy is specially configured for copying 1 or more t_proc
**		'special_memcpy(it, it2, end);' can be replaced with:
**			ft_memcpy(it, it2, (end - it2) * sizeof(t_proc));
**			end = &v->d[v->len];
*/

void		vecproc_del_dead(t_vecproc *v)
{
	t_proc		*end;
	t_proc		*it;
	t_proc		*it2;

	end = &v->d[v->len];
	it = v->d;
	while (it < end)
	{
		if (it->pid == 0)
		{
			it2 = &it[1];
			while (it2 != end && it2->pid == 0)
				it2 = &it2[1];
			ft_memcpy(it, it2, (end - it2) * sizeof(t_proc));
			v->len -= (it2 - it);
			end = &v->d[v->len];
		}
		it = &it[1];
	}
}
