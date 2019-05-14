/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 18:13:30 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/14 18:07:57 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "asm.h"

void	fail_msg(t_asm *my_asm, char *s)
{
	size_t			len;

	len = ft_strlen(s);
	if (my_asm)
		ft_printf("%s on [%d:%d]\n", s, my_asm->curr_line, my_asm->curr_char);
	else
		ft_printf("Error: %s\n", s);
	exit(EXIT_FAILURE);
}

void	write_nb(char *s, int nb, int byte_nb)
{
	int				i;
	static char		*digits = "0123456789abcdef";

	//printf("Printing %d\n%d\n\n", nb, byte_nb);
	i = -1;
	while (++i < byte_nb)
	{
		s[byte_nb - 1 - i] = nb % 256;//digits[nb % 16];
		//s[byte_nb - 2 - i] = (nb / 256) % 256;//digits[(nb / 16) % 16];
		nb >>= 8;
	}
	//for (int k = 0; k < byte_nb; k++)
	//	printf("RESULTING %d\n", s[k]);
}

void	skip_whitespace(char *s, size_t *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
}
