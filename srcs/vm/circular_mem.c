/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular_mem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 22:54:47 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/21 01:19:37 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		circumem(int ptr)
{
	while (ptr < 0)
		ptr = MEM_SIZE + ptr;
	while (ptr >= MEM_SIZE)
		ptr = ptr - MEM_SIZE;
	return (ptr);
}
