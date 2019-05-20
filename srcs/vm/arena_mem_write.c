/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_mem_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 13:38:45 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/21 00:23:01 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

uint8_t			*write32(t_vm *vm, int pc, int aptr, uint32_t data)
{
	uint8_t		*r;
	int			i;
	uint8_t		new_owner;
	int			tmp;

	r = (uint8_t*)&data;
	new_owner = vm->mem_owner[pc] + DELT_FRESH_COLOR;
	i = -1;
	while (++i < 4)
	{
		tmp = circumem(aptr + (3 - i));
		// if (aptr < 0)
		// 	ft_printf("write to addr: %d, before circumem: %d, before anything: %d\n", tmp, aptr + (3 - i), aptr);
		vm->arena[tmp] = r[i];
		vm->mem_owner[tmp] = new_owner;
	}
	return (&vm->arena[aptr]);
}

uint8_t			*write16(t_vm *vm, int pc, int aptr, uint16_t data)
{
	uint8_t		new_owner;
	int			aptr1;

	aptr = circumem(aptr);
	aptr1 = circumem(aptr + 1);
	vm->arena[aptr1] = (uint8_t)data;
	vm->arena[aptr] = (uint8_t)(data >> 8);
	new_owner = vm->mem_owner[pc] + DELT_FRESH_COLOR;
	vm->mem_owner[aptr1] = new_owner;
	vm->mem_owner[aptr] = new_owner;
	return (&vm->arena[aptr]);
}

uint8_t			*write8(t_vm *vm, int pc, int aptr, uint8_t data)
{
	aptr = circumem(aptr);
	vm->arena[aptr] = data;
	vm->mem_owner[aptr] = vm->mem_owner[pc] + DELT_FRESH_COLOR;
	return (&vm->arena[aptr]);
}
