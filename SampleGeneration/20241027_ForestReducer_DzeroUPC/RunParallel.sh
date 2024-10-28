#!/bin/bash

OUTPUT="output"
counter=0
MAXCORES=100
filelist="list.txt"
MERGEDOUTPUT="MergedOutput.root"
rm $MERGEDOUTPUT

# Check the number of physical CPUs
NUM_PHYSICAL_CPUS=$(sysctl -n hw.physicalcpu)

## Verify that MAXCORES is not larger than the number of physical CPUs
#if (( MAXCORES > NUM_PHYSICAL_CPUS )); then
#    echo "Error: MAXCORES ($MAXCORES) exceeds the number of physical CPUs ($NUM_PHYSICAL_CPUS)."
#    exit 1
#fi

# Remove the file if it exists, suppress error if it doesn't
rm -f "$filelist"

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT"

# List all matching files and write to the filelist

#ls  /eos/cms/store/group/phys_heavyions/ginnocen/PbPb2018_gtoccbar/20241023_DiJetpThat15PbPb2018gtoccbar_v1/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/HINPbPbSpring21MiniAOD-FixL1CaloGT_112X_upgrade2018_realistic_HI_v9-v1/MINIAODSIM/DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/20241023_DiJetpThat15PbPb2018gtoccbar_v1/241023_225656/000*/*.root > "$filelist"
ls /data/gtoccbar/20241023_DataPbPb2018gtoccbar_v2/241024_002902/000*/*.root >> "$filelist"
#ls Samples/HiForestAOD_*_Data_pp.root > "$filelist"

# Check if the filelist is empty
if [[ ! -s "$filelist" ]]; then
    echo "No matching files found in Samples directory."
    exit 1
fi

echo "File list created successfully: $filelist"
rm -rf $OUTPUT
mkdir $OUTPUT
# Loop through each file in the file list
while IFS= read -r file; do
            echo "Processing $file"
            ./Execute --Input "$file" \
            --Output "$OUTPUT/output_$counter.root" \
            --Year 2018 \
            --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
            --DoGenLevel false \
            --IsData false \
            --IsPP false \
            --IsBackground false \
            --CheckZ true \
            --MinZPT 30 \
            --MinTrackPT 1 \
            --DoSumET false \
            --MuonVeto 0.01 \
            --DoTrackResidual false &
    ((counter++))
    if (( counter % $MAXCORES == 0 )); then
        wait
    fi
done < "$filelist"
wait 

hadd $MERGEDOUTPUT $OUTPUT/output_*.root
echo "All done!"
echo "Merged output file: $MERGEDOUTPUT"

