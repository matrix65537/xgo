//Go语言匿名组合
package main

import (
	"fmt"
)

type Animal struct {
	x int
}

func (cat *Animal) eat() {
	fmt.Println("Animal eat")
}

type Cat struct {
	Animal
	y int
}

func NewCat() *Cat {
	return &Cat{}
}

func (cat *Cat) eat() {
	fmt.Println("cat eat")
}

type Dog struct {
}

func NewDog() *Dog {
	return &Dog{}
}

func (dog *Dog) eat() {
	fmt.Println("dog eat")
}

func main() {
	var animal Animal
	var cat *Cat
	var dog *Dog

	cat = NewCat()
	cat.eat()

	dog = NewDog()
	dog.eat()

	animal = NewCat()
	animal.eat()

	animal = NewDog()
	animal.eat()
}
