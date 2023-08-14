
module systolic_array_tb;

  // Parameters

  //Ports
  reg  clk;
  reg  rst_n;
  reg [4*16-1:0] a;
  reg [4*16-1:0] b;
  wire [16*32-1:0] result;

  systolic_array  systolic_array_inst (
                    .clk(clk),
                    .rst_n(rst_n),
                    .a(a),
                    .b(b),
                    .result(result)
                  );

  //always #5  clk = ! clk ;


  task print_matrix_32;
    input [16*32-1:0]matix;
    integer  i,j;
    begin
      for (i = 0; i < 4; i = i + 1)
      begin
        $write("[");
        for (j = 0; j < 4; j = j + 1)
        begin
          $write("%d ", matix[(4*i+j+1)*32-1-:32]);
        end
        $write("]\n");
      end
    end
  endtask

  reg[15:0] matrix_a[3:0][3:0];
  reg[15:0] matrix_b[3:0][3:0];

  function [16*4-1:0]get_matrix_a;
    input integer step;
    begin
      get_matrix_a = 0;
      for (integer j=0;j<4;j=j+1)
      begin
        if (-step+3+j>=0&&-step+3+j<=3)
        begin
          get_matrix_a[16*(j+1)-1-:16] = matrix_a[-step+3+j][j];
          $display("get_matrix_a[%d] = %d",-step+3+j,matrix_a[-step+3+j][j]);
        end
      end
    end
  endfunction

  function [16*4-1:0]get_matrix_b;
    input integer step;
    begin
      get_matrix_b = 0;
      for (integer i=0;i<4;i=i+1)
      begin
        if (-step+3+i>=0&&-step+3+i<=3)
        begin
          get_matrix_b[16*(i+1)-1-:16] = matrix_b[i][-step+3+i];
          $display("get_matrix_b[%d] = %d",-step+3+i,matrix_b[i][-step+3+i]);
        end
      end
    end
  endfunction

  
  initial
  begin
    clk = 0;
    forever
      #5 clk = ! clk;
  end

  initial
  begin
    rst_n = 0;
    #10 rst_n = 1;
    $display("start test");
  end


  // initial matrix
  initial
  begin
    for (integer i = 0; i < 4; i = i + 1)
    begin
      for (integer j = 0; j < 4; j = j + 1)
      begin
        matrix_a[i][j] = i*4+j+1;
        matrix_b[i][j] = i*4+j+1;
      end
    end
  end

  initial
  begin
    wait(rst_n == 1'b1);
    // #30;
    for (integer step= 0; step<2*4; step=step+1)
    begin
      $display("step = %d",step);
      @(posedge clk);
      a = get_matrix_a(step);
      b = get_matrix_b(step);
      print_matrix_32(result);
    end
    repeat(4) @(posedge clk);
    $display("result:");
    print_matrix_32(result);
    $finish;
  end

  initial
  begin
    $dumpfile("systolic_array.vcd");
    $dumpvars(0, systolic_array_tb);
  end
endmodule
