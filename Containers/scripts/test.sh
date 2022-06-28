#!/usr/bin/env bash
rm -rf containers_test
git clone https://github.com/mli42/containers_test.git
cd ./containers_test/
./do.sh vector map stack set
echo
echo "########## Finished! ##########"