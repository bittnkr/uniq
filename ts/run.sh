#!/bin/bash

# Check if a source file was provided as a parameter
if [ "$#" -ne 1 ]; then
  echo -e "\033[31mError: missing source file\033[0m" >&2
  echo "Usage: $0 <source_file>" >&2
  exit 1
fi

source_file=$1

# Compile the AssemblyScript code to WebAssembly
echo -e "\033[33mCompiling AssemblyScript code to WebAssembly...\033[0m"
asc "$source_file" -o "${source_file%.*}.wasm"

if [ ! -f "${source_file%.*}.wasm" ]; then
  echo -e "\033[31mError: WebAssembly binary not found\033[0m" >&2
  exit 1
fi

# Convert the WebAssembly binary to JavaScript
echo -e "\033[33mConverting WebAssembly binary to JavaScript...\033[0m"
wasm2js "${source_file%.*}.wasm" -o "${source_file%.*}.mjs"

if [ ! -f "${source_file%.*}.mjs" ]; then
  echo -e "\033[31mError: JavaScript file not found\033[0m" >&2
  exit 1
fi

# Run the JavaScript file using Node.js
echo -e "\033[33mRunning JavaScript file using Node.js...\033[0m"
node "${source_file%.*}.mjs"
