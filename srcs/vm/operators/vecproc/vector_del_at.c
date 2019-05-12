/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vecproc_del_at.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 19:05:48 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/11 00:13:57 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_del_at(t_vecproc *v, int at)
{
	if (at >= v->len)
		return (NULL);
	--v->len;
	ft_memcpy(&v->d[at], &v->d[at + 1], (v->len - at) * sizeof(*v->d));
	return (v);
}
