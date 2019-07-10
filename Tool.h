#pragma once
#ifndef _TYPE_H_
#define _TYPE_H_

#include <string>
#include <sstream>
enum Types
{
    _R,
    _I,
    _S,
    _B,
    _U,
    _J,
    ERROR = -1
};
enum R_type
{
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    RERROR = -1
};
enum I_type
{
    SLLI,
    SRLI,
    SRAI,
    JALR,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    IERROR = -1
};
enum S_type
{
    SB,
    SH,
    SW,
    SERROR = -1
};
enum B_type
{
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    BERROR = -1
};
enum U_type
{
    LUI,
    AUIPC,
    UERROR = -1
};
enum J_type
{
    JAL,
    JERROR = -1
};
enum REG_type
{
    zero,
    ra,
    sp,
    gp,
    tp,
    t0,
    t1,
    t2,
    s0,
    s1,
    a0,
    a1,
    a2,
    a3,
    a4,
    a5,
    a6,
    a7,
    s2,
    s3,
    s4,
    s5,
    s6,
    s7,
    s8,
    s9,
    s10,
    s11,
    t3,
    t4,
    t5,
    t6
};

extern Types getType(uint32_t _buf);

extern std::string type_name[6];
extern std::string R_type_name[];
extern std::string I_type_name[];
extern std::string S_type_name[];
extern std::string B_type_name[];
extern std::string U_type_name[];
extern std::string REG_type_name[];

struct REG
{
    REG() : rs1(0), rs2(0), rd(0), imm(0), rs1_sum(0), rs2_sum(0), sum(0), flag(false) {}
    Types type = ERROR;
    R_type r_type = RERROR;
    I_type i_type = IERROR;
    S_type s_type = SERROR;
    B_type b_type = BERROR;
    U_type u_type = UERROR;
    J_type j_type = JERROR;
    int rs1 = 0, rs2 = 0, rd = 0, imm = 0;
    int rs1_sum = 0, rs2_sum = 0, sum = 0;
    int _pc = 0;
    bool flag = false;

    std::string OUT()
    {
        std::stringstream sstream;
        if (!flag)
            sstream << "false";
        else
        {
            switch (type)
            {
            case _R:
                sstream << "type:" << R_type_name[r_type] <<
                         " rs1:" << REG_type_name[rs1] << " rs2:" << REG_type_name[rs2] <<
                         " rd:" << REG_type_name[rd] << " rs1_sum:" << rs1_sum << " rs2_sum" << rs2_sum;
                break;
            case _I:
                sstream << "type:" << I_type_name[i_type] <<
                         " rs1:" << REG_type_name[rs1] << " rd:" << REG_type_name[rd] <<
                         " rs1_sum:" << rs1_sum << " immediate:" << std::hex << imm;
                break;
            case _S:
                sstream << "type:" << S_type_name[s_type] <<
                         " rs1:" << REG_type_name[rs1] << " rs2:" << REG_type_name[rs2] <<
                         " rs1_sum:" << rs1_sum << " rs2_sum:" << rs2_sum << " immediate:" << std::hex << imm;
                break;
            case _B:
                sstream << "type:" << B_type_name[b_type] <<
                            " rs1:" << REG_type_name[rs1] << " rs2:" << REG_type_name[rs2] <<
                            " rs1_sum:" << rs1_sum << " rs2_sum:" << rs2_sum << " immediate:" << std::hex << imm;
                break;
            case _U:
                sstream << "type:" << U_type_name[u_type] << " rd:" << REG_type_name[rd] << " immediate:" << std::hex << imm;
                break;
            case _J:
                sstream << "type:JAL rd:" << REG_type_name[rd] << " immediate:" << std::hex << imm;
                break;
            }
        }
        sstream << " pc:" << std::hex << this->_pc;
        return sstream.str();
    }
};
#endif