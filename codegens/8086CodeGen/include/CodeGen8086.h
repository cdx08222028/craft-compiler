/*
    Craft Compiler v0.1.0 - The standard compiler for the Craft programming language.
    Copyright (C) 2016  Daniel McCarthy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   CodeGen8086.h
 * Author: Daniel McCarthy
 *
 * Created on 14 September 2016, 03:40
 */

#ifndef CODEGEN8086_H
#define CODEGEN8086_H

#define EXTERNAL_USE

#include <deque>
#include <vector>
#include <memory>
#include <condition_variable>
#include "CodeGenerator.h"
#include "branches.h"

#define POINTER_SIZE 2

enum
{
    GLOBAL_VARIABLE,
    ARGUMENT_VARIABLE,
    SCOPE_VARIABLE
};

struct HANDLING_POINTER
{
    bool is_handling;
    std::shared_ptr<VDEFBranch> pointer_selected_variable;
    std::shared_ptr<VarIdentifierBranch> pointer_selected_var_iden;
};

struct VARIABLE_ADDRESS
{
    std::string segment;
    std::string op;
    int offset;
    std::string apply_reg;

    inline std::string to_string()
    {
        std::string result = segment + op + std::to_string(offset);
        if (apply_reg != "")
        {
            // Second operator is always addition
            result += "+" + apply_reg;
        }

        return result;
    }
};

struct COMPARE_EXPRESSION_DESC
{
    std::string cmp_exp_true_label_name;
    std::string cmp_exp_false_label_name;
    std::string cmp_exp_end_label_name;
    std::string cmp_exp_last_logic_operator;
};

struct exp_compare_info
{

    exp_compare_info()
    {
        use_low_reg = false;
    }
    bool use_low_reg;
};

struct exp_info
{

    void StartCompareExpression()
    {
        std::shared_ptr<struct exp_compare_info> exp_compare_info = std::shared_ptr<struct exp_compare_info>(new struct exp_compare_info());
        last_compare_exp_info = exp_compare_info;
        exp_compares.push_back(exp_compare_info);
    }

    void EndCompareExpression()
    {
        if (!exp_compares.empty())
        {
            exp_compares.pop_back();
        }

        if (!exp_compares.empty())
        {
            last_compare_exp_info = exp_compares.back();
        }
        else
        {
            last_compare_exp_info = NULL;
        }
    }
    std::vector<std::shared_ptr<struct exp_compare_info>> exp_compares;
    std::shared_ptr<struct exp_compare_info> last_compare_exp_info;
};

struct stmt_info
{

    stmt_info()
    {
        is_assignment = false;
        is_assigning_pointer = false;
        is_child_of_pointer = false;
        is_assignment_variable = false;
        assignment_data_size = -1;
        pointer_var_position = "";
        pointer_your_child_of = NULL;
    }
    bool is_assignment;
    bool is_assigning_pointer;
    bool is_child_of_pointer;
    bool is_assignment_variable;
    int assignment_data_size;
    std::string pointer_var_position;
    std::shared_ptr<PTRBranch> pointer_your_child_of;
    struct exp_info exp_info;
};

class FuncBranch;
class ScopeBranch;
class STRUCTAccessBranch;

class CodeGen8086 : public CodeGenerator
{
public:
    CodeGen8086(Compiler* compiler, std::shared_ptr<VirtualObjectFormat> object_format);
    virtual ~CodeGen8086();

    virtual struct formatted_segment format_segment(std::string segment_name);

    inline void make_label(std::string label, std::string segment = "code");
    inline void make_exact_label(std::string label, std::string segment = "code");
    inline std::string make_unique_label(std::string segment = "code");
    inline std::string build_unique_label();

    void setup_comparing();
    void new_breakable_label(std::shared_ptr<Branch> branch_to_stop_reset);
    void end_breakable_label();

    void new_continue_label(std::string label_name, std::shared_ptr<Branch> branch_to_stop_reset);
    void end_continue_label();

    void new_scope(std::shared_ptr<StandardScopeBranch> scope_branch);
    void end_scope();

    std::string make_string(std::shared_ptr<Branch> string_branch);
    void make_inline_asm(struct stmt_info* s_info, std::shared_ptr<ASMBranch> asm_branch);
    void make_variable(std::string name, std::string datatype, std::shared_ptr<Branch> value_exp);
    void make_mem_assignment(std::string dest, std::shared_ptr<Branch> value_exp = NULL, bool is_word = false, std::function<void() > assignment_val_processed = NULL);
    void handle_logical_expression(std::shared_ptr<Branch> exp_branch, struct stmt_info* s_info, bool should_setup = true);
    void make_expression(std::shared_ptr<Branch> exp, struct stmt_info* info, std::function<void() > exp_start_func = NULL, std::function<void() > exp_end_func = NULL);
    void make_expression_part(std::shared_ptr<Branch> exp, std::string register_to_store, struct stmt_info* s_info);
    void make_expression_left(std::shared_ptr<Branch> exp, std::string register_to_store, struct stmt_info* s_info);
    void make_expression_right(std::shared_ptr<Branch> exp, struct stmt_info* s_info);
    bool is_gen_reg_16_bit(std::string reg);
    void make_math_instruction(std::string op, std::string first_reg, std::string second_reg = "");
    void make_compare_instruction(std::string op, std::string first_value, std::string second_value);
    void move_data_to_register(std::string reg, std::string pos, int data_size);
    void dig_bx_to_address(int depth);
    void make_move_reg_variable(std::string reg_name, std::shared_ptr<VarIdentifierBranch> var_branch, struct stmt_info* s_info);
    void make_move_var_addr_to_reg(struct stmt_info* s_info, std::string reg_name, std::shared_ptr<VarIdentifierBranch> var_branch);
    void make_array_offset_instructions(struct stmt_info* s_info, std::shared_ptr<ArrayIndexBranch> array_branch, int size_p_elem = 1);
    void make_move_mem_to_mem(VARIABLE_ADDRESS &dest_loc, VARIABLE_ADDRESS &from_loc, int size);
    void make_move_mem_to_mem(std::string dest_loc, std::string from_loc, int size);
  
    void handle_struct_access(struct stmt_info* s_info, std::shared_ptr<STRUCTAccessBranch> access_branch);
    std::string make_var_access(struct stmt_info* s_info, std::shared_ptr<VarIdentifierBranch> var_branch, int* data_size = NULL);
    void make_appendment(std::string target_reg, std::string op, std::string pos);
    void make_var_assignment(std::shared_ptr<Branch> var_branch, std::shared_ptr<Branch> value, std::string op);
    void make_logical_not(std::shared_ptr<LogicalNotBranch> logical_not_branch, std::string register_to_store, struct stmt_info* s_info);

    void calculate_scope_size(std::shared_ptr<ScopeBranch> scope_branch);
    void reset_scope_size();

    void handle_ptr(struct stmt_info* s_info, std::shared_ptr<PTRBranch> ptr_branch);
    void handle_global_var_def(std::shared_ptr<VDEFBranch> vdef_branch);
    void handle_structure(std::shared_ptr<STRUCTBranch> struct_branch);
    void handle_function_definition(std::shared_ptr<FuncDefBranch> func_def_branch);
    void handle_function(std::shared_ptr<FuncBranch> func_branch);
    void handle_func_args(std::shared_ptr<Branch> arguments);
    void handle_body(struct stmt_info* s_info, std::shared_ptr<Branch> body);
    void handle_stmt(struct stmt_info* s_info, std::shared_ptr<Branch> branch);
    void handle_function_call(std::shared_ptr<FuncCallBranch> branch);
    void handle_scope_assignment(std::shared_ptr<AssignBranch> assign_branch);
    void handle_func_return(struct stmt_info* s_info, std::shared_ptr<ReturnBranch> return_branch);
    void handle_compare_expression();
    void handle_scope_variable_declaration(std::shared_ptr<VDEFBranch> branch);
    void compare_single();
    void handle_if_stmt(std::shared_ptr<IFBranch> branch);
    void handle_for_stmt(std::shared_ptr<FORBranch> branch);
    void handle_while_stmt(std::shared_ptr<WhileBranch> branch);
    void handle_break(std::shared_ptr<BreakBranch> branch);
    void handle_continue(std::shared_ptr<ContinueBranch> branch);
    void handle_array_index(struct stmt_info* s_info, std::shared_ptr<ArrayIndexBranch> array_index_branch, int elem_size);

    int getSizeOfVariableBranch(std::shared_ptr<VDEFBranch> vdef_branch);
    int getStructSize(std::string struct_name);
    bool hasGlobalVariable(std::shared_ptr<VarIdentifierBranch> var_branch);
    bool hasScopeVariable(std::shared_ptr<Branch> var_branch);
    bool hasArgumentVariable(std::shared_ptr<Branch> var_branch);
    int getVariableType(std::shared_ptr<Branch> var_branch);
    std::string convert_full_reg_to_low_reg(std::string reg);

    // scope_handle_func is deprecated
    void scope_handle_func(struct stmt_info* s_info, struct position_info* pos_info);
    void handle_next_access(struct stmt_info* s_info, struct VARIABLE_ADDRESS* address, std::shared_ptr<VarIdentifierBranch> next_var_iden);
    struct VARIABLE_ADDRESS getASMAddressForVariable(struct stmt_info* s_info, std::shared_ptr<VarIdentifierBranch> root_var_branch, bool to_variable_start_only = false);
    std::string getASMAddressForVariableFormatted(struct stmt_info* s_info, std::shared_ptr<VarIdentifierBranch> root_var_branch, bool to_variable_start_only = false);

    std::shared_ptr<VDEFBranch> getVariable(std::shared_ptr<Branch> var_branch);
    std::shared_ptr<Branch> getScopeVariable(std::shared_ptr<Branch> var_branch);
    std::shared_ptr<Branch> getGlobalVariable(std::shared_ptr<Branch> var_branch);
    std::shared_ptr<Branch> getFunctionArgumentVariable(std::shared_ptr<Branch> var_branch);

    bool isVariablePointer(std::shared_ptr<Branch> var_branch);
    inline bool is_cmp_logic_operator_nothing_or_and();
    inline bool is_alone_var_to_be_word(std::shared_ptr<VDEFBranch> vdef_branch, bool ignore_pointer = false);
    inline bool is_alone_var_to_be_word(std::shared_ptr<VarIdentifierBranch> var_branch, bool ignore_pointer = false);

    void generate_global_branch(std::shared_ptr<Branch> branch);
    void assemble(std::string assembly);
private:
    Compiler* compiler;
    std::vector<std::shared_ptr<Branch>> func_arguments;
    std::vector<std::shared_ptr<VDEFBranch>> global_variables;
    std::vector<std::shared_ptr<VDEFBranch>> scope_variables;
    std::vector<std::shared_ptr<STRUCTBranch>> structures;
    std::shared_ptr<STRUCTBranch> last_structure;

    std::string cmp_exp_true_label_name;
    std::string cmp_exp_false_label_name;
    std::string cmp_exp_end_label_name;
    std::string cmp_exp_last_logic_operator;
    bool cmp_exp_handle_if_all_false_follow_through;

    std::string breakable_label;
    std::string continue_label;

    std::deque<std::string> breakable_label_stack;
    std::deque<std::string> continue_label_stack;

    // Basically the branch where the count for the stack pointer to restore should stop
    std::shared_ptr<Branch> breakable_branch_to_stop_reset;
    std::shared_ptr<Branch> continue_branch_to_stop_reset;

    std::deque<std::shared_ptr<Branch>> breakable_branch_to_stop_reset_stack;
    std::deque<std::shared_ptr<Branch>> continue_branch_to_stop_reset_stack;

    bool is_cmp_expression;
    bool do_signed;


    std::shared_ptr<VDEFBranch> last_found_var_access_variable;

    /* Holds current scopes, this is used so nesting of scopes is compatible. */
    std::deque<int> current_scopes_sizes;

    // Holds the current function being generated
    std::shared_ptr<FuncBranch> cur_func;
    int cur_func_scope_size;

    std::shared_ptr<StandardScopeBranch> current_scope;
    std::deque<std::shared_ptr<StandardScopeBranch>> current_scopes;
    std::deque<struct COMPARE_EXPRESSION_DESC> compare_exp_descriptor_stack;
    int current_label_index;
    int scope_size;

};

#endif /* CODEGEN8086_H */

