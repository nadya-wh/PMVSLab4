sudo insmod ./calc.ko
sudo echo "12" > /dev/first
sudo echo "-" > /dev/operation
sudo echo "13" > /dev/second
sudo cat /dev/result
sudo echo "12" > /dev/first
sudo echo "+" > /dev/operation
sudo echo "13" > /dev/second
sudo cat /dev/result
sudo echo "12" > /dev/first
sudo echo "*" > /dev/operation
sudo echo "13" > /dev/second
sudo cat /dev/result
sudo echo "4" > /dev/first
sudo echo "/" > /dev/operation
sudo echo "2" > /dev/second
sudo cat /dev/result
sudo rmmod calc
