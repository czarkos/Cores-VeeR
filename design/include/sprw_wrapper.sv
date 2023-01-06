/*
* Entity: sprw_module
* File: sprw_module.sv
* Author: Christos Zarkos
* Description: SPARROW module pipeline
*/

import sparrow::*;

module sprw_wrapper(
    input logic clk,
    input logic rstn,
    input logic holdn,
    input logic  [31:0] ra,
    input logic  [31:0] rb,
    input logic  [31:0] instr,

    //input logic bypass,

    output logic [31:0] sprw_out
);

    sprw_in_type  sdi;
    sprw_out_type sdo;

    assign sdi.ra      = ra;
    assign sdi.rb      = rb;
    assign sdi.op1     = instr[30:26];
    assign sdi.op2     = instr[14:12];
    assign sdi.rc_we   = 1'b1;

    assign sdi.ctrl.mk = 4'b0000;
    assign sdi.ctrl.ms = 1'b0;
    // assign sdi.sa   = ;
    // assign sdi.sb   = ;
    assign sdi.ctrl.ol = 2'b00;
    assign sdi.ctrl.od = 4'b0000;
    assign sdi.ctrl.hp = 1'b0;

    assign sdi.bpv     = 32'b00000000_00000000_00000000_00000000;
    assign sdi.bp      = 2'b00;

    sprw_module jack (
        .clk(clk),
        .rstn(rstn),
        .holdn(holdn),
        .sdi(sdi),  
        .sdo(sdo)  
    );

    assign sprw_out = sdo.result;
    // assign sprw_out = (bypass) ? sdo.s2bp : sdo.result;
    //assign sprw_out = sdo.s2bp;
endmodule
