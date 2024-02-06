# It selects instances of the knapsackProblemInstances repository
# and copies them to the instances/ directory.
# It also logs the selected instances in the selected_instances.log file.
#
# Usage:
#   ./select_instances.sh
#       the script must be in the knapsackProblemInstances repository

NUMBER_OF_INSTANCES=100
SELECTED_INSTANCES=$(find . -name test.in | shuf -n $NUMBER_OF_INSTANCES)
OUTPUT_DIR="instances/"
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
