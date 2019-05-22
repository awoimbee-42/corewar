/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_empty.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 18:35:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/22 13:51:19 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"


t_vecproc		*vecproc_push_empty(t_garbage *gb, t_vecproc *vec)
{
	if (vec->len == vec->mem)
		vecproc_realloc(gb, vec);
	ft_bzero(&vec->d[vec->len], sizeof(*vec->d));
	vec->last_pid++;
	vec->d[vec->len].pid = vec->last_pid;
	++vec->len;
	return (vec);
}
