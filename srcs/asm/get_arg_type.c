/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg_type.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:51:30 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/23 11:15:39 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		init_asm_loop(t_asm *my_asm, size_t *i, char *s)
{
	if (my_asm->cursor - sizeof(t_header) > CHAMP_MAX_SIZE)
		fail_msg(my_asm, "Champion too long");
	my_asm->curr_line++;
	*i = 0;
	skip_whitespace(s, i);
	my_asm->curr_char = *i;
}

t_arg_type	get_arg_type_return(t_arg_type type, size_t i, char *s)
{
	skip_whitespace(s, &i);
	if (!s[i] || s[i] == SEPARATOR_CHAR || s[i] == COMMENT_CHAR)
		return (type);
	return (-1);
}

t_arg_type	get_arg_type_end(char *s, size_t i)
{
	if (*s == LABEL_CHAR)
	{
		while (ft_strchr(LABEL_CHARS, s[++i]) && s[i])
			;
		return (get_arg_type_return(T_IND | T_LAB, i, s));
	}
	else if (ft_isdigit(*s) || *s == '-')
	{
		if (*s == '-')
			i++;
		while (ft_isdigit(s[i]))
			i++;
		return (get_arg_type_return(T_IND, i, s));
	}
	return (-1);
}

t_arg_type	get_arg_type(char *s)
{
	size_t	i;

	i = 0;
	if (*s == 'r' || (*s == DIRECT_CHAR && s[1] != LABEL_CHAR))
	{
		if (*s != 'r' && s[1] == '-')
			i++;
		while (ft_isdigit(s[++i]))
			;
		return (get_arg_type_return(*s == 'r' ? T_REG : T_DIR, i, s));
	}
	else if (*s == DIRECT_CHAR && s[1] == LABEL_CHAR)
	{
		++i;
		while (ft_strchr(LABEL_CHARS, s[++i]) && s[i])
			;
		return (get_arg_type_return(T_DIR | T_LAB, i, s));
	}
	else
		return (get_arg_type_end(s, i));
}
