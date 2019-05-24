/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_del_at.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 11:48:47 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 14:14:08 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_del_at(t_vecproc *v, int at)
{
	if (at >= v->len)
		return (NULL);
	--v->len;
	memcpy(&v->d[at], &v->d[at + 1], (v->len - at) * sizeof(*v->d));
	return (v);
}
