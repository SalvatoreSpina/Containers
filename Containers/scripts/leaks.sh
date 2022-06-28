#!/usr/bin/env bash
echo "Testing vector leaks..."
./leaks/leaks_vector | grep "Process "
echo
echo "Testing map leaks..."
./leaks/leaks_map | grep "Process "
echo
echo "Testing stack leaks..."
./leaks/leaks_stack | grep "Process "