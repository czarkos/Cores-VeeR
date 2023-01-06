/*
* Entity: sparrow
* File: sparrow.sv
* Author: Christos Zarkos
* Description: SPARROW types
*/
// `timescale 1ns/1ps

package sparrow;
   localparam string sprw_version = "1.0";

   localparam integer XLEN = 32;     // CFG_XLEN
   localparam integer VLEN = 8;      // CFG_VLEN
   localparam integer LOGSZ = 2;     // CFG_LOGSZ
   localparam integer VSIZE = XLEN/VLEN;

   typedef logic [XLEN-1:0] word;

   // vector register type
   typedef logic [VLEN-1:0] vector_component;
   // typedef vector_component [VSIZE-1:0] vector_reg_type;
   typedef logic [VSIZE-1:0][VLEN-1:0] vector_reg_type;
   // typedef vector_component vector_reg_type [VSIZE];
   
   // interstage vector register type (high precision)
   typedef logic [2*VLEN-1:0] high_prec_component;
   // typedef high_prec_component [VSIZE-1:0] inter_reg_type;
   typedef logic [VSIZE-1:0][2*VLEN-1:0] inter_reg_type;
   // typedef high_prec_component inter_reg_type [VSIZE];

   /* SPARROW CONTROL REGISTER */

   // mask registers(predicate)
   typedef logic [(XLEN/VLEN)-1:0] mask_reg_type;

   // output type selection
   typedef logic[1:0] output_length_type;
   
   // duplicate output
   typedef logic [(XLEN/VLEN)-1:0] output_dup_select;

   // swizzling registers (reordering)
   typedef integer log_length;  // TODO: add assertion to check the range later
   typedef log_length [(XLEN/VLEN)-1:0] swizzling_reg_type;

   typedef struct packed {
       mask_reg_type mk;        // mask value 
       logic ms;                // mask selection (ra or 0)
       swizzling_reg_type sa;   // swizzling ra
       swizzling_reg_type sb;   // swizzling rb
       output_length_type ol;   // output type (word/half/byte)
       output_dup_select od;    // output duplication
       logic hp;                // use rhd for shift and high prec. multiplication
       // vector_reg_type ac;
       } sprw_ctrl_reg_type /*verilator public*/;

    typedef struct packed {
        word ra;                    // operand 1 data
        word rb;                    // operand 2 data
        logic [4:0] op1;            // operation code stage1
        logic [2:0] op2;            // operation code stage2
        logic   rc_we;              // we on destination (work)
        sprw_ctrl_reg_type ctrl;    // special register to control the module behavior
        word bpv;                   // value coming from memory
        logic [1:0] bp;             // bp mux value (bit0 = rs1, bit1 = rs2)
    } sprw_in_type /*verilator public*/;

    typedef struct packed {
        word result;    // output data
        word s1bp;      // s1 bypass output data
        word s2bp;      // s2 bypass output data
    } sprw_out_type /*verilator public*/;

    typedef struct packed {
        vector_component opA;
        vector_component opB;
        logic sign;
        logic sat;
    } lpmul_in_type;

    typedef struct packed {
        high_prec_component mul_res;
    } lpmul_out_type;

    /* CONSTANTS FOR OPERATIONS */

    // constant function operations stage1 (sprw_code 4-0)
    localparam logic [4:0] S1_NOP   = 5'b00000;
    localparam logic [4:0] S1_ADD   = 5'b00001;
    localparam logic [4:0] S1_SUB   = 5'b00010;
    localparam logic [4:0] S1_MUL   = 5'b00011;
    localparam logic [4:0] S1_MAX   = 5'b00101;
    localparam logic [4:0] S1_MIN   = 5'b00110;
    localparam logic [4:0] S1_AND   = 5'b00111;
    localparam logic [4:0] S1_OR    = 5'b01000;
    localparam logic [4:0] S1_XOR   = 5'b01001;
    localparam logic [4:0] S1_NAND  = 5'b01010;
    localparam logic [4:0] S1_NOR   = 5'b01011;
    localparam logic [4:0] S1_XNOR  = 5'b01100;
    localparam logic [4:0] S1_SADD  = 5'b01101;
    localparam logic [4:0] S1_SSUB  = 5'b01110;
    localparam logic [4:0] S1_SMUL  = 5'b01111;
    localparam logic [4:0] S1_MOVB  = 5'b10000;
    localparam logic [4:0] S1_SHFT  = 5'b10001;
    localparam logic [4:0] S1_UMUL  = 5'b10011;
    localparam logic [4:0] S1_UMAX  = 5'b10101;
    localparam logic [4:0] S1_UMIN  = 5'b10110;
    localparam logic [4:0] S1_SSHFT = 5'b11001;
    localparam logic [4:0] S1_USADD = 5'b11101;
    localparam logic [4:0] S1_USSUB = 5'b11110;
    localparam logic [4:0] S1_USMUL = 5'b11111;

    // constant function operations stage2 (sprw_code 7-5)
    localparam logic [2:0] S2_NOP    = 3'b000;
    localparam logic [2:0] S2_SUM    = 3'b001;
    localparam logic [2:0] S2_MAX    = 3'b010;
    localparam logic [2:0] S2_MIN    = 3'b011;
    localparam logic [2:0] S2_XOR    = 3'b100;

    localparam logic [2:0] S2_USUM   = 3'b101;
    localparam logic [2:0] S2_UMAX   = 3'b110;
    localparam logic [2:0] S2_UMIN   = 3'b111;
endpackage
