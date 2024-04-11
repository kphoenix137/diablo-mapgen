#!/bin/bash

# Set default values for arguments
start_offset=0
total_count=2179583646
scanner="pattern"
num_threads=$(nproc)
num_processes=$((num_threads - 1))

# Function to display usage information
usage() {
    echo "Usage: $0 [--scanner <${scanner}>] [--count <${total_count}>] [--start <${start_offset}>] [--threads <${num_processes}>]"
    exit 1
}

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        --scanner)
            scanner="$2"
            shift
            shift
            ;;
        --count)
            total_count="$2"
            shift
            shift
            ;;
        --start)
            start_offset="$2"
            shift
            shift
            ;;
        --threads)
            num_processes="$2"
            shift
            shift
            ;;
        *)
            usage
            ;;
    esac
done

# Cap total_count
if ((total_count < 1)); then
    total_count=1
fi
if ((total_count > 4294967295)); then
    total_count=4294967295
fi

# Define the batch size (number of IDs to scan per process)
batch_size=$((total_count / num_processes + 1))

# Define the command to run
command="./diablo-mapgen --scanner $scanner"

# Function to send SIGTERM to the process group ID of the main script
sigint_handler() {
    kill -- -$$
    exit 1
}
trap 'sigint_handler' SIGINT

# Loop through batches and run the command with different offsets
for ((i = 0; i < num_processes; i++)); do
    # Calculate the start offset for this batch
    offset=$((start_offset + i * batch_size))

    # Calculate the actual count for this batch
    end_offset=$((offset + batch_size))
    if ((end_offset > total_count)); then
        actual_count=$((total_count - offset))
    else
        actual_count=$((batch_size))
    fi

    # Run the command with the current offset and count
    $command --start $offset --count $actual_count &
done

# Wait for the processes to finish
wait
