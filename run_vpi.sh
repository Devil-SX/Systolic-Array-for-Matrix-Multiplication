#!/bin/bash

rtl_path="./rtl"
tb_file="$rtl_path/systolic_array_tb_vpi.sv"
vpi_path="./vpi"
vpi_file="$vpi_path/matrix.c"
echo $src_files

file_name=$(basename "$tb_file")
top_module=${file_name%.*}
echo "top module: ${top_module}" 

rm $vpi_file
iverilog-vpi $vpi_file
iverilog -g2012 -s $top_module -y$rtl_path -o sim_vpi.vvp $tb_file
vvp -M$vpi_path -mmatrix  -lxt2 sim_vpi.vvp
