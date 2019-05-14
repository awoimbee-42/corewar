/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:12:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/12 22:46:25 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "corewar.h"
#include "libft.h"

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

void	print_memory(const void *addr, size_t size)
{
	size_t			a;
	const uint8_t	*ptr;

	ptr = addr;
	while (size)
	{
		if (ptr - 16 >= (uint8_t*)addr && !ft_memcmp(ptr - 16, ptr, size > 16 ? 16 : size))
		{
			if (a != 20)
				write(1, "*\n", 2);
			a = 20;
		}
		else
		{
			a = 0;
			while (a < 16 && a < size)
			{
				ft_putnbr_hex(*(ptr + a), 2);
				if (a % 2)
					write(1, " ", 1);
				a++;
			}
			while (a < 16)
			{
				write(1, "  ", 2);
				if (a % 2)
					write(1, " ", 1);
				a++;
			}
			sp_putstr(ptr, size > 16 ? 16 : size);
			write(1, "\n", 1);
		}

		size -= 16;
		ptr += 16;

	}
}
