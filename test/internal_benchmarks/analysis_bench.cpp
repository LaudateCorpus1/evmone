// evmone: Fast Ethereum Virtual Machine implementation
// Copyright 2020 The evmone Authors.
// SPDX-License-Identifier: Apache-2.0

#include "evmone/baseline.hpp"
#include "test/experimental/jumpdest_analysis.hpp"
#include "test/utils/utils.hpp"
#include <benchmark/benchmark.h>
#include <numeric>

#include <iostream>


namespace
{
const auto test_bytecode = from_hex(
    "608060405234801561001057600080fd5b50600436106100365760003560e01c80631e0924231461003b578063d299"
    "dac0146102bb575b600080fd5b610282600480360360a081101561005157600080fd5b810190602081018135640100"
    "00000081111561006c57600080fd5b82018360208201111561007e57600080fd5b8035906020019184600183028401"
    "11640100000000831117156100a057600080fd5b91908080601f016020809104026020016040519081016040528093"
    "9291908181526020018383808284376000920191909152509295949360208101935035915050640100000000811115"
    "6100f357600080fd5b82018360208201111561010557600080fd5b8035906020019184600183028401116401000000"
    "008311171561012757600080fd5b91908080601f016020809104026020016040519081016040528093929190818152"
    "602001838380828437600092019190915250929594936020810193503591505064010000000081111561017a576000"
    "80fd5b82018360208201111561018c57600080fd5b8035906020019184600183028401116401000000008311171561"
    "01ae57600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380"
    "828437600092019190915250929594936020810193503591505064010000000081111561020157600080fd5b820183"
    "60208201111561021357600080fd5b8035906020019184600183028401116401000000008311171561023557600080"
    "fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092"
    "0191909152509295505050903567ffffffffffffffff1691506103f49050565b604051808261010080838360005b83"
    "8110156102a8578181015183820152602001610290565b5050505090500191505060405180910390f35b6102826004"
    "80360360608110156102d157600080fd5b8101906020810181356401000000008111156102ec57600080fd5b820183"
    "6020820111156102fe57600080fd5b8035906020019184600183028401116401000000008311171561032057600080"
    "fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600092"
    "019190915250929594936020810193503591505064010000000081111561037357600080fd5b820183602082011115"
    "61038557600080fd5b803590602001918460018302840111640100000000831117156103a757600080fd5b91908080"
    "601f016020809104026020016040519081016040528093929190818152602001838380828437600092019190915250"
    "9295505050903567ffffffffffffffff1691506104489050565b6103fc61156d565b61040461158d565b61040c6115"
    "6d565b61042982858961041b8a610485565b6104248a610485565b610538565b61043382896106dc565b61043d8282"
    "61079b565b979650505050505050565b61045061156d565b61047d8484602060405190810160405280600081525060"
    "20604051908101604052806000815250866103f4565b949350505050565b61048d6115ca565b60005b825181101561"
    "04fb5760088184018101519067ffffffffffffffff82166007841660010182026040031b9084908404600281106104"
    "c957fe5b6020020151188360088404600281106104de57fe5b67ffffffffffffffff90921660209290920201525060"
    "0101610490565b5061050d8160005b60200201516108cd565b67ffffffffffffffff16815261052481600161050356"
    "5b67ffffffffffffffff166020820152919050565b67ffffffffffffffff84161580610559575060408467ffffffff"
    "ffffffff16115b80610565575060408351115b1561056f57600080fd5b61057761156d565b50604080516101008101"
    "8252676a09e667f3bcc908815267bb67ae8584caa73b6020820152673c6ef372fe94f82b9181019190915267a54ff5"
    "3a5f1d36f1606082015267510e527fade682d16080820152679b05688c2b3e6c1f60a0820152671f83d9abfb41bd6b"
    "60c0820152675be0cd19137e217960e082015260005b600881101561063d5781816008811061060a57fe5b60200201"
    "5187602001518260088110151561062157fe5b67ffffffffffffffff90921660209290920201526001016105f5565b"
    "50835160208781018051805167ffffffffffffffff94851660081b1889186301010000188416905285518151608090"
    "81018051909218851690915286830151825160a0018051909118851690528551825160c00180519091188516905291"
    "850151905160e00180519091188316905286821690880152845190600090821611156106d3576106cb87866106dc56"
    "5b608060608801525b50505050505050565b60005b815181101561079657826060015167ffffffffffffffff166080"
    "141561074057606083015160408401805167ffffffffffffffff9092169091016fffffffffffffffffffffffffffff"
    "ffff16905261073883600061094f565b600060608401525b60608301805167ffffffffffffffff6001820181169092"
    "52166107616115e5565b508351835160009085908590811061077557fe5b90602001015160f81c60f81b60f81c9050"
    "80838301535050506001016106df565b505050565b60608201805160408401805167ffffffffffffffff8084169182"
    "016fffffffffffffffffffffffffffffffff1690925260019092011690915260006107de6115e5565b508351825b60"
    "808110156107f95782818301536001016107e3565b5061080585600161094f565b60005b6080860151600890048110"
    "1561085457602086015161082c90826008811061050357fe5b85826008811061083857fe5b67ffffffffffffffff90"
    "92166020929092020152600101610808565b506040856080015110156108c657608085015160208601516008600783"
    "16810260400392610889929190046008811061050357fe5b67ffffffffffffffff16901c8460088760800151811515"
    "6108a657fe5b04600881106108b157fe5b67ffffffffffffffff90921660209290920201525b5050505050565b6000"
    "67010000000000000060ff8316026501000000000061ff00841602630100000062ff000085160261010063ff000000"
    "861681029064ff00000000871604630100000065ff00000000008816046501000000000066ff000000000000891604"
    "67010000000000000067ff000000000000008a16041818181818181892915050565b610957611604565b61095f6116"
    "04565b61096761156d565b506040805161010081018252676a09e667f3bcc908815267bb67ae8584caa73b60208201"
    "52673c6ef372fe94f82b9181019190915267a54ff53a5f1d36f1606082015267510e527fade682d16080820152679b"
    "05688c2b3e6c1f60a0820152671f83d9abfb41bd6b60c0820152675be0cd19137e217960e082015260005b60088110"
    "15610a5f57602086015181600881106109fe57fe5b6020020151848260108110610a0f57fe5b67ffffffffffffffff"
    "9092166020929092020152818160088110610a2f57fe5b6020020151846008830160108110610a4357fe5b67ffffff"
    "ffffffffff90921660209290920201526001016109e5565b506040850180516101808501805167ffffffffffffffff"
    "928316188216905290516101a085018051680100000000000000006fffffffffffffffffffffffffffffffff909316"
    "9290920490911890911690528315610acc576101c0830180511967ffffffffffffffff1690525b600080805b601081"
    "60ff161015610b56576000925060038116801515610b0c578851600460ff84160460ff16600481101515610b0457fe"
    "5b602002015192505b67ffffffffffffffff83826003036040021c169350610b2a846108cd565b8660ff8416601081"
    "10610b3957fe5b67ffffffffffffffff909216602092909202015250600101610ad1565b50610b7985600060046008"
    "600c89845b60200201518a60015b60200201516113e8565b610b9685600160056009600d8960025b60200201518a60"
    "03610b6f565b610bb38560026006600a600e8960045b60200201518a6005610b6f565b610bd08560036007600b600f"
    "8960065b60200201518a6007610b6f565b610bed8560006005600a600f8960085b60200201518a6009610b6f565b61"
    "0c0a8560016006600b600c89600a5b60200201518a600b610b6f565b610c2785600260076008600d89600c5b602002"
    "01518a600d610b6f565b610c4385600360046009600e89815b60200201518a600f610b6f565b610c60856000600460"
    "08600c89600e5b60200201518a600a610b6f565b610c7d85600160056009600d8960045b60200201518a6008610b6f"
    "565b610c918560026006600a600e896009610c36565b610cae8560036007600b600f89600d5b60200201518a600661"
    "0b6f565b610ccb8560006005600a600f8960015b60200201518a600c610b6f565b610ce88560016006600b600c8960"
    "005b60200201518a6002610b6f565b610cfc85600260076008600d89600b610bc3565b610d1085600360046009600e"
    "896005610b89565b610d2485600060046008600c89600b610c70565b610d4185600160056009600d89600c5b602002"
    "01518a6000610b6f565b610d558560026006600a600e896005610cdb565b610d688560036007600b600f8981610c1a"
    "565b610d848560006005600a600f89825b60200201518a600e610b6f565b610d988560016006600b600c896003610c"
    "a1565b610dab85600260076008600d8983610b66565b610dc785600360046009600e89825b60200201518a6004610b"
    "6f565b610ddb85600060046008600c896007610be0565b610def85600160056009600d896003610b66565b610e0385"
    "60026006600a600e89600d610cbe565b610e168560036007600b600f8982610d77565b610e2a8560006005600a600f"
    "896002610ca1565b610e3e8560016006600b600c896005610c53565b610e5285600260076008600d896004610d3456"
    "5b610e6685600360046009600e89600f610c70565b610e7a85600060046008600c896009610d34565b610e8d856001"
    "60056009600d8983610bc3565b610ea08560026006600a600e8984610dba565b610eb48560036007600b600f89600a"
    "610c36565b610ec88560006005600a600f89600e610b66565b610edb8560016006600b600c8982610cbe565b610eef"
    "85600260076008600d896006610c70565b610f0285600360046009600e8984610c1a565b610f168560006004600860"
    "0c896002610cbe565b610f2a85600160056009600d896006610c53565b610f3e8560026006600a600e896000610bfd"
    "565b610f528560036007600b600f896008610b89565b610f668560006005600a600f896004610c1a565b610f7a8560"
    "016006600b600c896007610ba6565b610f8e85600260076008600d89600f610d77565b610fa285600360046009600e"
    "896001610be0565b610fb585600060046008600c8981610ba6565b610fc885600160056009600d8984610c36565b61"
    "0fdb8560026006600a600e8981610c1a565b610fef8560036007600b600f896004610c53565b611002856000600560"
    "0a600f8984610bc3565b6110158560016006600b600c8983610b89565b61102985600260076008600d896009610cdb"
    "565b61103d85600360046009600e896008610bfd565b61105185600060046008600c89600d610bfd565b6110658560"
    "0160056009600d896007610d77565b6110798560026006600a600e89600c610b66565b61108c8560036007600b600f"
    "8984610be0565b61109f8560006005600a600f8983610d34565b6110b38560016006600b600c89600f610dba565b61"
    "10c685600260076008600d8982610ca1565b6110da85600360046009600e896002610c53565b6110ee856000600460"
    "08600c896006610c36565b61110285600160056009600d89600e610be0565b6111168560026006600a600e89600b61"
    "0b89565b61112a8560036007600b600f896000610c70565b61113e8560006005600a600f89600c610cdb565b611152"
    "8560016006600b600c89600d610bc3565b61116685600260076008600d896001610dba565b61117a85600360046009"
    "600e89600a610ba6565b61118e85600060046008600c89600a610cdb565b6111a285600160056009600d896008610d"
    "ba565b6111b68560026006600a600e896007610ca1565b6111ca8560036007600b600f896001610ba6565b6111dd85"
    "60006005600a600f8981610bfd565b6111f18560016006600b600c896009610d77565b61120585600260076008600d"
    "896003610cbe565b61121985600360046009600e89600d610d34565b61122c85600060046008600c8984610b66565b"
    "61124085600160056009600d896002610b89565b6112548560026006600a600e896004610ba6565b61126885600360"
    "07600b600f896006610bc3565b61127c8560006005600a600f896008610be0565b6112908560016006600b600c8960"
    "0a610bfd565b6112a485600260076008600d89600c610c1a565b6112b785600360046009600e8981610c36565b6112"
    "cb85600060046008600c89600e610c53565b6112df85600160056009600d896004610c70565b6112f3856002600660"
    "0a600e896009610c36565b6113078560036007600b600f89600d610ca1565b61131b8560006005600a600f89600161"
    "0cbe565b61132f8560016006600b600c896000610cdb565b61134385600260076008600d89600b610bc3565b611357"
    "85600360046009600e896005610b89565b60005b60088160ff1610156113de578560ff600883011660108110611378"
    "57fe5b60200201518660ff83166010811061138c57fe5b602002015189602001518360ff166008811015156113a657"
    "fe5b6020020151181888602001518260ff166008811015156113c257fe5b67ffffffffffffffff9092166020929092"
    "02015260010161135a565b5050505050505050565b60008787601081106113f657fe5b602002015190506000888760"
    "10811061140b57fe5b60200201519050600089876010811061142057fe5b6020020151905060008a87601081106114"
    "3557fe5b6020020151905068010000000000000000868486010893508318602081811c91901b67ffffffffffffffff"
    "161868010000000000000000818308915067ffffffffffffffff82841860281b1682841860181c1892506801000000"
    "0000000000858486010893508318601081901c60309190911b67ffffffffffffffff16186801000000000000000081"
    "8308928318603f81901c60019190911b67ffffffffffffffff1618929150838b8b601081106114e957fe5b67ffffff"
    "ffffffffff9092166020929092020152828b8a6010811061150a57fe5b67ffffffffffffffff909216602092909202"
    "0152818b896010811061152b57fe5b67ffffffffffffffff9092166020929092020152808b886010811061154c57fe"
    "5b67ffffffffffffffff90921660209290920201525050505050505050505050565b61010060405190810160405280"
    "6008906020820280388339509192915050565b6101e0604051908101604052806115a26115e5565b81526020016115"
    "af61156d565b81526000602082018190526040820181905260609091015290565b6040805180820182529060029082"
    "9080388339509192915050565b6080604051908101604052806004906020820280388339509192915050565b610200"
    "60405190810160405280601090602082028038833950919291505056fea165627a7a72305820a59dc9d098d29bacdd"
    "88cb50c25c96ed4ba3047fd46a5c6ecf57e447a3c699100029");

using BuilderFn = decltype(&evmone::build_jumpdest_map);

enum : uint8_t
{
    OP_JUMPDEST = 0x5b,
    OP_PUSH1 = 0x60,
    OP_PUSH32 = 0x7f,
};

[[gnu::noinline]] auto build_bitset2(const uint8_t* code, size_t code_size)
{
    evmone::bitset m(code_size);
    for (size_t i = 0; i < code_size; ++i)
    {
        const auto op = code[i];
        if (op == OP_JUMPDEST)
            m.set(i);

        if ((op >> 5) == 0b11)
            i += static_cast<size_t>((op & 0b11111) + 1);
    }
    return m;
}

[[gnu::noinline]] auto build_vec(const uint8_t* code, size_t code_size)
{
    std::vector<bool> m(code_size);
    for (size_t i = 0; i < code_size; ++i)
    {
        const auto op = code[i];
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[i] = true;
        else if (op >= OP_PUSH1 && op <= OP_PUSH32)
            i += static_cast<size_t>(op - OP_PUSH1 + 1);
    }
    return m;
}

inline bool is_push(uint8_t op)
{
    //    return op >= OP_PUSH1 && op <= OP_PUSH32;
    // return (op >> 5) == 0b11;
    return (op & uint8_t{0b11100000}) == 0b01100000;
}

[[maybe_unused]] bool x = []() noexcept {
    size_t last_push = size_t(-1);
    std::vector<int> dists;
    for (size_t i = 0; i < test_bytecode.size();)
    {
        const auto op = test_bytecode[i];

        if (is_push(op))
        {
            if (last_push != size_t(-1))
                dists.push_back(static_cast<int>(i - last_push));
            last_push = i;
        }

        i += is_push(op) ? static_cast<size_t>(op - OP_PUSH1 + 2) : 1;
    }

    auto sum = std::accumulate(dists.begin(), dists.end(), 0);
    std::sort(dists.begin(), dists.end());

    std::cerr << "AVG: " << double(sum) / double(dists.size()) << "\n";
    std::cerr << "MED: " << dists[dists.size() / 2] << "\n";

    int last = 0;
    int count = 0;
    for (auto d : dists)
    {
        if (d == last)
            ++count;
        else
        {
            std::cerr << last << " x" << count << "\n";
            count = 1;
            last = d;
        }
    }
    return true;
}();

// uint8_t get_push_size(uint8_t x)
//{
//    return (x & 0b11111);
//}

[[gnu::noinline]] auto build_vec3(const uint8_t* code, size_t code_size)
{
    std::vector<bool> m(code_size);
    for (size_t i = 0; i < code_size;)
    {
        const auto op = code[i];
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[i] = true;

        i += is_push(op) ? static_cast<size_t>(op - OP_PUSH1 + 2) : 1;
    }
    return m;
}

[[gnu::noinline]] auto build_vec4(const uint8_t* code, size_t code_size)
{
    std::vector<bool> m(code_size);
    const auto code_beg = code;
    const auto code_end = code + code_size;
    for (; code < code_end; ++code)
    {
        const auto op = *code;
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[size_t(code - code_beg)] = true;
        else if (is_push(op))
            code += static_cast<size_t>(op - OP_PUSH1 + 1);
    }
    return m;
}

[[gnu::noinline]] auto build_vec5(const uint8_t* code, size_t code_size)
{
    std::vector<bool> m(code_size);
    for (size_t i = 0; i < code_size; ++i)
    {
        const auto op = code[i];
        const auto s = size_t(op - OP_PUSH1);
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[i] = true;
        else if (s <= 31)
            i += s + 1;
    }
    return m;
}

[[gnu::noinline]] auto build_vec6(const uint8_t* code, size_t code_size)
{
    std::vector<bool> m(code_size);
    for (size_t i = 0; i < code_size;)
    {
        const auto op = code[i];
        const auto s = size_t(op - OP_PUSH1);
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[i] = true;

        i += (s <= 31) ? s + 1 : 1;
    }
    return m;
}

[[gnu::noinline]] auto build_vec7(const uint8_t* code, size_t code_size)
{
    std::vector<bool> m(code_size);
    for (size_t i = 0; i < code_size;)
    {
        const auto op = code[i];
        const auto s = size_t(op - OP_PUSH1);
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[i] = true;

        const auto a = (s <= 31) ? s : 0;

        i += a + 1;
    }
    return m;
}

[[gnu::noinline]] auto build_bytes(const uint8_t* code, size_t code_size)
{
    std::vector<uint8_t> m(code_size);
    for (size_t i = 0; i < code_size; ++i)
    {
        const auto op = code[i];
        if (__builtin_expect(op == OP_JUMPDEST, false))
            m[i] = true;
        else if (op >= OP_PUSH1 && op <= OP_PUSH32)
            i += static_cast<size_t>(op - OP_PUSH1 + 1);
    }
    return m;
}

[[gnu::noinline]] auto build_shadow_code2p(const uint8_t* code, size_t code_size)
{
    std::unique_ptr<uint8_t[]> m{new uint8_t[code_size + 32]};
    long push_data = 0;
    auto p = m.get();
    const auto end = p + code_size;
    while (p != end)
    {
        const auto op = *code;
        *p = push_data <= 0 ? op : 0;
        --push_data;
        if (op >= OP_PUSH1 && op <= OP_PUSH32)
            push_data = op - OP_PUSH1 + 1;
        ++p;
        ++code;
    }
    return m;
}

[[gnu::noinline]] auto build_shadow_code3p(const uint8_t* code, size_t code_size)
{
    std::unique_ptr<uint8_t[]> m{new uint8_t[code_size + 32]};
    std::memcpy(m.get(), code, code_size);
    auto p = m.get();
    const auto end = p + code_size;
    while (p < end)
    {
        const auto op = *p++;
        if ((op >> 5) == 0b11)
        {
            const size_t s = (op & 0b11111);
            std::memset(p, 0, s + 1);
            p += s;
        }
    }
    return m;
}

[[gnu::noinline]] auto build_shadow_code4(const uint8_t* code, size_t code_size)
{
    std::unique_ptr<uint8_t[]> m{new uint8_t[code_size + 33]};
    std::memcpy(m.get(), code, code_size);
    long push_data = 0;
    for (size_t i = 0; i < code_size; ++i)
    {
        const auto op = m[i];
        bool is_push_data = push_data > 0;
        --push_data;
        if (!is_push_data && (op >> 5) == 0b11)
        {
            push_data = op - OP_PUSH1 + 1;
        }
        else if (is_push_data && op == OP_JUMPDEST)
        {
            m[i] = 0;
        }
    }
    return m;
}

[[gnu::noinline]] auto copy_by1(const uint8_t* code, size_t code_size)
{
    std::unique_ptr<uint8_t[]> m{new uint8_t[code_size + 32]};
    for (size_t i = 0; i < code_size; ++i)
    {
        const auto op = code[i];
        m[i] = op;
    }
    return m;
}

#pragma GCC push_options
#pragma GCC optimize("no-tree-loop-distribute-patterns")
[[gnu::noinline]] auto copy_by1p(const uint8_t* code, size_t code_size)
{
    std::unique_ptr<uint8_t[]> m{new uint8_t[code_size + 32]};
    auto p = m.get();
    const auto end = p + code_size;
    while (p != end)
        *p++ = *code++;
    return m;
}
#pragma GCC pop_options

[[gnu::noinline]] auto memcpy(const uint8_t* code, size_t code_size)
{
    std::unique_ptr<uint8_t[]> m{new uint8_t[code_size]};
    std::memcpy(m.get(), code, code_size);
    return m;
}

template <typename MapT, MapT Fn(const uint8_t*, size_t)>
void build_jumpdest(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto r = Fn(test_bytecode.data(), test_bytecode.size());
        benchmark::DoNotOptimize(r);
    }
}
}  // namespace

BENCHMARK_TEMPLATE(build_jumpdest, evmone::bitset, evmone::build_jumpdest_map);
BENCHMARK_TEMPLATE(
    build_jumpdest, evmone::bitset, evmone::experimental::build_jumpdest_map_bitset1);
BENCHMARK_TEMPLATE(build_jumpdest, evmone::bitset, build_bitset2);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<bool>, build_vec);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::vector<bool>, evmone::experimental::build_jumpdest_map_vec1);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::vector<bool>, evmone::experimental::build_jumpdest_map_vec2);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::vector<bool>, evmone::experimental::build_jumpdest_map_sttni);
BENCHMARK_TEMPLATE(
    build_jumpdest, evmone::experimental::bitset32, evmone::experimental::build_jumpdest_map_simd1);
BENCHMARK_TEMPLATE(
    build_jumpdest, evmone::experimental::bitset32, evmone::experimental::build_jumpdest_map_simd2);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<bool>, build_vec3);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<bool>, build_vec4);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<bool>, build_vec5);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<bool>, build_vec6);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<bool>, build_vec7);
BENCHMARK_TEMPLATE(build_jumpdest, std::vector<uint8_t>, build_bytes);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::unique_ptr<uint8_t[]>, evmone::experimental::build_internal_code_v1);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::unique_ptr<uint8_t[]>, evmone::experimental::build_internal_code_v2);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::unique_ptr<uint8_t[]>, evmone::experimental::build_internal_code_v3);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::unique_ptr<uint8_t[]>, evmone::experimental::build_internal_code_v4);
BENCHMARK_TEMPLATE(
    build_jumpdest, std::unique_ptr<uint8_t[]>, evmone::experimental::build_internal_code_v8);
BENCHMARK_TEMPLATE(build_jumpdest, std::unique_ptr<uint8_t[]>, build_shadow_code2p);
BENCHMARK_TEMPLATE(build_jumpdest, std::unique_ptr<uint8_t[]>, build_shadow_code3p);
BENCHMARK_TEMPLATE(build_jumpdest, std::unique_ptr<uint8_t[]>, build_shadow_code4);
BENCHMARK_TEMPLATE(build_jumpdest, std::unique_ptr<uint8_t[]>, copy_by1);
BENCHMARK_TEMPLATE(build_jumpdest, std::unique_ptr<uint8_t[]>, copy_by1p);
BENCHMARK_TEMPLATE(build_jumpdest, std::unique_ptr<uint8_t[]>, memcpy);
