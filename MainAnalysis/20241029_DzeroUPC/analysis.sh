###bash script to run the analysis code for D0 meson in UPC data

 ./ExecuteDzeroUPC --Input Sample/MergedOutput.root  --MinDzeroPT 3 --MaxDzeroPT 6 --MinDzeroY -2 --MaxDzeroY -1 --Output outputUPCpt2to4_ym2ym1.root &
 ./ExecuteDzeroUPC --Input Sample/MergedOutput.root  --MinDzeroPT 3 --MaxDzeroPT 6 --MinDzeroY -1 --MaxDzeroY 0 --Output outputUPCpt2to4_ym1to0.root &
 ./ExecuteDzeroUPC --Input Sample/MergedOutput.root  --MinDzeroPT 3 --MaxDzeroPT 6 --MinDzeroY 0 --MaxDzeroY 1 --Output outputUPCpt2to4_y0to1.root &
 ./ExecuteDzeroUPC --Input Sample/MergedOutput.root  --MinDzeroPT 3 --MaxDzeroPT 6 --MinDzeroY 1 --MaxDzeroY 2 --Output outputUPCpt2to4_y1to2.root &
