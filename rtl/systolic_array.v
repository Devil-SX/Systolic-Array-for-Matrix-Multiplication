module systolic_array (
    input clk,
    input rst_n,

    input [4*16-1:0] a,
    input [4*16-1:0] b,
    output [16*32-1:0] result
  );

// array data flow
  reg[15:0] shift_reg_a[3:0][3:0];
  reg[15:0] shift_reg_b[3:0][3:0];

  integer i;
  integer step;
  always @(posedge clk or negedge rst_n)
  begin
    if (~rst_n)
    begin
      for (i = 0; i < 4; i = i + 1)
      begin
        for (step = 0; step < 4; step = step + 1)
        begin
          shift_reg_a[i][step] <= 0;
          shift_reg_b[i][step] <= 0;
        end
      end
    end
    else
    begin
      for (i = 0; i < 4; i = i + 1) 
      begin
        shift_reg_a[0][i] <= a[16*i+15-:16];
        shift_reg_b[i][0] <= b[16*i+15-:16];
        for (step = 1; step < 4; step = step + 1)
        begin
          shift_reg_a[step][i] <= shift_reg_a[step-1][i];
          shift_reg_b[i][step] <= shift_reg_b[i][step-1];
        end
      end
    end
  end

  // array pe
  genvar k;
  genvar l;
  generate
    for (k = 0; k < 4; k = k + 1) begin
       for (l = 0; l < 4; l = l + 1) begin
          pe pe_inst (
            .clk(clk),
            .rst_n(rst_n),
            .a(shift_reg_a[k][l]),
            .b(shift_reg_b[k][l]),
            .out(result[32*(4*k+l+1)-1-:32])
          );
       end 
    end
  endgenerate


endmodule
