/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_mem_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 13:38:45 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/14 14:03:06 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

uint8_t			*write32(t_vm *vm, t_proc *proc, int aptr, uint32_t data)
{
	uint8_t		*r;
	int			i;

	r = (uint8_t*)&data;
	i = -1;
	while (++i < 4)
	{
		vm->arena[(aptr + (3 - i)) % MEM_SIZE] = r[i];
		vm->mem_owner[(aptr + (3 - i)) % MEM_SIZE] = vm->mem_owner[proc->pc];
	}
	return (&vm->arena[aptr]);
}

uint8_t			*write16(t_vm *vm, t_proc *proc, int aptr, uint16_t data)
{
	uint8_t		*r;

	r = (uint8_t*)&data;
	vm->arena[(aptr + 1) % MEM_SIZE] = (uint8_t)data;
	vm->arena[(aptr) % MEM_SIZE] = (uint8_t)(data >> 8);
	vm->mem_owner[(aptr + 1) % MEM_SIZE] = vm->mem_owner[proc->pc];
	vm->mem_owner[aptr % MEM_SIZE] = vm->mem_owner[proc->pc];
	return (&vm->arena[aptr]);
}

uint8_t			*write8(t_vm *vm, t_proc *proc, int aptr, uint8_t data)
{
	vm->arena[aptr % MEM_SIZE] = data;
	vm->mem_owner[aptr % MEM_SIZE] = vm->mem_owner[proc->pc];
	return (&vm->arena[aptr]);
}