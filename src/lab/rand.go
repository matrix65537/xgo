//使用select构造的随机数(原因：当select监控的多个通道同时有事件，则Go内部随机选择一个)
package main

import (
	"fmt"
)

func rand() (x int) {
	ch := make(chan int, 1)
	select {
	case ch <- 0:
	case ch <- 1:
	}
	x = <-ch
	return
}

func main() {
	for i := 0; i < 8; i++ {
		fmt.Println(rand())
	}
	fmt.Println("Finish")
}
