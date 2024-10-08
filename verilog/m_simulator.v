module m_simulator(w_clock, w_cc);
  input wire w_clock;
  input wire [31:0] w_cc;

  reg [15:0] r_in1;
  reg [31:0] r_in2;

  wire [31:0] w_y;

  initial begin #50
    #100 r_in1 = 1; r_in2 = 2;
    #100 r_in1 = 3; r_in2 = 4;
    #100 r_in1 = 5; r_in2 = 6;
    #100 r_in1 = 7; r_in2 = 8;
    #100 r_in1 = 0; r_in2 = 0;
  end

  m_multiply_adder m (w_clock, r_in1, r_in2, w_y);

  initial #99 forever #100 $display("CC%1d %5d (x 3) %5d %5d", w_cc, m.r_b, m.r_c, m.r_y);
  initial #800 $finish;
endmodule

module m_multiply_adder(w_clock, w_b, w_c, r_y);
  input wire w_clock;

  input wire [15:0] w_b;
  input wire [31:0] w_c;

  output reg [31:0] r_y = 0;

  reg [15:0] r_b= 0;
  reg [31:0] r_c= 0;

  always @(posedge w_clock) begin
    r_b <= w_b;
    r_c <= w_c;
    r_y <= (16'd3 * r_b) + r_c;
  end
endmodule
