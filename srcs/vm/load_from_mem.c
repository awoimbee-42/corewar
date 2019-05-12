/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_from_mem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:56:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/12 23:38:00 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

uint32_t		load32(t_vm *vm, t_register pc)
{
	uint32_t	res;
	uint8_t		*r;
	int			i;

	r = (uint8_t*)&res;
	i = -1;
	while (++i < 4)
		r[i] = vm->arena[(pc + (3 - i)) % MEM_SIZE];
	return (res);
}

uint16_t		load16(t_vm *vm, t_register pc)
{
	uint16_t	res;

	res = ((uint16_t)vm->arena[(pc) % MEM_SIZE] << 8)
		| vm->arena[(pc + 1) % MEM_SIZE];
	return (res);
}

uint8_t			load8(t_vm *vm, t_register pc)
{
	uint8_t		res;

	res = vm->arena[(pc) % MEM_SIZE];
	return (res);
}
