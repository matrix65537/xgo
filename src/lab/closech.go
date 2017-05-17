//通道关闭后另一端不会再接收到数据
package main

import (
	"fmt"
)

func rutine1(ch chan int, length int) {
	for i := 0; i < length; i++ {
		ch <- i

	}
	close(ch)
}

func main() {
	ch := make(chan int)
	go rutine1(ch, 0x10)
	for v := range ch {
		fmt.Println(v)
	}
	fmt.Println("Finish")
}
