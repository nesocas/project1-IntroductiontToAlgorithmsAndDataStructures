# Project 1 - Introduction to Algorithms and Data Structures (2022/2023)
### Submission Date: March 24th, 2023

## Overview
This project implements a public transportation management system. Users can define bus stops and lines, add connections, query routes and intersections through a command-line interface.

## Features
- Add and list bus lines and stops
- Add connections between stops with cost and duration
- List intersections between lines
- Supports circular routes
- Maximum: 200 lines, 10.000 stops and 30.000 connections.

## Compilation
gcc -O3 -Wall -Wextra -Werror -ansi -pedantic -o proj1 *.c

## Notes
- Uses only standard C libraries: stdio.h, stdlib.h, string.h, ctype.h
- Incremental development and testing recommended

## Project Structure
- project1.c - main implementation
- enunciado.md, guidelines.md, informacoes.md - project intructions, guidelines and relevant information
