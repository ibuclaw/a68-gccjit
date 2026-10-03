/* Implementations of formal holes that convert between Algol68 and C types.
Copyright (C) 2026 Iain Buclaw.

This file is part of a68-gccjit.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>  */
#include <libgccjit.h>
#include <stdint.h>
#include <stdlib.h>

/* Convert from Algol68 UCS-4 `string` to C UTF8 `char *`.  */
static char *
ucs4_to_utf8 (const uint32_t *s, size_t n, size_t stride)
{
  size_t length = 0;
  for (size_t i = 0; i < n; i++)
    {
      uint32_t c = *(const uint32_t *)((const char *)s + i * stride);
      if (c < 0x80)
	length += 1;
      else if (c < 0x800)
	length += 2;
      else if (c < 0x10000)
	length += 3;
      else
	length += 4;
    }

  uint8_t *result = (uint8_t *) malloc (length * sizeof (uint8_t) + 1);
  size_t p = 0;
  for (size_t i = 0; i < n; i++)
    {
      uint32_t c = *(const uint32_t *)((const char *)s + i * stride);
      if (c < 0x80)
	{
	  result[p] = c;
	  p += 1;
	}
      else if (c < 0x800)
	{
	  result[p]     = 0xc0 + c / 0x40;
	  result[p + 1] = 0x80 + c % 0x40;
	  p += 2;
	}
      else if (c < 0x10000)
	{
	  result[p]     = 0xe0 + c / 0x1000;
	  result[p + 1] = 0x80 + (c / 0x40) % 0x40;
	  result[p + 2] = 0x80 + c % 0x40;
	  p += 3;
	}
      else
	{
	  result[p]     = 0xf0 + c / 0x40000;
	  result[p + 1] = 0x80 + (c / 0x1000) % 0x40;
	  result[p + 2] = 0x80 + (c / 0x40) % 0x40;
	  result[p + 3] = 0x80 + c % 0x40;
	  p += 4;
	}
    }
  result[length] = '\0';
  return result;
}

/* `nest C` formal holes that need a little more assistance for mapping between
   Algol68 types to C types.  */
void
jit68_context_set_str_option (gcc_jit_context *ctxt,
			      enum gcc_jit_str_option opt,
			      uint32_t *s, size_t len, size_t stride)
{
  char *value = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_set_str_option (ctxt, opt, value);
  free (value);
}

void
jit68_context_add_command_line_option (gcc_jit_context *ctxt,
				       uint32_t *s, size_t len, size_t stride)
{
  char *optname = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_add_command_line_option, (ctxt, optname);
  free (optname);
}

void
jit68_context_add_driver_option (gcc_jit_context *ctxt,
				 uint32_t *s, size_t len, size_t stride)
{
  char *optname = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_add_driver_option (ctxt, optname);
  free (optname);
}

void
jit68_context_compile_to_file (gcc_jit_context *ctxt,
			       enum gcc_jit_output_kind kind,
			       uint32_t *s, size_t len, size_t stride)
{
  char *output_path = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_compile_to_file (ctxt, kind, output_path);
  free (output_path);
}

void
jit68_context_dump_to_file (gcc_jit_context *ctxt,
			    uint32_t *s, size_t len, size_t stride,
			    int update_locations)
{
  char *path = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_dump_to_file (ctxt, path, update_locations);
  free (path);
}

void *
jit68_result_get_code (gcc_jit_result *res,
		       uint32_t *s, size_t len, size_t stride)
{
  char *funcname = ucs4_to_utf8 (s, len, stride);
  void *result = gcc_jit_result_get_code (res, funcname);
  free (funcname);
  return result;
}

void *
jit68_result_get_global (gcc_jit_result *res,
			 uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  void *result = gcc_jit_result_get_global (res, name);
  free (name);
  return result;
}

gcc_jit_location *
jit68_context_new_location (gcc_jit_context *ctxt,
			    uint32_t *s, size_t len, size_t stride,
			    int line, int column)
{
  char *filename = ucs4_to_utf8 (s, len, stride);
  gcc_jit_location *result = gcc_jit_context_new_location (ctxt, filename,
							   line, column);
  free (filename);
  return result;
}

gcc_jit_field *
jit68_context_new_field (gcc_jit_context *ctxt,
			 gcc_jit_location *loc,
			 gcc_jit_type *type,
			 uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_field *result = gcc_jit_context_new_field (ctxt, loc, type, name);
  free (name);
  return result;
}

gcc_jit_field *
jit68_context_new_bitfield (gcc_jit_context *ctxt,
			    gcc_jit_location *loc,
			    gcc_jit_type *type,
			    int width,
			    uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_field *result = gcc_jit_context_new_bitfield (ctxt, loc, type, width,
							name);
  free (name);
  return result;
}

gcc_jit_struct *
jit68_context_new_struct_type (gcc_jit_context *ctxt,
			       gcc_jit_location *loc,
			       uint32_t *s, size_t len, size_t stride,
			       gcc_jit_field **fields,
			       size_t num_fields, size_t /*stride*/)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_struct *result = gcc_jit_context_new_struct_type (ctxt, loc, name,
							    num_fields, fields);
  free (name);
  return result;
}

gcc_jit_struct *
jit68_context_new_opaque_struct (gcc_jit_context *ctxt,
				 gcc_jit_location *loc,
				 uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_struct *result = gcc_jit_context_new_opaque_struct (ctxt, loc, name);
  free (name);
  return result;
}

void
jit68_struct_set_fields (gcc_jit_struct *struct_type,
			 gcc_jit_location *loc,
			 gcc_jit_field **fields,
			 size_t num_fields, size_t /*stride*/)
{
  gcc_jit_struct_set_fields (struct_type, loc, num_fields, fields);
}

gcc_jit_type *
jit68_context_new_union_type (gcc_jit_context *ctxt,
			      gcc_jit_location *loc,
			      uint32_t *s, size_t len, size_t stride,
			      gcc_jit_field **fields,
			      size_t num_fields, size_t /*stride*/)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_type *result = gcc_jit_context_new_union_type (ctxt, loc, name,
							 num_fields, fields);
  free (name);
  return result;
}

gcc_jit_type *
jit68_context_new_function_ptr_type (gcc_jit_context *ctxt,
				     gcc_jit_location *loc,
				     gcc_jit_type *return_type,
				     gcc_jit_type **param_types,
				     size_t num_params, size_t /*stride*/,
				     int is_variadic)
{
  return gcc_jit_context_new_function_ptr_type (ctxt, loc, return_type,
						num_params, param_types,
						is_variadic);
}

gcc_jit_param *
jit68_context_new_param (gcc_jit_context *ctxt,
			 gcc_jit_location *loc,
			 gcc_jit_type *type,
			 uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_param *result = gcc_jit_context_new_param (ctxt, loc, type, name);
  free (name);
  return result;
}

gcc_jit_function *
jit68_context_new_function (gcc_jit_context *ctxt, gcc_jit_location *loc,
			    enum gcc_jit_function_kind kind,
			    gcc_jit_type *return_type,
			    uint32_t *s, size_t len, size_t stride,
			    gcc_jit_param **params,
			    size_t num_params, size_t /*stride*/,
			    int is_variadic)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_function *result = gcc_jit_context_new_function (ctxt, loc, kind,
							   return_type, name,
							   num_params, params,
							   is_variadic);
  free (name);
  return result;
}

gcc_jit_function *
jit68_context_get_builtin_function (gcc_jit_context *ctxt,
				    uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_function *result = gcc_jit_context_get_builtin_function (ctxt, name);
  free (name);
  return result;
}

void
jit68_function_dump_to_dot (gcc_jit_function *func,
			    uint32_t *s, size_t len, size_t stride)
{
  char *path = ucs4_to_utf8 (s, len, stride);
  gcc_jit_function_dump_to_dot (func, path);
  free (path);
}

gcc_jit_block *
jit68_function_new_block (gcc_jit_function *func,
			  uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_block *result = gcc_jit_function_new_block (func, name);
  free (name);
  return result;
}

gcc_jit_lvalue *
jit68_context_new_global (gcc_jit_context *ctxt, gcc_jit_location *loc,
			  enum gcc_jit_global_kind kind, gcc_jit_type *type,
			  uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_lvalue *result = gcc_jit_context_new_global (ctxt, loc, kind, type,
						       name);
  free (name);
  return result;
}

gcc_jit_rvalue *
jit68_context_new_struct_constructor (gcc_jit_context *ctxt,
				      gcc_jit_location *loc,
				      gcc_jit_type *type,
				      gcc_jit_field **fields,
				      size_t /*num_fields*/, size_t /*stride*/,
				      gcc_jit_rvalue **values,
				      size_t num_values, size_t /*stride*/)
{
  return gcc_jit_context_new_struct_constructor (ctxt, loc, type, num_values,
						 fields, values);
}

gcc_jit_rvalue *
jit68_context_new_array_constructor (gcc_jit_context *ctxt,
				     gcc_jit_location *loc,
				     gcc_jit_type *type,
				     gcc_jit_rvalue **values,
				     size_t num_values, size_t /*stride*/)
{
  return gcc_jit_context_new_array_constructor (ctxt, loc, type,
						num_values, values);
}

gcc_jit_function *
jit68_context_get_target_builtin_function (gcc_jit_context *ctxt,
					   uint32_t *s, size_t len,
					   size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_function *result =
    gcc_jit_context_get_target_builtin_function (ctxt, name);
  free (name);
  return result;
}

gcc_jit_rvalue *
jit68_context_new_string_literal (gcc_jit_context *ctxt,
				  uint32_t *s, size_t len, size_t stride)
{
  char *value = ucs4_to_utf8 (s, len, stride);
  gcc_jit_rvalue *result = gcc_jit_context_new_string_literal (ctxt, value);
  free (value);
  return result;
}

gcc_jit_rvalue *
jit68_context_new_call (gcc_jit_context *ctxt, gcc_jit_location *loc,
			gcc_jit_function *func, gcc_jit_rvalue **args,
			size_t numargs, size_t /*stride*/)
{
  return gcc_jit_context_new_call (ctxt, loc, func, numargs, args);
}

gcc_jit_rvalue *
jit68_context_new_call_through_ptr (gcc_jit_context *ctxt,
				    gcc_jit_location *loc,
				    gcc_jit_rvalue *fn_ptr,
				    gcc_jit_rvalue **args,
				    size_t numargs, size_t /*stride*/)
{
  return gcc_jit_context_new_call_through_ptr (ctxt, loc, fn_ptr,
					       numargs, args);
}

void
jit68_lvalue_set_link_section (gcc_jit_lvalue *lvalue,
			       uint32_t *s, size_t len, size_t stride)
{
  char *section_name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_lvalue_set_link_section (lvalue, section_name);
  free (section_name);
}

void
jit68_lvalue_set_register_name (gcc_jit_lvalue *lvalue,
				uint32_t *s, size_t len, size_t stride)
{
  char *reg_name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_lvalue_set_register_name (lvalue, reg_name);
  free (reg_name);
}

gcc_jit_lvalue *
jit68_function_new_local (gcc_jit_function *func, gcc_jit_location *loc,
			  gcc_jit_type *type,
			  uint32_t *s, size_t len, size_t stride)
{
  char *name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_lvalue *result = gcc_jit_function_new_local (func, loc, type, name);
  free (name);
  return result;
}

void
jit68_block_add_comment (gcc_jit_block *block,
			 gcc_jit_location *loc,
			 uint32_t *s, size_t len, size_t stride)
{
  char *text = ucs4_to_utf8 (s, len, stride);
  gcc_jit_block_add_comment (block, loc, text);
  free (text);
}

void
jit68_block_end_with_switch (gcc_jit_block *block, gcc_jit_location *loc,
			     gcc_jit_rvalue *expr, gcc_jit_block *default_block,
			     gcc_jit_case **cases,
			     size_t num_cases, size_t /*stride*/)
{
  gcc_jit_block_end_with_switch (block, loc, expr, default_block,
				 num_cases, cases);
}

void
jit68_context_dump_reproducer_to_file (gcc_jit_context *ctxt,
				       uint32_t *s, size_t len, size_t stride)
{
  char *path = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_dump_reproducer_to_file (ctxt, path);
  free (path);
}

void
jit68_context_enable_dump (gcc_jit_context *ctxt,
			   uint32_t *s, size_t len, size_t stride,
			   char **out_ptr)
{
  char *dumpname = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_enable_dump (ctxt, dumpname, out_ptr);
  free (dumpname);
}

void
jit68_timer_push (gcc_jit_timer *timer,
		  uint32_t *s, size_t len, size_t stride)
{
  char *item_name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_timer_push (timer, item_name);
  free (item_name);
}

void
jit68_timer_pop (gcc_jit_timer *timer,
		 uint32_t *s, size_t len, size_t stride)
{
  char *item_name = ucs4_to_utf8 (s, len, stride);
  gcc_jit_timer_pop (timer, item_name);
  free (item_name);
}

gcc_jit_rvalue *
jit68_context_new_rvalue_from_vector (gcc_jit_context *ctxt,
				      gcc_jit_location *loc,
				      gcc_jit_type *vec_type,
				      gcc_jit_rvalue **elements,
				      size_t num_elements, size_t /*stride*/)
{
  return gcc_jit_context_new_rvalue_from_vector (ctxt, loc, vec_type,
						 num_elements, elements);
}

gcc_jit_extended_asm *
jit68_block_add_extended_asm (gcc_jit_block *block,
			      gcc_jit_location *loc,
			      uint32_t *s, size_t len, size_t stride)
{
  char *asm_template = ucs4_to_utf8 (s, len, stride);
  gcc_jit_extended_asm *result = gcc_jit_block_add_extended_asm (block, loc,
								 asm_template);
  free (asm_template);
  return result;
}

gcc_jit_extended_asm *
jit68_block_end_with_extended_asm_goto (gcc_jit_block *block,
					gcc_jit_location *loc,
					uint32_t *s, size_t len, size_t stride,
					gcc_jit_block **goto_blocks,
					size_t num_goto_blocks,
					size_t /*stride*/,
					gcc_jit_block *fallthrough_block)
{
  char *asm_template = ucs4_to_utf8 (s, len, stride);
  gcc_jit_extended_asm *result =
    gcc_jit_block_end_with_extended_asm_goto (block, loc, asm_template,
					      num_goto_blocks, goto_blocks,
					      fallthrough_block);
  free (asm_template);
  return result;
}

void
jit68_extended_asm_add_output_operand (gcc_jit_extended_asm *ext_asm,
				       uint32_t *asn_s, size_t asn_len,
				       size_t asn_stride,
				       uint32_t *c_s, size_t c_len,
				       size_t c_stride,
				       gcc_jit_lvalue *dest)
{
  char *asm_symbolic_name  = ucs4_to_utf8 (asn_s, asn_len, asn_stride);
  char *constraint = ucs4_to_utf8 (c_s, c_len, c_stride);
  gcc_jit_extended_asm_add_output_operand (ext_asm, asm_symbolic_name,
					   constraint, dest);
  free (asm_symbolic_name);
  free (constraint);
}

void
jit68_extended_asm_add_input_operand (gcc_jit_extended_asm *ext_asm,
				      uint32_t *asn_s, size_t asn_len,
				      size_t asn_stride,
				      uint32_t *c_s, size_t c_len,
				      size_t c_stride,
				      gcc_jit_rvalue *src)
{
  char *asm_symbolic_name  = ucs4_to_utf8 (asn_s, asn_len, asn_stride);
  char *constraint = ucs4_to_utf8 (c_s, c_len, c_stride);
  gcc_jit_extended_asm_add_input_operand (ext_asm, asm_symbolic_name,
					  constraint, src);
  free (asm_symbolic_name);
  free (constraint);
}

void
jit68_extended_asm_add_clobber (gcc_jit_extended_asm *ext_asm,
				uint32_t *s, size_t len, size_t stride)
{
  char *victim = ucs4_to_utf8 (s, len, stride);
  gcc_jit_extended_asm_add_clobber (ext_asm, victim);
  free (victim);
}

void
jit68_context_add_top_level_asm (gcc_jit_context *ctxt,
				 gcc_jit_location *loc,
				 uint32_t *s, size_t len, size_t stride)
{
  char *asm_stmts = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_add_top_level_asm (ctxt, loc, asm_stmts);
  free (asm_stmts);
}

void
jit68_function_add_string_attribute (gcc_jit_function *func,
				     enum gcc_jit_fn_attribute attribute,
    				     uint32_t *s, size_t len, size_t stride)
{
  char *value = ucs4_to_utf8 (s, len, stride);
  gcc_jit_function_add_string_attribute (func, attribute, value);
  free (value);
}

int
jit68_target_info_cpu_supports (gcc_jit_target_info *info,
				uint32_t *s, size_t len, size_t stride)
{
  char *feature = ucs4_to_utf8 (s, len, stride);
  int result = gcc_jit_target_info_cpu_supports (info, feature);
  free (feature);
  return result;
}

void
jit68_lvalue_add_string_attribute (gcc_jit_lvalue *variable,
				   enum gcc_jit_variable_attribute attribute,
				   uint32_t *s, size_t len, size_t stride)
{
  char *value = ucs4_to_utf8 (s, len, stride);
  gcc_jit_lvalue_add_string_attribute (variable, attribute, value);
  free (value);
}

void
jit68_context_set_output_ident (gcc_jit_context *ctxt,
				uint32_t *s, size_t len, size_t stride)
{
  char *output_ident = ucs4_to_utf8 (s, len, stride);
  gcc_jit_context_set_output_ident (ctxt, output_ident);
  free (output_ident);
}
