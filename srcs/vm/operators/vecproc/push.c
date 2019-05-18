/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:13:56 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/18 16:52:28 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, t_proc d, t_vm *env)
{
	if (vec->len == vec->mem)
		vecproc_realloc(gb, vec);
	vec->d[vec->len++] = d;
	// vec->d[vec->len - 1].carry = 1;
	read_instruction(&vec->d[vec->len - 1], env);
	return (vec);
}
