/*
* Entity: lpmul
* File: lpmul.sv
* Author: Christos Zarkos
* Description: SPARROW low-precision (8-bit) multiplier
*/

import sparrow::*;

module lpmul
(
    input lpmul_in_type muli,
    output lpmul_out_type mulo
);

    // localparam vector_component SMAX = {1'b0, (VLEN-1){1'b1}};
    localparam vector_component SMAX = {1'b0, 7'b1111_111};
    // localparam vector_component SMIN = {1'b1, (VLEN-1){1'b0}};
    localparam vector_component SMIN = {1'b1, 7'b0000_000};
    // localparam vector_component UMAX = VLEN{1'b1};
    localparam vector_component UMAX = 8'b1111_1111;

    function vector_component sign_invert8 (input vector_component a);
        automatic logic [7:0] tmp_a = ~a + 8'b0000_0001;
        //return ~a + 8'b0000_0001;
        return tmp_a;
    endfunction : sign_invert8

    function high_prec_component sign_invert16 (input high_prec_component a);
        automatic logic [15:0] tmp_a = ~a + 16'b0000_0000_0000_0001;
        //return ~a + 16'b0000_0000_0000_0001;
        return tmp_a;
    endfunction : sign_invert16

    function high_prec_component product (input vector_component a, b);
        automatic integer z = 0;
        automatic integer aux = int'(unsigned'(a));

        for (integer i = 0; i < VLEN; i++) begin
            if (b[i] == 1) begin
                z = aux +z;
            end
            aux = aux*2;
        end
        
        return z[15:0];
    endfunction : product

    function logic [2:0] sat_mux (input logic asign, bsign, rsign, sign, sat, 
                                    vector_component z2);
        automatic logic [2:0] sel = 3'b000;                         // result as it is, no saturation
        if (sat == 1'b1) begin
            if (sign == 1'b1) begin
                if (asign == bsign) begin                           // result should be positive
                    if (z2 != 8'b0000_0000 | rsign != 1'b0)        // overflow including sign bit
                        sel = 3'b011;                               // result is 7f signed max
                end
                else begin                                          // result should be negative 
                    if (z2 != 8'b0000_0000)                         // overflow
                        sel = 3'b100;                               // result is 80 signed min
                    else sel = 3'b001;
                end
            end
            else if (z2 != 8'b0000_0000) begin                      // overflow
                sel = 3'b111;                                       // result is ff unsigned max
            end
        end
        else if (sign == 1'b1) begin                                // if no saturation but signed
            if (asign != bsign)                                     // and result should be negative
                sel = 3'b001;                                       // result is ca2 negative
        end

        return sel;
    endfunction : sat_mux

    task sat_sel (input logic [2:0] sel, input high_prec_component r, nr,
                output high_prec_component mulres);
        logic [7:0] bit0 = 8'b0000_0000;
        logic [7:0] bit1 = 8'b1111_1111;

        case(sel)
            3'b000: mulres = r;
            3'b001: mulres = nr;
            3'b011: mulres = {bit0, SMAX};
            3'b100: mulres = {bit1, SMIN};
            3'b111: mulres = {bit0, UMAX};
            default: mulres = {bit0, bit0};
        endcase
    endtask : sat_sel

    high_prec_component z, nz;
    // high_prec_component out_reg;
    vector_component a, b, inverted_a, inverted_b;
    logic signA, signB;
    logic is_there_sat;
    logic [2:0] mux;

    always_comb begin
        is_there_sat = muli.sat;
        signA = muli.opA[VLEN-1];
        signB = muli.opB[VLEN-1];

        // have both operands as positive if signed and saturation
        if (muli.sign == 1'b1 && signA == 1'b1)
            a = sign_invert8(muli.opA);
        else
            a = muli.opA;
        if (muli.sign == 1'b1 && signB == 1'b1)
            b = sign_invert8(muli.opB);
        else
            b = muli.opB;

        //inverted_a = sign_invert8(muli.opA);
        //inverted_b = sign_invert8(muli.opB);

        // low precision product
        z = product(a, b);
        // select result with sign and saturation
        mux = sat_mux(signA, signB, z[7], muli.sign, muli.sat, z[15:8]);
        nz = sign_invert16(z);
        sat_sel(.sel(mux), .r(z), .nr(nz), .mulres(mulo.mul_res));
        //sat_sel(.sel(mux), .r(z), .nr(sign_invert16(z)), .mulres(out_reg));
        //mulo.mul_res = out_reg;
    end
    /*
    assign mulo.mul_res = {muli.opA, muli.opB};
    */

    /*
    initial begin
        vector_component a = 8'b0000_0010;
        vector_component b = 8'b1000_0100;
        high_prec_component pout = product(a, b);
        $display("%d = %d = %b, %d = %d = %b", a, signed'(a), a, b, signed'(b), b);
        $display("The result of the product of and b is");
        $display("%d = %d = %b", pout, signed'(a), pout); 
        $finish; 
    end
    */
endmodule
