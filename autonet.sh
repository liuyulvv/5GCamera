#!/bin/bash

while true
do
    while true
    do
        if [ -c /dev/ttyUSB1 ]
        then
            echo -e "AT^NDISDUP=1,1\r\n" > /dev/ttyUSB1
            sleep 1 
            break
        fi
    done


    cardMac="00:1e:10:1f:00:00"

    for i in `ls /sys/class/net/`
    do 
        mac=`cat /sys/class/net/$i/address`
        if [ $mac == $cardMac ];then
            cardName=$i
            break
        fi
    done


    checkIP()
    {
        ip=`ifconfig $cardName | awk '/inet/{print $2}' | cut -f2 -d ":" |awk 'NR==1 {print $1}'`
        if [ ! -n $ip ];then
            echo "5G network card IP is None"
            return 0
        else
            echo "5G network card IP is $ip"
        fi

        echo "scaumv" | sudo -S sh -c 'echo "nameserver 127.0.0.53\nnameserver 114.114.114.114" > /etc/resolv.conf'

        if ping -I $cardName -c1 www.baidu.com >/dev/null
        then
            echo "ping baidu success"
            return 1
        else
            echo "ping baidu failed"
        fi
        return 0
    }

    if [ ! -n $cardName ];then
        echo "5G network card is None"
    else
        echo "5G network card is $cardName"
        for i in `seq 10`; 
        do
            ifconfig $cardName up
            timeout 10s dhclient $cardName -v
            sleep 1 
            checkIP
            if [ $? -eq 1 ];then
                echo "scaumv" |sudo -S sh -c "echo 100 > /sys/devices/pwm-fan/target_pwm"
                echo "scaumv" |sudo -S sh -c `cd /home/scaumv/5GCamera/build-5GCamera-Desktop-Release && ./5GCamera`
                echo "Everything is OK."
            fi
        done
    fi
done
