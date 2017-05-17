package main

import (
	"fmt"
	"time"
)

var cht = make(chan bool, 1)

func timeout(v time.Duration) {
	time.Sleep(v)
	cht <- true
}

func main() {
	ch1 := make(chan int)
	go timeout(3e9)
	select {
	case <-ch1:
	case <-cht:
		{
			fmt.Println("timeout")
		}
	}
	fmt.Println("Finish")
}
