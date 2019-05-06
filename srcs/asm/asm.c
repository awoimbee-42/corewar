/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:55:21 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/05 21:33:20 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "libft.h"
#include <fcntl.h>

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


int		read_label(t_label *label, char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != LABEL_CHAR && ft_strchr(LABEL_CHARS, s[i]))
		i++;//add_to_name(&label->name, &i, s[i]);
	if (s[i] != LABEL_CHAR)
		return (0);
	free(label->name);
	if (!(label->name = (char *)ft_memalloc(i + 1)))
		fail_msg("Realloc failed");
	ft_strncpy(label->name, s, i);
	return (1);
}

void	init_labels(t_asm *my_asm)
{
	my_asm->labels = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label));
	if (!my_asm->labels)
		fail_msg("Malloc failed");
	my_asm->labels[LABEL_COUNT].name = ft_strnew(0);
	my_asm->labels_holder = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label));
	if (!my_asm->labels_holder)
		fail_msg("Malloc failed");
	my_asm->labels_holder[LABEL_COUNT].name = ft_strnew(0);
	my_asm->label_pos = 0;
	my_asm->label_holder_pos = 0;
}

void	add_label(t_label **l, size_t *pos, char *name, size_t p)
{
	if ((*l)[*pos].name)
	{
		if (!(*l = (t_label *)realloc(*l, sizeof(t_label) * (*pos * 2 + 1))))
			fail_msg("Realloc failed");
		(*l)[*pos * 2].name = ft_strnew(0);
	}
	(*l)[*pos].name = name;
	(*l)[*pos].pos = p;
	(*pos)++;
}

void		write_to_output(char **output, size_t *pos, char *src)
{
	size_t	start;

	start = *pos;
	while (src[*pos - start])
	{
		if (*pos % OUTPUT_LENGTH == 0)
			if (!(*output = (char *)realloc(*output, *pos + OUTPUT_LENGTH)))
				fail_msg("Realloc failed");
		(*output)[*pos] = src[*pos - start];
		(*pos)++;
	}
}

t_arg_type	get_arg_type(char *s)
{
	size_t	i;

	i = 0;
	if (*s == 'r' || (*s == DIRECT_CHAR && s[1] != LABEL_CHAR))
	{
		while (ft_isdigit(s[++i]))
			;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (*s == 'r' ? T_REG : T_DIR);
		return (-1);
	}
	else if (*s == DIRECT_CHAR && s[1] == LABEL_CHAR)
	{
		++i;
		while (ft_strchr(LABEL_CHARS, s[++i]))
			;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (T_DIR);
		return (-1);
	}
	else if (*s == LABEL_CHAR)
	{
		while (ft_strchr(LABEL_CHARS, s[++i]))
			;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (T_IND);
		return (-1);
	}
	else if (ft_isdigit(*s))
	{
		while (ft_isdigit(s[i]))
			i++;
		skip_whitespace(s, &i);
		if (!s[i] || s[i] == SEPARATOR_CHAR)
			return (T_IND);
		return (-1);
	}
	return (-1);
}

void	write_nb_to_output(t_asm *my_asm, int nb, int pre)
{
	char *s = ft_strnew(pre);
	write_nb(s, nb, pre);
	write_to_output(&my_asm->output, &my_asm->cursor, s);
	free(s);
}

void	handle_op(t_asm *my_asm, char *s)
{
	size_t		i;
	size_t		current_param;
	t_arg_type	current_type;

	write_nb_to_output(my_asm, my_asm->current_op, 2);
	current_param = 0;
	i = 0;
	while (s[i] != ' ' && s[i] != '\t')
		i++;
	skip_whitespace(s, &i);

	if ((current_type = get_arg_type(s + i)) < 0)
		fail_msg("Syntax error for parameter");
	if (!(op_tab[my_asm->current_op - 1].args_types[current_param] & current_type))
		fail_msg("Invalid type");

}

int		get_asm(char *path, t_asm *my_asm)
{
	int		fd;
	char	*s;
	size_t	i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		exit(1);	// Better error handling required...
	my_asm->name[0] = 0;
	my_asm->comment[0] = 0;
	my_asm->cursor = 0;
	init_labels(my_asm);
	while (get_next_line(fd, &s) > 0)
	{
		i = 0;
		skip_whitespace(s, &i);
		if (!ft_strncmp(s + i, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
		{
			if (my_asm->name[0])
				fail_msg("Error: Name already declared");
			if (!get_name(s + i + ft_strlen(NAME_CMD_STRING),
						my_asm->name, PROG_NAME_LENGTH))
				fail_msg("Syntax error: Name not well-formated");
		}
		else if (!ft_strncmp(s + i, COMMENT_CMD_STRING,
					ft_strlen(COMMENT_CMD_STRING)))
		{
			if (my_asm->comment[0])
				fail_msg("Error: Comment already declared");
			if (!get_name(s + i + ft_strlen(COMMENT_CMD_STRING),
						my_asm->comment, COMMENT_LENGTH))
				fail_msg("Syntax error: Comment not well-formated");
		}
		else if ((my_asm->current_op = get_op_id(s + i)))
			handle_op(my_asm, s + i);
		else if (s[i] && s[i] != '#')
		{
			if (!my_asm->name[0] || !my_asm->comment[0])
				fail_msg("Error: Name and Comment must be declared before any instruction or label");
			if (!my_asm->cursor)
			{
				write_nb_to_output(my_asm, COREWAR_EXEC_MAGIC, 8);
				char *magic;// = ft_strnew(8);
				/*write_nb(magic, COREWAR_EXEC_MAGIC, 8);
				write_to_output(&my_asm->output, &my_asm->cursor, magic);
				free(magic);*/
				magic = ft_strnew(PROG_NAME_LENGTH);
				ft_strcpy(magic, my_asm->name);
				write_to_output(&my_asm->output, &my_asm->cursor, magic);
				free(magic);
				magic = ft_strnew(COMMENT_LENGTH);
				ft_strcpy(magic, my_asm->comment);
				write_to_output(&my_asm->output, &my_asm->cursor, magic);
				free(magic);
			}
			//Here, write header to file
			add_label(&my_asm->labels, &my_asm->label_pos, ft_strnew(0), my_asm->cursor);
			if (!read_label(my_asm->labels + my_asm->label_pos - 1, s + i))
				fail_msg("Syntax error on label");
			else
			{
				i += ft_strlen(my_asm->labels[my_asm->label_pos - 1].name) + 1;
				skip_whitespace(s, &i);
				if ((my_asm->current_op = get_op_id(s + i)))
					handle_op(my_asm, s + i);
				printf("Label parsed: %s\n", my_asm->labels[my_asm->label_pos - 1].name);
			}
			// Here, handle potential label definitions.
			// (precision: label names must be strictly followed by the LABEL_CHAR)
			// Else, it's a mistake.
		}
		free(s);
	}

	printf("My name is %s\n", my_asm->name);
	printf("My comment is %s\n", my_asm->comment);

	printf("%s\n", my_asm->output);

	free(my_asm->output);

	// Freeing everything to prevent nazi leaks
	for (int i = 0; i < my_asm->label_pos; i++)
		free(my_asm->labels[i].name);
	for (int i = 0; i < my_asm->label_holder_pos; i++)
		free(my_asm->labels_holder[i].name);
	free(my_asm->labels[(my_asm->label_pos / LABEL_COUNT + 1) * LABEL_COUNT].name);
	free(my_asm->labels_holder[(my_asm->label_holder_pos / LABEL_COUNT + 1) * LABEL_COUNT].name);
	free(my_asm->labels);
	free(my_asm->labels_holder);
	// End of freeing
	return (1);
}

int main(int ac, char **av)
{
	t_asm my_asm;

	if (ac < 2)
		return (0);
	return (get_asm(av[1], &my_asm));
}
