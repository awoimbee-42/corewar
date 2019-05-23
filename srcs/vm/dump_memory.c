/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:12:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 11:43:57 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "corewar.h"
#include "libft.h"
#include "vm.h"

/*
**	Using ft_strcat_join is a bit extreme here as we could pre-calculate the
**		lenght of the string and do a single malloc().
**	But it works, it's safe and it's fast enough.
*/

void	dump_memory(t_vm *vm)
{
	const uint8_t	*ptr;
	int				size;
	int				i;
	char			*out;

	if (vm->dump_width <= 0)
		vm->dump_width = 64;
	out = NULL;
	size = MEM_SIZE;
	ptr = vm->arena;
	while (size)
	{
		ft_asprintf(&out, "0x%04hx : ", ptr - vm->arena);
		i = -1;
		while (++i < vm->dump_width && i < size)
			ft_strcat_join(&out, ft_cprintf("%02hhx ", ptr[i]));
		while (i++ < vm->dump_width)
			ft_strcat_join(&out, ft_cprintf("   "));
		ft_printf("%s\n", out);
		ft_memfree(&out);
		size -= vm->dump_width;
		ptr += vm->dump_width;
	}
}
