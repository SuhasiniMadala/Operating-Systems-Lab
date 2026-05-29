#!/bin/bash
# Program Number: 36
# Student Name: Suhasini Madala; Register Number: BT2024043
# Date: 2-5-26
# Description: This script creates FIFO files using mknod and mkfifo
# commands, and compares their efficiency using strace.

echo "===== FIFO File Creation ====="
echo ""

# (a) Create FIFO using mknod command
# mknod creates special files. 'p' means FIFO type.
echo "(a) Creating FIFO using mknod command..."
mknod fifo_mknod p
if [ $? -eq 0 ]; then
    echo "  Created 'fifo_mknod' successfully."
    ls -l fifo_mknod
else
    echo "  fifo_mknod may already exist. Remove it first: rm fifo_mknod"
fi
echo ""

# (b) Create FIFO using mkfifo command
# mkfifo is specifically designed for creating FIFOs.
echo "(b) Creating FIFO using mkfifo command..."
mkfifo fifo_mkfifo
if [ $? -eq 0 ]; then
    echo "  Created 'fifo_mkfifo' successfully."
    ls -l fifo_mkfifo
else
    echo "  fifo_mkfifo may already exist. Remove it first: rm fifo_mkfifo"
fi
echo ""

# (c) Compare efficiency using strace
# strace traces system calls made by a command.
# Fewer system calls = more efficient.
echo "(c) Comparing efficiency using strace:"
echo "  Run these commands to compare:"
echo "    strace mknod fifo_test1 p 2>&1 | wc -l"
echo "    strace mkfifo fifo_test2 2>&1 | wc -l"
echo "  The one with fewer lines (system calls) is more efficient."
echo "  Typically, mkfifo is a wrapper around mknod, so they're similar."
echo ""

# Clean up
echo "Created FIFOs:"
ls -l fifo_mknod fifo_mkfifo 2>/dev/null
echo ""
echo "To clean up: rm fifo_mknod fifo_mkfifo"
