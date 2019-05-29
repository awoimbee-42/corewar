/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:13:56 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 13:28:03 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, t_proc d)
{
	if (vec->len == vec->mem)
		vecproc_realloc(gb, vec);
	vec->d[vec->len] = d;
	vec->d[vec->len].reg = gb_memalloc(gb,
			(REG_NUMBER + 1 + MAX_ARGS_NUMBER) * sizeof(t_register));
	ft_memcpy(vec->d[vec->len].reg, d.reg,
		(REG_NUMBER + 1 + MAX_ARGS_NUMBER) * sizeof(t_register));
	vec->last_pid++;
	vec->d[vec->len].pid = vec->last_pid;
	++vec->len;
	return (vec);
}
