/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:12:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/20 15:40:32 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "corewar.h"
#include "libft.h"
#include "vm.h"

static void	ft_putnbr_hex(int byte, int rem)
{
	char const *base = "0123456789abcdef";

	if (rem > 1)
		ft_putnbr_hex(byte >> 4, rem - 1);
	write(1, base + (byte % 16), 1);
}

static void	sp_putstr(const uint8_t *ptr, int len)
{
	while (len--)
	{
		if (' ' <= *ptr && *ptr <= '~')
			write(1, ptr, 1);
		else
			write(1, ".", 1);
		++ptr;
	}
}

void	print_memory(t_vm *env, const void *addr)
{
	int				a;
	int				size;
	const uint8_t	*ptr;

	if (env->dump_width <= 0)
		env->dump_width = 64;
	size = MEM_SIZE;
	ptr = addr;
	while (size)
	{
		write(1, "0x", 2);
		ft_putnbr_hex((ptr - (uint8_t *)addr), 4);
		write(1, " : ", 3);
		{
			a = 0;
			while (a < env->dump_width && a < size)
			{
				ft_putnbr_hex(*(ptr + a), 2);
				write(1, " ", 1);
				a++;
			}
			while (a < env->dump_width)
			{
				write(1, "   ", 3);
				a++;
			}
			if (env->verbosity >= VE_GREET)
				sp_putstr(ptr, size > env->dump_width ? env->dump_width : size);
			write(1, "\n", 1);
		}
		size -= env->dump_width;
		ptr += env->dump_width;

	}
}
