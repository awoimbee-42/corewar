/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_mem_load.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:56:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/16 23:04:18 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

int32_t		load32(t_vm *vm, t_register pc)
{
	uint32_t	res;
	uint8_t		*r;
	int			i;

	r = (uint8_t*)&res;
	i = -1;
	while (++i < 4)
		r[i] = vm->arena[circumem(pc + (3 - i))];
	return (res);
}

int16_t		load16(t_vm *vm, t_register pc)
{
	uint16_t	res;

	res = ((uint16_t)vm->arena[circumem(pc)] << 8)
		| vm->arena[circumem(pc + 1)];
	return (res);
}

int8_t			load8(t_vm *vm, t_register pc)
{
	uint8_t		res;

	res = vm->arena[circumem(pc)];
	return (res);
}
