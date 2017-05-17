//带缓冲的通道
package main

import (
	"fmt"
)

func main() {
	ch1 := make(chan int, 0x04)
	ch1 <- 1
	fmt.Println("write 1 finish")
	ch1 <- 2
	fmt.Println("write 2 finish")
	ch1 <- 3
	fmt.Println("write 3 finish")
	ch1 <- 4
	fmt.Println("write 4 finish")
	ch1 <- 5
	fmt.Println("write 5 finish")
	fmt.Println("Finish")
}
