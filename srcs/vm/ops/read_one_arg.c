/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_one_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 22:57:41 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/17 22:57:58 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static int		load_reg(t_vm *vm, t_proc *proc, int op_id, int cur_arg)
{
	int		tmp;

	tmp = load8(vm, proc->new_pc);
	proc->new_pc = (proc->new_pc + 1) % MEM_SIZE;
	return (tmp <= REG_NUMBER && tmp >= 1 && (T_REG & g_op[op_id].args_type[cur_arg]) ? tmp : -1);
}

static int		load_dir(t_vm *vm, t_proc *proc, int op_id, int cur_arg)
{
	proc->reg[REG_NUMBER + cur_arg] = g_op[op_id].dir2 ? load16(vm, proc->new_pc) : load32(vm, proc->new_pc);
	proc->new_pc = g_op[op_id].dir2 ? (proc->new_pc + 2) % MEM_SIZE : (proc->new_pc + 4) % MEM_SIZE;
	if (!(T_DIR & g_op[op_id].args_type[cur_arg]))
		return (-1);
	else
		return (REG_NUMBER + cur_arg);
}

static int		load_ind(t_vm *vm, t_proc *proc, int op_id, int cur_arg)
{
	if (g_op[op_id].ldx_rel)
		proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + (load16(vm, proc->new_pc) % IDX_MOD));
	else
		proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + load16(vm, proc->new_pc));
	proc->new_pc = (proc->new_pc + 2) % MEM_SIZE;
	if (!(T_IND & g_op[op_id].args_type[cur_arg]))
		return (-1);
	else
		return (REG_NUMBER + cur_arg);
}

int				read_one_arg(t_vm *vm, t_proc *proc, uint8_t cb, int cur_arg)
{
	int		op_id;
	int		tmp;

	op_id = vm->arena[proc->pc] - 1;
	if (cb == REG_CODE)
		return (load_reg(vm, proc, op_id, cur_arg));
	else if (cb == DIR_CODE)
		return (load_dir(vm, proc, op_id, cur_arg));
	else if (cb == IND_CODE)
		return (load_ind(vm, proc, op_id, cur_arg));
	else
		return (-1);
}