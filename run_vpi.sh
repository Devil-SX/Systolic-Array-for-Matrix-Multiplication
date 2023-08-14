#!/bin/bash

rtl_path="./rtl"
tb_file="$rtl_path/systolic_array_tb_vpi.sv"
vpi_path="./vpi"
vpi_file="$vpi_path/matrix.c"
# stf_file="$vpi_path/matrix.sft" # SFT files are deprecated. Please pass the VPI module instead.

file_name=$(basename "$tb_file")
top_module=${file_name%.*}
echo "top module: ${top_module}" 

rm $vpi_path/*.vpi
iverilog-vpi $vpi_file
iverilog -g2012 -s $top_module -y$rtl_path -o sim_vpi.vvp $tb_file # $stf_file
vvp -M$vpi_path -mmatrix  -lxt2 sim_vpi.vvp
