/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:13:56 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/22 13:51:29 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, t_proc d)
{
	if (vec->len == vec->mem)
		vecproc_realloc(gb, vec);
	vec->d[vec->len] = d;
	vec->last_pid++;
	vec->d[vec->len].pid = vec->last_pid;
	++vec->len;
	return (vec);
}
