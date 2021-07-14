// evmone: Fast Ethereum Virtual Machine implementation
// Copyright 2021 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "instructions.hpp"

namespace evmone::op2fn
{
inline constexpr auto OP_STOP = stop;
inline constexpr auto OP_ADD = add;
inline constexpr auto OP_MUL = mul;
inline constexpr auto OP_SUB = sub;
inline constexpr auto OP_DIV = div;
inline constexpr auto OP_SDIV = sdiv;
inline constexpr auto OP_MOD = mod;
inline constexpr auto OP_SMOD = smod;
inline constexpr auto OP_ADDMOD = addmod;
inline constexpr auto OP_MULMOD = mulmod;
inline constexpr auto OP_EXP = exp;
inline constexpr auto OP_SIGNEXTEND = signextend;
inline constexpr auto OP_LT = lt;
inline constexpr auto OP_GT = gt;
inline constexpr auto OP_SLT = slt;
inline constexpr auto OP_SGT = sgt;
inline constexpr auto OP_EQ = eq;
inline constexpr auto OP_ISZERO = iszero;
inline constexpr auto OP_AND = and_;
inline constexpr auto OP_OR = or_;
inline constexpr auto OP_XOR = xor_;
inline constexpr auto OP_NOT = not_;
inline constexpr auto OP_BYTE = byte;
inline constexpr auto OP_SHL = shl;
inline constexpr auto OP_SHR = shr;
inline constexpr auto OP_SAR = sar;
inline constexpr auto OP_KECCAK256 = keccak256;
inline constexpr auto OP_ADDRESS = address;
inline constexpr auto OP_BALANCE = balance;
inline constexpr auto OP_ORIGIN = origin;
inline constexpr auto OP_CALLER = caller;
inline constexpr auto OP_CALLVALUE = callvalue;
inline constexpr auto OP_CALLDATALOAD = calldataload;
inline constexpr auto OP_CALLDATASIZE = calldatasize;
inline constexpr auto OP_CALLDATACOPY = calldatacopy;
inline constexpr auto OP_CODESIZE = codesize;
inline constexpr auto OP_CODECOPY = codecopy;
inline constexpr auto OP_GASPRICE = gasprice;
inline constexpr auto OP_EXTCODESIZE = extcodesize;
inline constexpr auto OP_EXTCODECOPY = extcodecopy;
inline constexpr auto OP_RETURNDATASIZE = returndatasize;
inline constexpr auto OP_RETURNDATACOPY = returndatacopy;
inline constexpr auto OP_EXTCODEHASH = extcodehash;
inline constexpr auto OP_BLOCKHASH = blockhash;
inline constexpr auto OP_COINBASE = coinbase;
inline constexpr auto OP_TIMESTAMP = timestamp;
inline constexpr auto OP_NUMBER = number;
inline constexpr auto OP_DIFFICULTY = difficulty;
inline constexpr auto OP_GASLIMIT = gaslimit;
inline constexpr auto OP_CHAINID = chainid;
inline constexpr auto OP_SELFBALANCE = selfbalance;
inline constexpr auto OP_BASEFEE = basefee;
inline constexpr auto OP_POP = pop;
inline constexpr auto OP_MLOAD = mload;
inline constexpr auto OP_MSTORE = mstore;
inline constexpr auto OP_MSTORE8 = mstore8;
inline constexpr auto OP_SLOAD = sload;
inline constexpr auto OP_SSTORE = sstore;
inline constexpr auto OP_JUMP = jump;
inline constexpr auto OP_JUMPI = jumpi;
inline constexpr auto OP_PC = pc;
inline constexpr auto OP_MSIZE = msize;
inline constexpr auto OP_GAS = gas;
inline constexpr auto OP_JUMPDEST = jumpdest;
inline constexpr auto OP_PUSH1 = push<1>;
inline constexpr auto OP_PUSH2 = push<2>;
inline constexpr auto OP_PUSH3 = push<3>;
inline constexpr auto OP_PUSH4 = push<4>;
inline constexpr auto OP_PUSH5 = push<5>;
inline constexpr auto OP_PUSH6 = push<6>;
inline constexpr auto OP_PUSH7 = push<7>;
inline constexpr auto OP_PUSH8 = push<8>;
inline constexpr auto OP_PUSH9 = push<9>;
inline constexpr auto OP_PUSH10 = push<10>;
inline constexpr auto OP_PUSH11 = push<11>;
inline constexpr auto OP_PUSH12 = push<12>;
inline constexpr auto OP_PUSH13 = push<13>;
inline constexpr auto OP_PUSH14 = push<14>;
inline constexpr auto OP_PUSH15 = push<15>;
inline constexpr auto OP_PUSH16 = push<16>;
inline constexpr auto OP_PUSH17 = push<17>;
inline constexpr auto OP_PUSH18 = push<18>;
inline constexpr auto OP_PUSH19 = push<19>;
inline constexpr auto OP_PUSH20 = push<20>;
inline constexpr auto OP_PUSH21 = push<21>;
inline constexpr auto OP_PUSH22 = push<22>;
inline constexpr auto OP_PUSH23 = push<23>;
inline constexpr auto OP_PUSH24 = push<24>;
inline constexpr auto OP_PUSH25 = push<25>;
inline constexpr auto OP_PUSH26 = push<26>;
inline constexpr auto OP_PUSH27 = push<27>;
inline constexpr auto OP_PUSH28 = push<28>;
inline constexpr auto OP_PUSH29 = push<29>;
inline constexpr auto OP_PUSH30 = push<30>;
inline constexpr auto OP_PUSH31 = push<31>;
inline constexpr auto OP_PUSH32 = push<32>;
inline constexpr auto OP_DUP1 = dup<1>;
inline constexpr auto OP_DUP2 = dup<2>;
inline constexpr auto OP_DUP3 = dup<3>;
inline constexpr auto OP_DUP4 = dup<4>;
inline constexpr auto OP_DUP5 = dup<5>;
inline constexpr auto OP_DUP6 = dup<6>;
inline constexpr auto OP_DUP7 = dup<7>;
inline constexpr auto OP_DUP8 = dup<8>;
inline constexpr auto OP_DUP9 = dup<9>;
inline constexpr auto OP_DUP10 = dup<10>;
inline constexpr auto OP_DUP11 = dup<11>;
inline constexpr auto OP_DUP12 = dup<12>;
inline constexpr auto OP_DUP13 = dup<13>;
inline constexpr auto OP_DUP14 = dup<14>;
inline constexpr auto OP_DUP15 = dup<15>;
inline constexpr auto OP_DUP16 = dup<16>;
inline constexpr auto OP_SWAP1 = swap<1>;
inline constexpr auto OP_SWAP2 = swap<2>;
inline constexpr auto OP_SWAP3 = swap<3>;
inline constexpr auto OP_SWAP4 = swap<4>;
inline constexpr auto OP_SWAP5 = swap<5>;
inline constexpr auto OP_SWAP6 = swap<6>;
inline constexpr auto OP_SWAP7 = swap<7>;
inline constexpr auto OP_SWAP8 = swap<8>;
inline constexpr auto OP_SWAP9 = swap<9>;
inline constexpr auto OP_SWAP10 = swap<10>;
inline constexpr auto OP_SWAP11 = swap<11>;
inline constexpr auto OP_SWAP12 = swap<12>;
inline constexpr auto OP_SWAP13 = swap<13>;
inline constexpr auto OP_SWAP14 = swap<14>;
inline constexpr auto OP_SWAP15 = swap<15>;
inline constexpr auto OP_SWAP16 = swap<16>;
inline constexpr auto OP_LOG0 = log<0>;
inline constexpr auto OP_LOG1 = log<1>;
inline constexpr auto OP_LOG2 = log<2>;
inline constexpr auto OP_LOG3 = log<3>;
inline constexpr auto OP_LOG4 = log<4>;
inline constexpr auto OP_CREATE = create<EVMC_CREATE>;
inline constexpr auto OP_CALL = call<EVMC_CALL>;
inline constexpr auto OP_CALLCODE = call<EVMC_CALLCODE>;
inline constexpr auto OP_RETURN = return_<EVMC_SUCCESS>;
inline constexpr auto OP_DELEGATECALL = call<EVMC_DELEGATECALL>;
inline constexpr auto OP_CREATE2 = create<EVMC_CREATE2>;
inline constexpr auto OP_STATICCALL = call<EVMC_CALL, true>;
inline constexpr auto OP_INVALID = invalid;
inline constexpr auto OP_REVERT = return_<EVMC_REVERT>;
inline constexpr auto OP_SELFDESTRUCT = selfdestruct;
}  // namespace evmone::op2fn
