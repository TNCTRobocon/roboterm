cmake_minimum_required(VERSION 2.8)
add_library(shell STATIC
  file.c
  fs.c
  io.c
  shell.c
  pool.c
)