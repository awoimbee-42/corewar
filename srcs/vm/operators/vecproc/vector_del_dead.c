/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_del_dead.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 14:19:54 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 13:56:54 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include <x86intrin.h>

/*
**	Remove all the dead processes
**		A process is dead if it's PID is 0
*/

static void	*proc_memcpy(__m128i *dst, __m128i *src, __m128i *end)
{
	__m128i		a;
	__m128i		b;
	__m128i		c;
	__m128i		d;

	while (src != end)
	{
		a = _mm_stream_load_si128(src++);
		b = _mm_stream_load_si128(src++);
		c = _mm_stream_load_si128(src++);
		d = _mm_stream_load_si128(src++);
		_mm_stream_si128(dst++, a);
		_mm_stream_si128(dst++, b);
		_mm_stream_si128(dst++, c);
		_mm_stream_si128(dst++, d);
	}
	return (dst);
}

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
			end = proc_memcpy(it, it2, end);
			v->len -= (it2 - it);
		}
		it = &it[1];
	}
}
