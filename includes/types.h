/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/05 21:12:00 by awoimbee          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2019/05/13 18:17:37 by awoimbee         ###   ########.fr       */
=======
/*   Updated: 2019/05/12 20:52:54 by awoimbee         ###   ########.fr       */
>>>>>>> visu
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include "corewar.h"
# include <string.h>

typedef enum	e_bool
{
	FALSE,
	TRUE,
}				t_bool;

typedef struct	s_header
{
	unsigned int		magic;
	char				prog_name[PROG_NAME_LENGTH + 1];
	unsigned int		prog_size;
	char				comment[COMMENT_LENGTH + 1];
}				t_header;

#endif
