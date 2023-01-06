/*
* Entity: sprw_module
* File: sprw_module.sv
* Author: Christos Zarkos
* Description: SPARROW module pipeline
*/

import sparrow::*;

module sprw_module(
    input logic clk,
    input logic rstn,
    input logic holdn,
    input sprw_in_type sdi,
    output sprw_out_type sdo
);

    /* REGISTER TYPES DEFINITION */

    typedef lpmul_in_type  lpmul_in_array [0:VSIZE-1];      // TODO: CHECK SEMANTICS
    typedef lpmul_out_type lpmul_out_array [0:VSIZE-1];     // TODO: CHECK SEMANTICS

    typedef logic [VLEN:0] l1_sum_array [0:VSIZE/2];        // TODO: CHECK SEMANTICS
    typedef logic [VLEN+1:0] l2_sum_array [0:VSIZE/4];      // TODO: CHECK SEMANTICS

    /* Stage1 entry register */
    typedef struct packed {
        vector_reg_type ra;
        vector_reg_type rb;
        logic [4:0] op1;
        logic [2:0] op2;
        logic en;
    } s1_reg_type; 

    /* Stage2 entry register */
    typedef struct packed {
        inter_reg_type ra;
        logic [2:0] op2;
        logic sat;
        logic en;
    } s2_reg_type; 

    /* Stage3 entry register */
    typedef struct packed {
        word rc;
    } s3_reg_type; 

    /* Group of pipeline registers */
    typedef struct packed {
        s1_reg_type s1;
        s2_reg_type s2;
        s3_reg_type s3;
        vector_reg_type rdh;
    } registers;

    /* CONSTANTS FOR PIPELINE REGISTERS RESET */
    localparam vector_reg_type vector_reg_res = '{default:0};
    localparam inter_reg_type inter_reg_res = '{default:0};

    /* set the 1st stage registers reset */
    localparam s1_reg_type s1_reg_res = '{
        vector_reg_res,
        vector_reg_res,
        5'b0000_0,
        3'b000,
        1'b0
    };

    /* set the 2nd stage registers reset */
    localparam s2_reg_type s2_reg_res = '{
        inter_reg_res,
        3'b000,
        1'b0,
        1'b0
    };

    /* set the 3rd stage registers reset */
    localparam s3_reg_type s3_reg_res = '{
        32'b00000000_00000000_00000000_00000000
    };

    /* reset all registers */
    localparam registers RRES = '{
        s1_reg_res,
        s2_reg_res,
        s3_reg_res,
        vector_reg_res
    };

    /* FUNCTIONS */
    function vector_reg_type word_to_vector(input word data);
        automatic vector_reg_type vec;
        vec[0] = data[7:0];
        vec[1] = data[15:8];
        vec[2] = data[23:16];
        vec[3] = data[31:24];
        return vec;
    endfunction : word_to_vector

    function vector_reg_type inter_to_vector(input inter_reg_type data, input bit high );
        automatic vector_reg_type vec;
        for(integer i = 0; i < 4; i++) begin
            if(high)
                vec[i] = data[i][15:8];
            else
                vec[i] = data[i][7:0];
        end
        return vec;
    endfunction : inter_to_vector

    function word vector_to_word(vector_reg_type vec);
        automatic word data;
        data[7:0]   = vec[0];
        data[15:8]  = vec[1];
        data[23:16] = vec[2];
        data[31:24] = vec[3];
        return data;
    endfunction : vector_to_word

    function word inter_to_word(inter_reg_type vec);
        automatic word data;
            data[7:0]   = vec[0][7:0];
            data[15:8]  = vec[1][7:0];
            data[23:16] = vec[2][7:0];
            data[31:24] = vec[3][7:0];
        return data;
    endfunction : inter_to_word

    // TODO
    function vector_reg_type swizzling(input vector_reg_type data,
                                       input swizzling_reg_type sz);
       vector_reg_type result;
       for(integer i = 0; i < 4; i++) begin
           result[i] = data[sz[i]];
       end
       return result;
    endfunction : swizzling

    /* SATURATION FUNCTIONS */
    function integer clipping(input integer value, max_val, min_val);
        automatic integer ret_val;
        if (value > max_val)
            ret_val = max_val;
        else if (value < min_val)
            ret_val = min_val;
        else
            ret_val = value;
        return ret_val;
    endfunction : clipping

    // TODO: Check if it can be made more generic
    function high_prec_component extend(vector_component value, input logic sign);
        high_prec_component z;
        if (value[7] == 1'b1 & sign == 1'b1)
            z = {8'b1111_1111, value};
        else
            z = {8'b0000_0000, value};
        return z;
    endfunction : extend

    function high_prec_component signed_sat16(input integer a, logic sat);
        integer ret;
        if (sat == 1)
            ret = clipping(a, 127, -128);
        else
            ret = a;
        return ret[15:0];
    endfunction : signed_sat16

    function high_prec_component unsigned_sat16(input integer a, logic sat);
        integer ret;
        if (sat == 1)
            ret = clipping(a, 255, 0);
        else
            ret = a;
        return ret[15:0];
    endfunction : unsigned_sat16

    /* MUX FUNCTION */
    function vector_reg_type op_mux(
        input vector_reg_type vec, bpv,
        input logic[1:0] bp_sel,
        input integer reg_sel);
    begin
        if (bp_sel[reg_sel] == 1'b1)
            return bpv;
        else
            return vec;
    end
    endfunction : op_mux

    /* TWO OPERARANDS OPERATIONS (S1) */

    // s1 result multiplexor
    task s1_mux(input logic [4:0] op, output logic[3:0] sel);
        sel = {1'b0, op[2:0]};

        if(op == S1_SADD || op == S1_USADD)
            sel = 4'b0001;
        else if(op == S1_SSUB || op == S1_USSUB)
            sel = 4'b0010;
        else if(op == S1_SMUL || op == S1_USMUL)
            sel = 4'b0011;
        else if(op == S1_AND || op == S1_OR || op == S1_XOR || op == S1_NAND || op == S1_NOR || op == S1_XNOR)
            sel = 4'b0111;
        else if(op == S1_MOVB)
            sel = 4'b0100;
        else if(op == S1_SHFT || op == S1_SSHFT)
            sel = 4'b1000;
        else
            sel = {1'b0, op[2:0]};
            
    endtask : s1_mux

    task s1_select;
        input logic [3:0] sel;
        input inter_reg_type ra, rs2, add_res, sub_res, max_res, min_res, logic_res, shift_res, mul_res;
        output inter_reg_type s1_res;
    begin
        case(sel)
            4'b0000 : s1_res = ra;
            4'b0001 : s1_res = add_res;
            4'b0010 : s1_res = sub_res;
            4'b0011 : s1_res = mul_res;
            4'b0100 : s1_res = rs2;
            4'b0101 : s1_res = max_res;
            4'b0110 : s1_res = min_res;
            4'b0111 : s1_res = logic_res;
            4'b1000 : s1_res = shift_res;
            default : s1_res = '{default:0};
        endcase
    end
    endtask : s1_select

    //TODO: make signed sat and unsigned_sat so that the add and sub work
    function high_prec_component add(input vector_component a, b, logic sign, sat);
        integer z = int'(signed'(a)) + int'(signed'(b));
        assert(z > -512); assert(z < 512);
        if (sign == 1)
            return signed_sat16(z, sat);
        else
            return unsigned_sat16(z, sat);
    endfunction : add

    function high_prec_component sub(input vector_component a, b, logic sign, sat);
        integer z = int'(signed'(a)) - int'(signed'(b));
        assert(z > -512); assert(z < 512);
        if (sign == 1)
            return signed_sat16(z, sat);
        else
            return unsigned_sat16(z, sat);
    endfunction : sub

    // max function
    function high_prec_component max(input vector_component a, b, input logic sign);
        vector_component z;
        if (sign == 1) begin
            if (signed'(a) > signed'(b))
                z = a;
            else
                z = b;
        end
        else begin
            if (a > b)
                z = a;
            else
                z = b;
        end

        return extend(z, sign);
    endfunction : max

    // min function
    function high_prec_component min(input vector_component a, b, input logic sign);
        vector_component z;
        if (sign == 1) begin
            if (signed'(a) > signed'(b))
                z = b;
            else
                z = a;
        end
        else begin
            if (a > b)
                z = b;
            else
                z = a;
        end

        return extend(z, sign);
    endfunction : min

    // logic operations
    function vector_component logic_op(input vector_component a, b, logic [2:0] op);
        vector_component z;
        case(op)
            3'b111: z = a & b;
            3'b000: z = a | b;
            3'b001: z = a ^ b;
            3'b010: z = a ~& b;
            3'b011: z = a ~| b;
            3'b100: z = a ~^ b;
            default: z = 8'b0000_0000;
        endcase
        return z;
    endfunction : logic_op

    // TODO: CHECK LOGIC AND WHY EVERYTHING IS DONE LATER
    function high_prec_component shift_func(input vector_component a, b, rdh, input logic sat);
        integer z, i;
        high_prec_component c, new_c;
        c = {rdh, a};
        if (b[7] == 1'b1) begin // shift rigth
            i = -1*int'(signed'(b[7:1]));
            if (b[0] == 1'b1) // arithmetic
                return c >> i;
            else begin
                //new_c = (~c + 16'b0000_0000_0000_0001) >> i;
                //z =  -1 * int'(new_c);
                //return z[15:0];
                return c >> i;
            end
        end
        else begin
            i = int'(signed'(b[7:1]));
            new_c = c << i;
            z = int'(new_c);
            if (b[0] == 1'b1) // signed
                return signed_sat16(z, sat);
            else
                return unsigned_sat16(z, sat);
        end
    endfunction : shift_func

    function high_prec_component shift(input vector_component a, b, rdh, input logic sat, hp);
        high_prec_component z;
        vector_component zero = 8'b0000_0000;
        if (hp == 1'b1)
            z = shift_func(a, b, rdh, sat);
        else
            z = shift_func(a, b, zero, sat);
        return z;
    endfunction : shift

    task mask;
        input inter_reg_type vector, original;
        input logic [VSIZE-1:0] msk;
        input logic pas_ra;
        output inter_reg_type msk_res;
    begin
        if (pas_ra == 1'b1)
            msk_res = original;
        else
            msk_res = '{default:0};

        for (integer i = 0; i < 8; i++) begin
            if (msk[i] == 1'b1)
                msk_res[i] = vector[i];
        end
    end
    endtask : mask

    /* REDUCTION OPERATIONS */
    task s2_select;
        input logic [2:0] sel;
        input word ra, sum_res, max_res, min_res, xor_res;
        output word rc;
    begin
        if (sel == 3'b000)
            rc = ra;
        else if (sel == 3'b001 || sel == 3'b101)
            rc = sum_res;
        else if (sel == 3'b010 || sel == 3'b110)
            rc = max_res;
        else if (sel == 3'b011 || sel == 3'b111)
            rc = min_res;
        else if (sel == 3'b100)
            rc = xor_res;
        else
            rc = ra;
    end
    endtask : s2_select

    function word signed_sat32(input integer a, logic sat);
        integer ret;
        if (sat == 1)
            ret = clipping(a, 127, -128);
        else
            ret = a;
        return ret[31:0];
    endfunction : signed_sat32

    function word unsigned_sat32(input integer a, logic sat);
        integer ret;
        if (sat == 1)
            ret = clipping(a, 255, 0);
        else
            ret = a;
        return ret[31:0];
    endfunction : unsigned_sat32

    function word sum(input inter_reg_type a, logic sign, sat);
        automatic integer acc = 0;
        if (sign == 1'b1) begin
            for (integer i = 0; i < VSIZE; i++)
                acc = acc + int'(signed'(a[i]));
            return signed_sat32(acc, sat);
        end
        else begin
            for (integer i = 0; i < VSIZE; i++)
                acc = acc + int'(unsigned'(a[i]));
            return unsigned_sat32(acc, sat);
        end
    endfunction : sum

    // max recursive function
    function word max_red(input inter_reg_type a, logic sign);
        integer acc;
        if (sign == 1'b1) begin
            acc = int'(signed'(a[0]));
            for (integer i = 1; i < VSIZE; i++) begin
                if (int'(signed'(a[i])) > acc)
                    acc = int'(signed'(a[i]));
            end
        end
        else begin
            acc = int'(unsigned'(a[0]));
            for (integer i = 1; i < VSIZE; i++) begin
                if (int'(unsigned'(a[i])) > acc)
                    acc = int'(unsigned'(a[i]));
            end
        end

        return unsigned_sat32(acc, 0'b0);
    endfunction : max_red

    // min recursive function
    function word min_red(input inter_reg_type a, logic sign);
        integer acc;
        if (sign == 1'b1) begin
            acc = int'(signed'(a[0]));
            for (integer i = 1; i < VSIZE; i++) begin
                if (int'(signed'(a[i])) < acc)
                    acc = int'(signed'(a[i]));
            end
        end
        else begin
            acc = int'(unsigned'(a[0]));
            for (integer i = 1; i < VSIZE; i++) begin
                if (int'(unsigned'(a[i])) < acc)
                    acc = int'(unsigned'(a[i]));
            end
        end

        return unsigned_sat32(acc, 0'b0);
    endfunction : min_red

    function word xor_red(inter_reg_type a);
        vector_component acc = a[0][7:0];
        for (integer i = 1; i < VSIZE; i++) begin
            acc = a[i][7:0] ^ acc;
        end
        return {24'b00000000_00000000_00000000, acc};
    endfunction : xor_red

    function logic sign_ext(logic [4:0] op1, logic [2:0] op2);
        logic sign;
        if (op1 == S1_ADD || op1 == S1_NOP || op1 == S1_MOVB)
            sign = ~op2[2] && (op2[1] || op2[0]);
        else
            sign = ~ op1[4];
        return sign;
    endfunction : sign_ext


    /* SIGNAL DEFINITIONS */
    registers r, rin;

    lpmul_in_array lpmuli /*verilator split_var*/;
    lpmul_out_array lpmulo /*verilator split_var*/;

    logic [15:0] mul0_result;

    /* GENERATE 4 MULTIPLICATION UNITS */
    /*
    generate
        genvar i;
        for (i = 0; i < 4; i = i + 1) begin
            lpmul mul(.muli(lpmuli[i]),
                      .mulo(lpmulo[i])
            );
        end
    endgenerate
    */
    lpmul mul0 (
        .muli (lpmuli[0]),
        .mulo (lpmulo[0])
    );
    lpmul mul1 (
        .muli (lpmuli[1]),
        .mulo (lpmulo[1])
    );
    lpmul mul2 (
        .muli (lpmuli[2]),
        .mulo (lpmulo[2])
    );
    lpmul mul3 (
        .muli (lpmuli[3]),
        .mulo (lpmulo[3])
    );

    /* VARIABLES FOR MAIN BODY */
    registers v;
    vector_reg_type op1, op2;
    vector_reg_type rs1, rs2;
    inter_reg_type s1_res;
    inter_reg_type s1_ra, s1_r2, add_res, sub_res, mul_res, max_res, min_res, logic_res, shift_res;
    logic [3:0] s1_alusel;
    word s2sum_res, s2max_res, s2min_res, s2xor_res;

    logic sign;
    word s2_res;

    logic sdi_ctrl_hp;
    /* MAIN BODY */
    always_comb begin
        // v = r;

        // INPUT TO S1
        v.s1.ra = word_to_vector(sdi.ra);
        v.s1.rb = word_to_vector(sdi.rb);
        v.s1.en = sdi.rc_we; v.s1.op1 = sdi.op1; v.s1.op2 = sdi.op2;

        // MUX BP from memory
        op1 = op_mux(r.s1.ra, word_to_vector(sdi.bpv), sdi.bp, 0);
        op2 = op_mux(r.s1.rb, word_to_vector(sdi.bpv), sdi.bp, 1);

        // Swizzling
        //rs1 = swizzling(op1, sdi.ctrl.sa);
        //rs2 = swizzling(op2, sdi.ctrl.sb);
        rs1 = op1;
        rs2 = op2;

        /*
        for (integer i = 0; i < 4; i++) begin
            lpmuli[i].opA  = rs1[i];
            lpmuli[i].opB  = rs2[i];
            lpmuli[i].sign = ~r.s1.op1[4];
            lpmuli[i].sat  = r.s1.op1[3]; 
        end
        */
        lpmuli[0].opA  = rs1[0];
        lpmuli[0].opB  = rs2[0];
        lpmuli[0].sign = ~r.s1.op1[4];
        lpmuli[0].sat  = r.s1.op1[3]; 

        lpmuli[1].opA  = rs1[1];
        lpmuli[1].opB  = rs2[1];
        lpmuli[1].sign = ~r.s1.op1[4];
        lpmuli[1].sat  = r.s1.op1[3]; 

        lpmuli[2].opA  = rs1[2];
        lpmuli[2].opB  = rs2[2];
        lpmuli[2].sign = ~r.s1.op1[4];
        lpmuli[2].sat  = r.s1.op1[3]; 
        
        lpmuli[3].opA  = rs1[3];
        lpmuli[3].opB  = rs2[3];
        lpmuli[3].sign = ~r.s1.op1[4];
        lpmuli[3].sat  = r.s1.op1[3]; 

        sign = sign_ext(r.s1.op1, r.s1.op2);
        s1_mux(r.s1.op1, s1_alusel);

        sdi_ctrl_hp = sdi.ctrl.hp;
        // S1 TO S2
        for(integer i = 0; i < 4; i++) begin
            add_res[i]   = add(rs1[i], rs2[i], sign, r.s1.op1[3]);
            sub_res[i]   = sub(rs1[i], rs2[i], sign, r.s1.op1[3]);
            //mul_res[i]   = lpmulo[i].mul_res;
            max_res[i]   = max(rs1[i], rs2[i], sign);
            min_res[i]   = min(rs1[i], rs2[i], sign);
            shift_res[i] = shift(rs1[i], rs2[i], r.rdh[i], r.s1.op1[3], sdi.ctrl.hp);
            logic_res[i] = extend(logic_op(rs1[i], rs2[i], r.s1.op1[2:0]), 0'b0);
            s1_ra[i]     = extend(r.s1.ra[i], sign);
            s1_r2[i]     = extend(rs2[i], sign);
        end
        mul_res[0]   = lpmulo[0].mul_res;
        mul_res[1]   = lpmulo[1].mul_res;
        mul_res[2]   = lpmulo[2].mul_res;
        mul_res[3]   = lpmulo[3].mul_res;

        s1_select(s1_alusel, s1_ra, s1_r2, add_res, sub_res, max_res, min_res,
                  logic_res, shift_res, mul_res, s1_res);

        //mask(s1_res, s1_ra, sdi.ctrl.mk, sdi.ctrl.ms, v.s2.ra);
        v.s2.ra = s1_res;

        v.s2.op2 = r.s1.op2;
        v.s2.sat = r.s1.op1[3];
        v.s2.en  = r.s1.en;
        v.rdh    = inter_to_vector(v.s2.ra, 1'b1);

        // S2 TO S3
        s2sum_res = sum(r.s2.ra, ~r.s2.op2[2], r.s2.sat);
        s2max_res = max_red(r.s2.ra, ~r.s2.op2[2]);
        s2min_res = min_red(r.s2.ra, ~r.s2.op2[2]);
        s2xor_res = xor_red(r.s2.ra);

        s2_select(r.s2.op2, inter_to_word(r.s2.ra), s2sum_res, s2max_res, s2min_res, s2xor_res, s2_res);
        v.s3.rc = s2_res;

        sdo.result = r.s3.rc;
        sdo.s1bp = inter_to_word(v.s2.ra);
        sdo.s2bp = v.s3.rc;

        rin = v;
    end

    always_ff @(posedge clk) begin
        if (holdn == 1'b1)
            r <= rin;
        if (rstn == 1'b0) begin
            r <= RRES;
        end

        /*
        for (integer i = 0; i < 4; i++) begin
            lpmuli[i].opA  <= rs1[i];
            lpmuli[i].opB  <= rs2[i];
            lpmuli[i].sign <= ~r.s1.op1[4];
            lpmuli[i].sat  <= r.s1.op1[3]; 
        end
        */
    end
endmodule
