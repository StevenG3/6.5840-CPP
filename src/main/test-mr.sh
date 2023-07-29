#!/bin/bash

rm -rf mr-tmp
mkdir mr-tmp || exit 1
cd mr-tmp || exit 1
rm -f mr-*

(cd ../../mrapps && g++ -Wall -fPIC -shared -o wc.so wc.cpp) || exit 1
(cd .. && g++ $RACE ../mrapps/wc.cpp ../util/log.cpp mrsequential.cpp -I../../include -o mrsequential -std=c++17 -ldl) || exit 1

#########################################################
# first word-count

# generate the correct output
../mrsequential ../../mrapps/wc.so ../pg*txt || exit 1
# sort mr-out-0 > mr-correct-wc.txt
# rm -f mr-out*

echo '***' Starting wc test.