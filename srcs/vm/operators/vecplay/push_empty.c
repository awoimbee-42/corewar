/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_empty.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 18:35:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/07 20:59:11 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecplay		*vecplay_push_empty(t_garbage *gb, t_vecplay *vec)
{
	if (vec->len == vec->mem)
		vecplay_realloc(gb, vec);
	ft_bzero(&vec->d[vec->len], sizeof(*vec->d));
	++vec->len;
	return (vec);
}
