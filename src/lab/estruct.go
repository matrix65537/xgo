package main

import (
	"fmt"
)

type A struct {
}

type B struct {
}

func main() {
	var c struct{}
	var d struct{}
	a := &A{}
	b := &B{}
	fmt.Println(a)
	fmt.Println(b)
	fmt.Println(&a)
	fmt.Println(&b)

	fmt.Printf("%p\n", c)
	fmt.Printf("%p\n", d)
	fmt.Println("Finish")
}
