/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:13:56 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/07 18:47:16 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecplay		*vecplay_push(t_garbage *gb, t_vecplay *vec, t_play d)
{
	if (vec->len == vec->mem)
		vecplay_realloc(gb, vec);
	vec->d[vec->len++] = d;
	return (vec);
}
