package main

import (
	"fmt"
)

type Pool struct {
}

func NewPool() *Pool {
	return &Pool{}
}

func (pool *Pool) open() {
	fmt.Println("pool open")
}

func (pool *Pool) close() {
	fmt.Println("pool close")
}

func testdefer(pool *Pool) {
	fmt.Println("before open")
	pool.open()
	fmt.Println("after open")
	fmt.Println("befor defer close")
	defer pool.close()
	fmt.Println("after defer close")
	fmt.Println("do others before return")
}

func main() {
	pool := NewPool()
	testdefer(pool)
	fmt.Println("Finish")
}
