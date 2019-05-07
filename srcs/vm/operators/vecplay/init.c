/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:09:53 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/07 18:46:26 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecplay	*vecplay_init(t_garbage *gb, t_vecplay *vec, size_t reserved_len)
{
	vec->mem = reserved_len;
	vec->len = 0;
	vec->d = gb_malloc(gb, reserved_len * sizeof(*vec->d));
	return (vec);
}
