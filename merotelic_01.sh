#! /home/ebanigan/kineto/merotely/
#$ -cwd
#$ -t 1-30:1
seed=$(echo "scale=0; ($SGE_TASK_ID+0);" | bc)
modeleven=$(echo "scale=0; (($SGE_TASK_ID+10)%11);" | bc)
modseven=$(echo "scale=0; (($SGE_TASK_ID+6)%7);" | bc)
modtwo=$(echo "scale=0; ($SGE_TASK_ID%2);" | bc)
modthree=$(echo "scale=0; (($SGE_TASK_ID+2)%3);" | bc)
load=$(echo "scale=6; (0.);" | bc)
numf=$(echo "scale=0; (30);" | bc)
mero=$(echo "scale=0; (1);" | bc)
nummero=$(echo "scale=0; (2+3*$modthree*$modthree);" | bc)
pflux=$(echo "scale=6; (0.025);" | bc)
poly=$(echo "scale=6; (0.070);" | bc)
depoly=$(echo "scale=6; (2.0);" | bc)
res=$(echo "scale=6; (0.0020);" | bc)
cat=$(echo "scale=6; (0.00030);" | bc)
pex=$(echo "scale=6; (0.050+.005*5.);" | bc)
dex=$(echo "scale=6; (-0.100);" | bc)
rex=$(echo "scale=8; (0.200+.013333333*5.);" | bc)
cex=$(echo "scale=6; (-0.3);" | bc)
simtime=$(echo "scale=1; (100000);" | bc)
skip=$(echo "scale=0; (50000);" | bc)
numkt=$(echo "scale=0; (2);" | bc)
attrange=$(echo "scale=2; (20.);" | bc)
det=$(echo "scale=8; (0.00001);" | bc)
detfactor=$(echo "scale=3; (8.0);" | bc)
gdetexp=$(echo "scale=6; (0.0);" | bc)
sdetexp=$(echo "scale=6; (0.0);" | bc)
tstep=$(echo "scale=8; (0.0001);" | bc)
inf=$(echo "scale=0; (0);" | bc)
neg=$(echo "scale=0; (0);" | bc)
ktmtspring=$(echo "scale=1; (1.);" | bc)
ktktspring=$(echo "scale=4; (1.);" | bc)
drag=$(echo "scale=4; (1.);" | bc)
nohup ./runlangevin -s $seed -l $load -f $numf -w $pex -x $dex -y $rex -z $cex -t $simtime -k $skip -n $numkt -r $attrange -u $det -d $tstep -b $inf -m $neg -p $ktmtspring -c $cat -g $poly -e $depoly -j $res -F $detfactor -G $gdetexp -S $sdetexp -v $ktktspring -q $drag -P $pflux -M $mero -i $nummero