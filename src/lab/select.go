//select用于同时监控多个通道
package main

import (
	"fmt"
)

var ch1 = make(chan int)
var ch2 = make(chan int)
var ch3 = make(chan int)
var ch4 = make(chan int)

func S(wait chan int) {
	for {
		select {
		case <-ch1:
			{
				fmt.Println("ch1")
			}
		case <-ch2:
			{
				fmt.Println("ch2")
			}
		case <-ch3:
			{
				fmt.Println("ch3")
			}
		case <-ch4:
			{
				fmt.Println("ch4")
			}
		default:
			{
				fmt.Println("default")
			}
		}
	}
	wait <- 1
}

func main() {
	wait := make(chan int)
	go S(wait)

	ch1 <- 1
	ch4 <- 1
	ch2 <- 1
	ch3 <- 1

	<-wait
	fmt.Println("Finish")
}
