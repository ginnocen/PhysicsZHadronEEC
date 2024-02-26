#!/bin/bash

# Number of threads per instance
nThread=14

# Array to hold the names of the output files
declare -a outputFileNames

# Loop over the number of chunks
for (( chunk=1; chunk<=nThread; chunk++ ))
do
    outputFileName="output_${chunk}.root"
    outputFileNames+=($outputFileName)
    echo "Starting analysis of chunk $chunk"
    ./ExecuteCorrelationAnalysis $1 $2 $3 $4 $5 $6 $7 $8 $9 --nThread $nThread --nChunk $chunk --Output $outputFileName &
done

# Wait for all background processes to finish
wait

# Merge the output files into a single file
echo "Merging output files into output.root"
hadd -f output.root "${outputFileNames[@]}"
# Remove the individual chunk files
echo "Cleaning up individual chunk files..."
for fileName in "${outputFileNames[@]}"
do
    rm -f $fileName
done
echo "All chunks have been processed and merged into output.root."
