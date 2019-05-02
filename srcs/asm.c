/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:55:21 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/02 21:43:16 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/corewar.h"
#include "../libft/libft.h"
#include <fcntl.h>

#include "../ressources/op.c"

void	fail_msg(char *s)
{
	ft_printf("%s\n", s);
	exit(1);
}

void	write_nb(char *s, int nb, int byte_nb)
{
	int				i;
	static char		*digits = "0123456789abcdef";

	i = -1;
	while (++i < byte_nb / 2)
	{
		s[byte_nb - 1 - 2 * i] = digits[nb % 16];
		s[byte_nb - 2 - 2 * i] = digits[(nb / 16) % 16];
		nb >>= 8;
	}
}

void	skip_whitespace(char *s, size_t *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
}

int		get_name(char *s, char name[], size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	skip_whitespace(s, &i);
	if (s[i] != '"')
		return (0);
	i++;
	j = 0;
	while (s[i + j] && s[i + j] != '"' && j < len)
	{
		name[j] = s[i + j];
		j++;
	}
	if (!s[i + j] || !j || (j >= len && s[i + j] != '"'))
		return (0);
	name[j] = '\0';
	return (1);
}

int		get_ocp(int op, t_arg_type args[3])
{
	char res;

	res = 0;
	return (res);
}

int		get_op_id(char *s)
{
	size_t	i;

	i = 0;
	while (i < 16)
	{
		if (!ft_strncmp(s, op_tab[i].name, ft_strlen(op_tab[i].name))
				&& (s[ft_strlen(op_tab[i].name)] == ' '
					|| s[ft_strlen(op_tab[i].name)] == '\t'))
			return (i + 1);
		i++;
	}
	return (0);
}

void	init_labels(t_asm *my_asm)
{
	my_asm->labels = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label));
	if (!my_asm->labels)
		fail_msg("Malloc failed");
	my_asm->labels[LABEL_COUNT].name[0] = LABEL_END;
	my_asm->labels_holder = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label));
	if (!my_asm->labels_holder)
		fail_msg("Malloc failed");
	my_asm->labels_holder[LABEL_COUNT].name[0] = LABEL_END;
}

void	add_label(t_label **l, size_t *pos, t_label *label)
{
	if ((*l)[*pos].name[0] == LABEL_END)
	{
		if (!(*l = (t_label *)realloc(*l, sizeof(t_label) * (*pos * 2 + 1))))
			fail_msg("Realloc failed");
		(*l)[*pos * 2].name[0] = LABEL_END;
	}
	(*l)[*pos] = *label;
	(*pos)++;
}

int		get_asm(char *path)
{
	int		fd;
	char	*s;
	t_asm	my_asm;
	size_t	i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit(1);	// Better error handling required...
	my_asm.name[0] = 0;
	my_asm.comment[0] = 0;
	my_asm.cursor = 0;
	init_labels(&my_asm);
	while (get_next_line(fd, &s) > 0)
	{
		i = 0;
		skip_whitespace(s, &i);
		if (!ft_strncmp(s + i, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
		{
			if (my_asm.name[0])
				fail_msg("Error: Name already declared");
			if (!get_name(s + i + ft_strlen(NAME_CMD_STRING),
						my_asm.name, PROG_NAME_LENGTH))
				fail_msg("Syntax error: Name not well-formated");
		}
		else if (!ft_strncmp(s + i, COMMENT_CMD_STRING,
					ft_strlen(COMMENT_CMD_STRING)))
		{
			if (my_asm.comment[0])
				fail_msg("Error: Comment already declared");
			if (!get_name(s + i + ft_strlen(COMMENT_CMD_STRING),
						my_asm.comment, COMMENT_LENGTH))
				fail_msg("Syntax error: Comment not well-formated");
		}
		else if ((my_asm.current_op = get_op_id(s + i)))
			handle_ope; // Here handle operations writing
		else
		{
			// Here, handle potential label definitions.
			// (precision: label names must be strictly followed by the LABEL_CHAR)
			// Or it can be a comment.
			// Else, it's a mistake.
		}
		free(s);
	}

	printf("My name is %s\n", my_asm.name);
	printf("My comment is %s\n", my_asm.comment);


	free(my_asm.labels);
	free(my_asm.labels_holder);
	return (1);
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (0);
	return (get_asm(av[1]));
}
