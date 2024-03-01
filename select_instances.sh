# It selects instances of the knapsackProblemInstances repository
# and copies them to the instances/ directory.
# It also logs the selected instances in the selected_instances.log file.
#
# Usage:
#   ./select_instances.sh PATH_TO_PROBLEM_INSTANCES_DIRECTORY

CURRENT_DIR=$(realpath $(dirname $0))
RANDOM_SOURCE=$CURRENT_DIR/random_source.txt

NUMBER_OF_INSTANCES=120
SELECTED_INSTANCES=$(find $1 -name test.in | shuf --random-source=$RANDOM_SOURCE -n $NUMBER_OF_INSTANCES)
OUTPUT_DIR="$CURRENT_DIR/instances/"
LOG_FILE=$OUTPUT_DIR/selected_instances.log

idx=1;
mkdir -p $OUTPUT_DIR
cp /dev/null $LOG_FILE

for FILE in $SELECTED_INSTANCES
do
    INSTANCE_FILE=$OUTPUT_DIR/instance_${idx}.txt
    INSTANCE_IDENTIFIER=$(basename $(dirname $FILE))
    echo $INSTANCE_IDENTIFIER >> $LOG_FILE
    cp $FILE $INSTANCE_FILE
    idx=$((idx+1))
done
