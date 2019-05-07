/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:09:53 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/07 18:53:59 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc	*vecproc_new(t_garbage *gb, size_t reserved_len)
{
	t_vecproc	*vec;

	vec = gb_malloc(gb, reserved_len * sizeof(*vec->d));
	vec->mem = reserved_len;
	vec->len = 0;
	vec->d = gb_malloc(gb, reserved_len * sizeof(*vec->d));
	return (vec);
}
