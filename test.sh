sudo insmod ./calc.ko
sudo echo "12" > /dev/first
sudo echo "-" > /dev/operation
sudo echo "13" > /dev/second
result=$(cat /dev/result)
if [ "$result" -eq "-1" ]; then
    echo "Test 1 passed"
else
    echo "Test 1 failed"
fi
sudo echo "12" > /dev/first
sudo echo "+" > /dev/operation
sudo echo "13" > /dev/second
result=$(cat /dev/result)
if [ "$result" -eq "25" ]; then
    echo "Test 2 passed"
else
    echo "Test 2 failed"
fi
sudo echo "2" > /dev/first
sudo echo "*" > /dev/operation
sudo echo "3" > /dev/second
result=$(cat /dev/result)
if [ "$result" -eq "6" ]; then
    echo "Test 3 passed"
else
    echo "Test 3 failed"
fi
sudo echo "4" > /dev/first
sudo echo "/" > /dev/operation
sudo echo "2" > /dev/second
result=$(cat /dev/result)
if [ "$result" -eq "2" ]; then
    echo "Test 4 passed"
else
    echo "Test 4 failed"
fi
sudo rmmod calc
