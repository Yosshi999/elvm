#include <ir/ir.h>
#include <target/util.h>

static const char* REG_NAMES_RAIL[] = {
    "a", "b", "c", "d", "bp", "sp", "pc"
};


static void init_state_rail(Data* data) {
    reg_names = REG_NAMES_RAIL;
    FILE *fp = fopen("./rail_template.txt", "r");
    if (fp == NULL) {
        error("no file");
        return;
    }
    char buf[5000];
    fread(buf, 5000, 1, fp);
    puts(buf);

    printf("$'main'\n \\-{initmem}-");
}


static char* string_load_code_rail(Value operand) {
    if (operand.type == REG) {
        return format("(%s)", reg_names[operand.reg]);
    } else (operand.type == IMM) {
        return format("[%d]", operand.imm);
    } else {
        error("invalid value");
        return "";
    }
}


const char* cmp_str_rail(Inst* inst) {
    int op = normalize_cond(inst->op, 0);
    const char* op_str;
    switch (op) {
        case JEQ:
        op_str = "=="; break;
        case JNE:
        op_str = "!="; break;
        case JLT:
        op_str = "<"; break;
        case JGT:
        op_str = ">"; break;
        case JLE:
        op_str = "<="; break;
        case JGE:
        op_str = ">="; break;
        case JMP:
        return "t";
        default:
        error("oops");
    }
    return format("%s(%s){%s}",
        string_store_code_rail(inst->src),
        reg_names[inst->dst.reg],
        op_str);
}

static void rail_emit_inst(Inst* inst) {
    switch (inst -> op) {
    case MOV:
        printf("%s(!%s!)",
            string_load_code_rail(inst->src),
            reg_names[inst->dst.reg]);
        break;
    case ADD:
        printf("%s(%s)a(!%s!)",
            string_load_code_rail(inst->src),
            reg_names[inst->dst.reg],
            reg_names[inst->dst.reg]);
        break;
    case SUB:
        printf("%s(%s)a(!%s!)",
            string_load_code_rail(inst->src),
            reg_names[inst->dst.reg],
            reg_names[inst->dst.reg]);
        break;

    case LOAD:
        printf("%s{load}(!%s!)",
            string_load_code_rail(inst->src),
            reg_names[inst->dst.reg]);
        break;
    case STORE:
        printf("%s(!%s!){write}",
            string_load_code_rail(inst->src),
            reg.names[inst->dst.reg]);
        break;
    case PUTC:
        printf("(!%s!)[128]r{putc}",
            string_load_code_rail(inst->src));
        break;
    case GETC:
        printf("{getc}(!%s!)", reg_names[inst->dst.reg]);
        break;
    case EXIT:
        printf("#\n");
        break;
    
    case JEQ:
    case JNE:
    case JLT:
    case JGT:
    case JLE:
    case JGE:
    case JMP:
        printf("%s%s(pc){if}(!pc!)",
            cmp_str_rail(inst),
            string_load_code_rail(inst->jmp));
        break;
    
    case EQ:
    case NE:
    case LT:
    case GT:
    case LE:
    case GE:
        printf("%s10{if}", cmp_str_rail(inst));
        break;
    
    case DMP:
        break;
    default:
        error("invalid opecode");
    }
}
