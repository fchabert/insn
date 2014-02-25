/**
 * Copyright (c) 2014, Floris Chabert. All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "decoder.h"
#include "isa.h"

#include <functional>

namespace insn {
namespace arm64 {

namespace opcode {

#pragma mark opcode - top

bool is_data_proc_imm(uint32_t opcode) {
	return (opcode & 0b00011100000000000000000000000000)
	              == 0b00010000000000000000000000000000;
}
bool is_branch_sys(uint32_t opcode) {
	return (opcode & 0b00011100000000000000000000000000)
	              == 0b00010100000000000000000000000000;
}
bool is_load_store(uint32_t opcode) {
	return (opcode & 0b00001010000000000000000000000000)
	              == 0b00001000000000000000000000000000;
}
bool is_data_proc_reg(uint32_t opcode) {
	return (opcode & 0b00001110000000000000000000000000)
	              == 0b00001010000000000000000000000000;
}
bool is_data_proc_neon(uint32_t opcode) {
	return (opcode & 0b00001110000000000000000000000000)
	              == 0b00001110000000000000000000000000;
}

#pragma mark opcode - data processing (immediate)

bool is_rel_addressing(uint32_t opcode) {
	return (opcode & 0b00011111000000000000000000000000)
	              == 0b00010000000000000000000000000000;
}
bool is_add_sub_imm(uint32_t opcode) {
	return (opcode & 0b00011111000000000000000000000000)
	              == 0b00010000100000000000000000000000;
}
bool is_logical_imm(uint32_t opcode) {
	return (opcode & 0b00011111100000000000000000000000)
	              == 0b00010010000000000000000000000000;
}
bool is_move_wide(uint32_t opcode) {
	return (opcode & 0b00011111100000000000000000000000)
	              == 0b00010010100000000000000000000000;
}
bool is_bitfield(uint32_t opcode) {
	return (opcode & 0b00011111100000000000000000000000)
	              == 0b00010011000000000000000000000000;
}
bool is_extract(uint32_t opcode) {
	return (opcode & 0b00011111100000000000000000000000)
	              == 0b00010011100000000000000000000000;
}

#pragma mark opcode - branches, exceptions, system

bool is_unconditional_branch_imm(uint32_t opcode) {
	return (opcode & 0b01111100000000000000000000000000)
	              == 0b00010100000000000000000000000000;
}
bool is_compare_and_branch(uint32_t opcode) {
	return (opcode & 0b01111110000000000000000000000000)
	              == 0b00110100000000000000000000000000;
}
bool is_test_and_branch(uint32_t opcode) {
	return (opcode & 0b01111110000000000000000000000000)
	              == 0b00110110000000000000000000000000;
}
bool is_conditional_branch(uint32_t opcode) {
	return (opcode & 0b11111110000000000000000000000000)
	              == 0b01010100000000000000000000000000;
}
bool is_exception(uint32_t opcode) {
	return (opcode & 0b01111111000000000000000000000000)
	              == 0b11010100000000000000000000000000;
}
bool is_system(uint32_t opcode) {
	return (opcode & 0b01111111110000000000000000000000)
	              == 0b11010101000000000000000000000000;
}
bool is_unconditional_branch_reg(uint32_t opcode) {
	return (opcode & 0b01111110000000000000000000000000)
	              == 0b11010110000000000000000000000000;
}

#pragma mark opcode - load, store

bool is_load_store_exclusive(uint32_t opcode) {
	return (opcode & 0b00111111000000000000000000000000)
	              == 0b00001000000000000000000000000000;
}
bool is_load_reg(uint32_t opcode) {
	return (opcode & 0b00111011000000000000000000000000)
	              == 0b00011000000000000000000000000000;
}
bool is_load_store_noallloc_pair(uint32_t opcode) {
	return (opcode & 0b00111011100000000000000000000000)
	              == 0b00101000000000000000000000000000;
}
bool is_load_store_pair_post_idx(uint32_t opcode) {
	return (opcode & 0b00111011100000000000000000000000)
	              == 0b00101000100000000000000000000000;
}
bool is_load_store_pair_offset(uint32_t opcode) {
	return (opcode & 0b00111011100000000000000000000000)
	              == 0b00101001000000000000000000000000;
}
bool is_load_store_pair_pre_idx(uint32_t opcode) {
	return (opcode & 0b00111011100000000000000000000000)
	              == 0b00101001100000000000000000000000;
}
bool is_load_store_reg_imm(uint32_t opcode) {
	return (opcode & 0b00111011001000000000110000000000)
	              == 0b00111000000000000000000000000000;
}
bool is_load_store_reg_post_idx(uint32_t opcode) {
	return (opcode & 0b00111011001000000000110000000000)
	              == 0b00111000000000000000010000000000;
}
bool is_load_store_reg_unpriv(uint32_t opcode) {
	return (opcode & 0b00111011001000000000110000000000)
	              == 0b00111000000000000000100000000000;
}
bool is_load_store_reg_pre_idx(uint32_t opcode) {
	return (opcode & 0b00111011001000000000110000000000)
	              == 0b00111000000000000000110000000000;
}
bool is_load_store_reg_offset(uint32_t opcode) {
	return (opcode & 0b00111011001000000000110000000000)
	              == 0b00111000001000000000100000000000;
}
bool is_load_store_reg_uimm(uint32_t opcode) {
	return (opcode & 0b00111011000000000000000000000000)
	              == 0b00111001000000000000000000000000;
}
bool is_load_store_simd_multiple(uint32_t opcode) {
	return (opcode & 0b10111111101111110000000000000000)
	              == 0b00001100000000000000000000000000;
}
bool is_load_store_simd_multiple_post(uint32_t opcode) {
	return (opcode & 0b10111111101000000000000000000000)
	              == 0b00001100100000000000000000000000;
}
bool is_load_store_simd_single(uint32_t opcode) {
	return (opcode & 0b10111111100111110000000000000000)
	              == 0b00001101000000000000000000000000;
}
bool is_load_store_simd_single_post(uint32_t opcode) {
	return (opcode & 0b10111111100000000000000000000000)
	              == 0b00001101100000000000000000000000;
}

#pragma mark opcode - data processing (register)

bool is_logical_shift(uint32_t opcode) {
	return (opcode & 0b00011111000000000000000000000000)
	              == 0b00001010000000000000000000000000;
}
bool is_add_sub_shift(uint32_t opcode) {
	return (opcode & 0b00011111001000000000000000000000)
	              == 0b00001011000000000000000000000000;
}
bool is_add_sub_ext(uint32_t opcode) {
	return (opcode & 0b00011111001000000000000000000000)
	              == 0b00001011001000000000000000000000;
}
bool is_add_sub_carry(uint32_t opcode) {
	return (opcode & 0b00011111111000000000000000000000)
	              == 0b00011010000000000000000000000000;
}
bool is_cond_comp_reg(uint32_t opcode) {
	return (opcode & 0b00011111111000000000100000000000)
	              == 0b00011010010000000000000000000000;
}
bool is_cond_comp_imm(uint32_t opcode) {
	return (opcode & 0b00011111111000000000100000000000)
	              == 0b00011010010000000000100000000000;
}
bool is_cond_sel(uint32_t opcode) {
	return (opcode & 0b00011111111000000000000000000000)
	              == 0b00011010100000000000000000000000;
}
bool is_data_proc_3(uint32_t opcode) {
	return (opcode & 0b00011111000000000000000000000000)
	              == 0b00011011000000000000000000000000;
}
bool is_data_proc_2(uint32_t opcode) {
	return (opcode & 0b01011111111000000000000000000000)
	              == 0b00011010110000000000000000000000;
}
bool is_data_proc_1(uint32_t opcode) {
	return (opcode & 0b01011111111000000000000000000000)
	              == 0b01011010110000000000000000000000;
}

#pragma mark opcode - data processing (simd, vfp) instructions


#pragma mark opcode helpers

bool bit(uint32_t opcode, int index) {
	return (opcode & (1 << index)) != 0;
}

int val(uint32_t opcode, int index_end, int index_start) {
	int v = opcode >> index_start;
	if (index_end - index_start < 31) {
		v &= ((1 << (index_end-index_start+1)) - 1);
	}
	return v;
}

}

#pragma mark decode - data processing (immediate)

std::function<void(isa*)> decode_rel_addressing(uint32_t opcode) {
	int op = opcode::val(opcode, 31, 31);
	int rd = opcode::val(opcode, 4, 0);
	int immh = opcode::val(opcode, 23, 5);
	int imml = opcode::val(opcode, 30, 29);
	int imm = (immh << 2) | imml;

	switch (op) {
		case 0b0:
			return std::bind(&isa::_adr, std::placeholders::_1, rd, imm);
		case 0b1:
			return std::bind(&isa::_adrp, std::placeholders::_1, rd, imm);
	}

	throw invalid();
}

std::function<void(isa*)> decode_add_sub_imm(uint32_t opcode) {
	bool is_64 = opcode::bit(opcode, 31);
	int op = opcode::val(opcode, 30, 30);
	bool set_flags = opcode::bit(opcode, 29);
	int rd = opcode::val(opcode, 4, 0);
	int rn = opcode::val(opcode, 9, 5);
	int imm = opcode::val(opcode, 21, 10);
	int shift = opcode::val(opcode, 23, 22);

	switch (op) {
		case 0b0:
			return std::bind(&isa::_add, std::placeholders::_1, rd, rn, imm, shift, is_64, set_flags);
		case 0b1:
			return std::bind(&isa::_sub, std::placeholders::_1, rd, rn, imm, shift, is_64, set_flags);
	}

	throw invalid();
}

std::function<void(isa*)> decode_logical_imm(uint32_t opcode) {
	bool is_64 = opcode::bit(opcode, 31);
	int op = opcode::val(opcode, 30, 29);
	int rd = opcode::val(opcode, 4, 0);
	int rn = opcode::val(opcode, 9, 5);
	int imms = opcode::val(opcode, 15, 10);
	int immr = opcode::val(opcode, 21, 16);

	switch (op) {
		case 0b00:
			return std::bind(&isa::_and, std::placeholders::_1, rd, rn, imms, immr, is_64);
		case 0b01:
			return std::bind(&isa::_orr, std::placeholders::_1, rd, rn, imms, immr, is_64);
		case 0b10:
			return std::bind(&isa::_eor, std::placeholders::_1, rd, rn, imms, immr, is_64);
		case 0b11:
			return std::bind(&isa::_ands, std::placeholders::_1, rd, rn, imms, immr, is_64);
	}

	throw invalid();
}

std::function<void(isa*)> decode_move_wide(uint32_t opcode) {
	bool is_64 = opcode::bit(opcode, 31);
	int op = opcode::val(opcode, 30, 29);
	int rd = opcode::val(opcode, 4, 0);
	int imm = opcode::val(opcode, 21, 5);

	switch (op) {
		case 0b00:
			return std::bind(&isa::_movn, std::placeholders::_1, rd, imm, is_64);
		case 0b10:
			return std::bind(&isa::_movz, std::placeholders::_1, rd, imm, is_64);
		case 0b11:
			return std::bind(&isa::_movk, std::placeholders::_1, rd, imm, is_64);
	}

	throw invalid();
}

std::function<void(isa*)> decode_bitfield(uint32_t opcode) {
	bool is_64 = opcode::bit(opcode, 31);
	int op = opcode::val(opcode, 30, 29);
	int rd = opcode::val(opcode, 4, 0);
	int rn = opcode::val(opcode, 9, 5);
	int imms = opcode::val(opcode, 15, 10);
	int immr = opcode::val(opcode, 21, 16);

	switch (op) {
		case 0b00:
			return std::bind(&isa::_sbfm, std::placeholders::_1, rd, rn, imms, immr, is_64);
		case 0b01:
			return std::bind(&isa::_bfm, std::placeholders::_1, rd, rn, imms, immr, is_64);
		case 0b10:
			return std::bind(&isa::_ubfm, std::placeholders::_1, rd, rn, imms, immr, is_64);
	}

	throw invalid();
}

std::function<void(isa*)> decode_extract(uint32_t opcode) {
	bool is_64 = opcode::bit(opcode, 31);
	int op = opcode::val(opcode, 30, 29);
	int rd = opcode::val(opcode, 4, 0);
	int rn = opcode::val(opcode, 9, 5);
	int imms = opcode::val(opcode, 15, 10);
	int rm = opcode::val(opcode, 20, 16);

	switch (op) {
		case 0b00:
			return std::bind(&isa::_ext, std::placeholders::_1, rd, rn, rm, imms, is_64);
	}

	throw invalid();
}

std::function<void(isa*)> decode_data_proc_imm(uint32_t opcode) {
	if (opcode::is_rel_addressing(opcode)) {
		return decode_rel_addressing(opcode);
	}
	if (opcode::is_add_sub_imm(opcode)) {
		return decode_add_sub_imm(opcode);
	}
	if (opcode::is_logical_imm(opcode)) {
		return decode_logical_imm(opcode);
	}
	if (opcode::is_move_wide(opcode)) {
		return decode_move_wide(opcode);
	}
	if (opcode::is_bitfield(opcode)) {
		return decode_bitfield(opcode);
	}
	if (opcode::is_extract(opcode)) {
		return decode_extract(opcode);
	}

	throw invalid();
}

#pragma mark decode - branch, system

std::function<void(isa*)> decode_unconditional_branch_imm(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_compare_and_branch(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_test_and_branch(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_conditional_branch(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_exception(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_system(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_unconditional_branch_reg(uint32_t opcode) {
	throw unsupported();
}

std::function<void(isa*)> decode_branch_sys(uint32_t opcode) {
	if (opcode::is_unconditional_branch_imm(opcode)) {
		return decode_unconditional_branch_imm(opcode);
	}
	if (opcode::is_compare_and_branch(opcode)) {
		return decode_compare_and_branch(opcode);
	}
	if (opcode::is_test_and_branch(opcode)) {
		return decode_test_and_branch(opcode);
	}
	if (opcode::is_conditional_branch(opcode)) {
		return decode_conditional_branch(opcode);
	}
	if (opcode::is_exception(opcode)) {
		return decode_exception(opcode);
	}
	if (opcode::is_system(opcode)) {
		return decode_system(opcode);
	}
	if (opcode::is_unconditional_branch_reg(opcode)) {
		return decode_unconditional_branch_reg(opcode);
	}

	throw invalid();
}

#pragma mark decode - load, store

std::function<void(isa*)> decode_load_store(uint32_t opcode) {
	throw unsupported();
}

#pragma mark decode - data processing (register)

std::function<void(isa*)> decode_logical_shift(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_add_sub_shift(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_add_sub_ext(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_add_sub_carry(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_cond_comp_reg(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_cond_comp_imm(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_cond_sel(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_data_proc_3(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_data_proc_2(uint32_t opcode) {
	throw unsupported();
}
std::function<void(isa*)> decode_data_proc_1(uint32_t opcode) {
	throw unsupported();
}

std::function<void(isa*)> decode_data_proc_reg(uint32_t opcode) {
	if (opcode::is_logical_shift(opcode)) {
		return decode_logical_shift(opcode);
	}
	if (opcode::is_add_sub_shift(opcode)) {
		return decode_add_sub_shift(opcode);
	}
	if (opcode::is_add_sub_ext(opcode)) {
		return decode_add_sub_ext(opcode);
	}
	if (opcode::is_add_sub_carry(opcode)) {
		return decode_add_sub_carry(opcode);
	}
	if (opcode::is_cond_comp_reg(opcode)) {
		return decode_cond_comp_reg(opcode);
	}
	if (opcode::is_cond_comp_imm(opcode)) {
		return decode_cond_comp_imm(opcode);
	}
	if (opcode::is_cond_sel(opcode)) {
		return decode_cond_sel(opcode);
	}
	if (opcode::is_data_proc_3(opcode)) {
		return decode_data_proc_3(opcode);
	}
	if (opcode::is_data_proc_2(opcode)) {
		return decode_data_proc_2(opcode);
	}
	if (opcode::is_data_proc_1(opcode)) {
		return decode_data_proc_1(opcode);
	}

	throw invalid();
}

#pragma mark decode - data processing (simd, vfp)

std::function<void(isa*)> decode_data_proc_neon(uint32_t opcode) {
	throw unsupported();
}

#pragma mark decoder

uint32_t decoder::fetch() {
	uint32_t opcode;

	opcode = *reinterpret_cast<uint32_t*>(code_current);
	code_current += 4;

	return opcode;
}

std::function<void(isa*)> decoder::decode(uint32_t opcode) {
	if (opcode::is_data_proc_imm(opcode)) {
		return decode_data_proc_imm(opcode);
	}
	if (opcode::is_branch_sys(opcode)) {
		return decode_branch_sys(opcode);
	}
	if (opcode::is_load_store(opcode)) {
		return decode_load_store(opcode);
	}
	if (opcode::is_data_proc_reg(opcode)) {
		return decode_data_proc_reg(opcode);
	}
	if (opcode::is_data_proc_neon(opcode)) {
		return decode_data_proc_neon(opcode);
	}

	throw invalid();
}

void decoder::next() {
	uint32_t opcode = fetch();
	std::function<void(isa*)> exec = decode(opcode);
	exec((isa *)&_printer);
}

}
}
