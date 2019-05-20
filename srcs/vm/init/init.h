/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:07:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/20 23:20:24 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H
# include "vm.h"

int				read_champ(t_vm *env, char **input, int i);
void			champs_setup(t_vm *vm);
void			init_ncurses(t_vm *vm);

#endif
