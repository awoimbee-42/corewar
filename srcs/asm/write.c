/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:55:36 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/14 19:00:56 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		write_to_output(char **output, size_t *pos, char *src)
{
	size_t	start;

	start = *pos;
	while (src[*pos - start])
	{
		if (*pos % OUTPUT_LENGTH == 0)
		{
			if (!(*output = (char *)realloc(*output, *pos + OUTPUT_LENGTH)))
				fail_msg(0, "Realloc failed");
			ft_bzero(*output + *pos, OUTPUT_LENGTH - 1);
		}
		(*output)[*pos] = src[*pos - start];
		(*pos)++;
	}
}

void		write_n_to_output(char **output, size_t *pos, char *src, size_t n)
{
	size_t	start;

	start = *pos;
	while (*pos - start < n)
	{
		if (*pos % OUTPUT_LENGTH == 0)
		{
			if (!(*output = (char *)realloc(*output, *pos + OUTPUT_LENGTH)))
				fail_msg(0, "Realloc failed");
			ft_bzero(*output + *pos, OUTPUT_LENGTH);
		}
		(*output)[*pos] = src[*pos - start];
		(*pos)++;
	}
}

void		write_nb_to_output(t_asm *my_asm, int nb, int pre)
{
	char	*s;

	s = ft_strnew(pre);
	if (pre > 1)
		write_nb(s, nb, pre);
	else
		s[0] = nb;
	write_n_to_output(&my_asm->output, &my_asm->cursor, s, pre);
	free(s);
}

void		write_output(t_asm *my_asm, char *path)
{
	int		fd;
	char	*new_path;

	fd = open(my_asm->file_name, O_CREAT | O_WRONLY, 0777);
	if (fd >= 0)
		write(fd, my_asm->output, my_asm->cursor);
	else
		fail_msg(0, "Cannot write to output file");
	close(fd);
	printf("Written output program to %s\n", my_asm->file_name);
	free(my_asm->file_name);
}

void		write_opcode(t_asm *my_asm, t_arg_type types[3])
{
	size_t	i;

	i = -1;
	while (++i < 3)
	{
		if (types[i] & T_REG)
			my_asm->output[my_asm->op_begin + 1] += 1;
		else if (types[i] & T_IND)
			my_asm->output[my_asm->op_begin + 1] += 3;
		else if (types[i] & T_DIR)
			my_asm->output[my_asm->op_begin + 1] += 2;
		my_asm->output[my_asm->op_begin + 1] <<= 2;
	}
}
